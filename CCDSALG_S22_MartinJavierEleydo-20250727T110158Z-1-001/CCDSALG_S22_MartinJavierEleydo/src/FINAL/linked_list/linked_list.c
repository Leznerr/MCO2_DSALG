/* ============================================================================
 *  linked_list.c – Generic Singly Linked List implementation
 * ----------------------------------------------------------------------------
 *  Implements all standard singly linked list operations:
 *    - Insertion (prepend, append, insert at index)
 *    - Removal (pop front/back, remove at index, remove by pointer)
 *    - Access (get by index, peek front/back)
 *    - Query (size, empty, find by pointer)
 *    - Iteration helpers (node traversal)
 *
 *  All operations are memory-safe, handle errors robustly, and use a
 *  user-supplied free function for payloads (if needed).
 * ============================================================================
 */

#include "linked_list.h"
#include <stdlib.h>
#include <stdint.h>

/* ----------------------------------------------------------------------------
 *  INTERNAL STRUCTURES
 * ----------------------------------------------------------------------------
 *  - ListNode: Represents a single node in the list, storing void* data and a pointer
 *    to the next node.
 *  - LinkedList: Maintains pointers to head/tail, size, and the free function
 *    to call on each element during destruction.
 * ----------------------------------------------------------------------------
 */
struct ListNode {
    void *data;
    struct ListNode *next;
};

struct LinkedList {
    ListNode *head;             // Points to the first node (or NULL if empty)
    ListNode *tail;             // Points to the last node (for fast append)
    size_t size;                // Number of nodes in the list
    list_free_func_t free_func; // Optional: called on data in list_destroy
};

/* ----------------------------------------------------------------------------
 *  LIFECYCLE FUNCTIONS
 * ----------------------------------------------------------------------------
 *  list_create:  Allocates and returns a new empty linked list.
 *                Caller may pass a custom free_func to manage payloads,
 *                or NULL if not needed.
 *
 *  list_destroy: Frees all nodes in the list (and their data using free_func if set).
 *                Safe to call on NULL.
 * ----------------------------------------------------------------------------
 */
LinkedList *list_create(list_free_func_t free_func)
{
    LinkedList *list = malloc(sizeof(LinkedList));
    if (!list) return NULL;
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->free_func = free_func;
    
    return list;
}

void list_destroy(LinkedList *list)
{
    if (!list) return;
    
    ListNode *current = list->head;
    while (current) {
        ListNode *next = current->next;
        if (list->free_func && current->data) {
            list->free_func(current->data);
        }
        free(current);
        current = next;
    }
    
    free(list);
}

/* ----------------------------------------------------------------------------
 *  NODE CREATION HELPER
 * ----------------------------------------------------------------------------
 *  create_node: Allocates and initializes a new list node.
 * ----------------------------------------------------------------------------
 */
static ListNode *create_node(void *data)
{
    ListNode *node = malloc(sizeof(ListNode));
    if (!node) return NULL;
    
    node->data = data;
    node->next = NULL;
    return node;
}

/* ----------------------------------------------------------------------------
 *  INSERTION OPERATIONS
 * ----------------------------------------------------------------------------
 *  list_prepend: Insert new node at the front of the list (O(1)).
 *  list_append:  Insert new node at the end of the list (O(1)).
 *  list_insert_at: Insert at a specified index (O(n)). If index==0, prepends.
 *                  If index ≥ size, appends.
 * ----------------------------------------------------------------------------
 */
bool list_prepend(LinkedList *list, void *data)
{
    if (!list) return false;
    
    ListNode *new_node = create_node(data);
    if (!new_node) return false;
    
    new_node->next = list->head;
    list->head = new_node;
    
    if (!list->tail) {
        list->tail = new_node;
    }
    
    list->size++;
    return true;
}

bool list_append(LinkedList *list, void *data)
{
    if (!list) return false;
    
    ListNode *new_node = create_node(data);
    if (!new_node) return false;
    
    if (!list->head) {
        list->head = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    
    list->size++;
    return true;
}

bool list_insert_at(LinkedList *list, size_t index, void *data)
{
    if (!list) return false;
    
    if (index == 0) {
        return list_prepend(list, data);
    }
    
    if (index >= list->size) {
        return list_append(list, data);
    }
    
    ListNode *new_node = create_node(data);
    if (!new_node) return false;
    
    ListNode *current = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        current = current->next;
    }
    
    new_node->next = current->next;
    current->next = new_node;
    
    list->size++;
    return true;
}

/* ----------------------------------------------------------------------------
 *  REMOVAL OPERATIONS
 * ----------------------------------------------------------------------------
 *  list_pop_front: Remove and return the payload at the head. O(1).
 *  list_pop_back:  Remove and return the payload at the tail. O(n).
 *  list_remove:    Remove first node with pointer-equal data (no free), O(n).
 *  list_remove_at: Remove node at given index, return its data, O(n).
 * ----------------------------------------------------------------------------
 */
void *list_pop_front(LinkedList *list)
{
    if (!list || !list->head) return NULL;
    
    ListNode *old_head = list->head;
    void *data = old_head->data;
    
    list->head = old_head->next;
    if (!list->head) {
        list->tail = NULL;
    }
    
    free(old_head);
    list->size--;
    
    return data;
}

void *list_pop_back(LinkedList *list)
{
    if (!list || !list->head) return NULL;
    
    if (list->head == list->tail) {
        void *data = list->head->data;
        free(list->head);
        list->head = list->tail = NULL;
        list->size--;
        return data;
    }
    
    ListNode *current = list->head;
    while (current->next != list->tail) {
        current = current->next;
    }
    
    void *data = list->tail->data;
    free(list->tail);
    list->tail = current;
    current->next = NULL;
    list->size--;
    
    return data;
}

bool list_remove(LinkedList *list, void *data)
{
    // Remove first node whose data pointer equals 'data'
    if (!list || !list->head) return false;
    
    if (list->head->data == data) {
        list_pop_front(list);
        return true;
    }
    
    ListNode *current = list->head;
    while (current->next && current->next->data != data) {
        current = current->next;
    }
    
    if (!current->next) return false;
    
    ListNode *to_remove = current->next;
    current->next = to_remove->next;
    
    if (to_remove == list->tail) {
        list->tail = current;
    }
    
    free(to_remove);
    list->size--;
    return true;
}

void *list_remove_at(LinkedList *list, size_t index)
{
    // Remove node at given index, return its data
    if (!list || index >= list->size) return NULL;
    
    if (index == 0) {
        return list_pop_front(list);
    }
    
    ListNode *current = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        current = current->next;
    }
    
    ListNode *to_remove = current->next;
    void *data = to_remove->data;
    current->next = to_remove->next;
    
    if (to_remove == list->tail) {
        list->tail = current;
    }
    
    free(to_remove);
    list->size--;
    
    return data;
}

/* ----------------------------------------------------------------------------
 *  ACCESS OPERATIONS
 * ----------------------------------------------------------------------------
 *  list_get_at:    Return data at a given index, or NULL if out of bounds. O(n).
 *  list_peek_front:Return data at the head, or NULL if list empty. O(1).
 *  list_peek_back: Return data at the tail, or NULL if list empty. O(1).
 * ----------------------------------------------------------------------------
 */
void *list_get_at(LinkedList *list, size_t index)
{
    if (!list || index >= list->size) return NULL;
    
    ListNode *current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    
    return current->data;
}

void *list_peek_front(LinkedList *list)
{
    if (!list || !list->head) return NULL;
    return list->head->data;
}

void *list_peek_back(LinkedList *list)
{
    if (!list || !list->tail) return NULL;
    return list->tail->data;
}

/* ----------------------------------------------------------------------------
 *  QUERY OPERATIONS
 * ----------------------------------------------------------------------------
 *  list_is_empty: Returns true if list has no elements.
 *  list_size:     Returns the number of elements in the list.
 *  list_find:     Returns index of the first node with matching data pointer,
 *                 or SIZE_MAX if not found.
 * ----------------------------------------------------------------------------
 */
bool list_is_empty(LinkedList *list)
{
    return !list || list->size == 0;
}

size_t list_size(LinkedList *list)
{
    return list ? list->size : 0;
}

size_t list_find(LinkedList *list, void *data)
{
    if (!list) return SIZE_MAX;
    
    ListNode *current = list->head;
    for (size_t i = 0; i < list->size; i++) {
        if (current->data == data) {
            return i;
        }
        current = current->next;
    }
    
    return SIZE_MAX;
}

/* ----------------------------------------------------------------------------
 *  ITERATION HELPERS
 * ----------------------------------------------------------------------------
 *  list_begin:     Returns pointer to first node, or NULL.
 *  list_node_next: Returns next node pointer, or NULL.
 *  list_node_data: Returns the data pointer at the node, or NULL.
 *
 *  These are for simple C-style iteration:
 *      for (ListNode *n = list_begin(list); n; n = list_node_next(n)) ...
 * ----------------------------------------------------------------------------
 */
ListNode *list_begin(LinkedList *list)
{
    return list ? list->head : NULL;
}

ListNode *list_node_next(ListNode *node)
{
    return node ? node->next : NULL;
}

void *list_node_data(ListNode *node)
{
    return node ? node->data : NULL;
}
