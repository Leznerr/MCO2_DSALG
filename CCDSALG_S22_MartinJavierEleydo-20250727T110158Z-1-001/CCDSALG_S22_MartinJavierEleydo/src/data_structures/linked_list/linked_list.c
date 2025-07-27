/* ============================================================================
 *  linked_list.c – Generic Singly Linked List implementation
 *  ----------------------------------------------------------------------------
 *  Implements all linked list operations with proper memory management
 *  and error handling.
 * ==========================================================================*/

#include "linked_list.h"
#include <stdlib.h>
#include <stdint.h>

/* -------------------------------------------------------------------------- */
/*  INTERNAL STRUCTURES                                                       */
/* -------------------------------------------------------------------------- */
struct ListNode {
    void *data;
    struct ListNode *next;
};

struct LinkedList {
    ListNode *head;
    ListNode *tail;
    size_t size;
    list_free_func_t free_func;
};

/* -------------------------------------------------------------------------- */
/*  LIFECYCLE                                                                 */
/* -------------------------------------------------------------------------- */
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

/* -------------------------------------------------------------------------- */
/*  HELPER FUNCTIONS                                                          */
/* -------------------------------------------------------------------------- */
static ListNode *create_node(void *data)
{
    ListNode *node = malloc(sizeof(ListNode));
    if (!node) return NULL;
    
    node->data = data;
    node->next = NULL;
    return node;
}

/* -------------------------------------------------------------------------- */
/*  INSERTION                                                                 */
/* -------------------------------------------------------------------------- */
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

/* -------------------------------------------------------------------------- */
/*  REMOVAL                                                                   */
/* -------------------------------------------------------------------------- */
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

/* -------------------------------------------------------------------------- */
/*  ACCESS                                                                    */
/* -------------------------------------------------------------------------- */
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

/* -------------------------------------------------------------------------- */
/*  QUERY                                                                     */
/* -------------------------------------------------------------------------- */
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

/* -------------------------------------------------------------------------- */
/*  ITERATION                                                                 */
/* -------------------------------------------------------------------------- */
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
