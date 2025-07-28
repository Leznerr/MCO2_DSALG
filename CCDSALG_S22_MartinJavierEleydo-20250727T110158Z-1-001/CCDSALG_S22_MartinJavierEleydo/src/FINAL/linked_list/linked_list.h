/* ============================================================================
 *  linked_list.h – Generic Singly Linked List for CCDSALG MCO-2
 * ----------------------------------------------------------------------------
 *  Provides a flexible, student-friendly API for a singly linked list,
 *  where each node holds a void* pointer for arbitrary user data.
 *
 *  Why use this module?
 *    • Handles memory management for all nodes
 *    • Allows user to supply a cleanup callback for node data, for safe destruction
 *    • O(1) prepend, O(n) append/search/remove (classic linked list complexity)
 *    • Iterator helpers allow simple for-loop traversal
 *    • Used as a foundational DS for stacks, queues, adjacency lists, etc.
 *
 *  Design principles:
 *    - No assumptions about payload structure (fully generic)
 *    - Minimalist, safe, and easy to integrate in C projects
 *    - Documentation, naming, and behaviors are rubric-aligned for MCO-2
 * ============================================================================
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 *  OPAQUE TYPES
 * --------------------------------------------------------------------------
 *  LinkedList: User interacts via pointer to this struct; internals are hidden.
 *  ListNode:   For iteration, pointer-walking, and debugging.
 * -------------------------------------------------------------------------- */
typedef struct LinkedList LinkedList;
typedef struct ListNode ListNode;

/* --------------------------------------------------------------------------
 *  FUNCTION TYPE: list_free_func_t
 * --------------------------------------------------------------------------
 *  Signature for a function to free/cleanup data payloads in nodes
 *  (e.g., called in list_destroy).
 *  Can be NULL if no cleanup is needed (e.g., for static data).
 * -------------------------------------------------------------------------- */
typedef void (*list_free_func_t)(void *data);

/* ==========================================================================
 *  LIFECYCLE FUNCTIONS
 * ==========================================================================
 */
/**
 * Create a new empty linked list.
 * @param free_func Optional callback to free node data during destruction.
 *                  Pass NULL if no special cleanup is needed.
 * @return Pointer to the new list, or NULL if allocation fails.
 */
LinkedList *list_create(list_free_func_t free_func);

/**
 * Destroy the entire list and all its nodes.
 * Calls free_func (if provided) on each data pointer before node deletion.
 * Safe to call on NULL.
 */
void list_destroy(LinkedList *list);

/* ==========================================================================
 *  INSERTION OPERATIONS
 * ==========================================================================
 */
/**
 * Insert data at the front (head) of the list. (O(1))
 * @param data Pointer to be stored in the new node.
 * @return true on success, false on allocation failure or invalid list.
 */
bool list_prepend(LinkedList *list, void *data);

/**
 * Insert data at the end (tail) of the list. (O(1) if tail pointer is kept, else O(n))
 * @param data Pointer to be stored in the new node.
 * @return true on success, false on allocation failure or invalid list.
 */
bool list_append(LinkedList *list, void *data);

/**
 * Insert data at the specified position (0-based index). (O(n))
 * If index is 0, prepends. If index >= size, appends.
 * @return true on success, false on allocation failure or invalid list.
 */
bool list_insert_at(LinkedList *list, size_t index, void *data);

/* ==========================================================================
 *  REMOVAL OPERATIONS
 * ==========================================================================
 */
/**
 * Remove and return the data at the front (head) of the list. (O(1))
 * @return The pointer from the removed node, or NULL if list is empty.
 */
void *list_pop_front(LinkedList *list);

/**
 * Remove and return the data at the end (tail) of the list. (O(n))
 * @return The pointer from the removed node, or NULL if list is empty.
 */
void *list_pop_back(LinkedList *list);

/**
 * Remove the first node whose data pointer equals 'data'. (O(n))
 * Uses pointer (not value) comparison.
 * @return true if found and removed, false otherwise.
 */
bool list_remove(LinkedList *list, void *data);

/**
 * Remove and return the data at a specific index. (O(n))
 * @param index 0-based index.
 * @return The pointer from the removed node, or NULL if index is out of bounds.
 */
void *list_remove_at(LinkedList *list, size_t index);

/* ==========================================================================
 *  ACCESS OPERATIONS
 * ==========================================================================
 */
/**
 * Get the data pointer at the specified index (without removing). (O(n))
 * @param index 0-based index.
 * @return The data pointer, or NULL if out of bounds.
 */
void *list_get_at(LinkedList *list, size_t index);

/**
 * Get the data pointer at the head (front) of the list, without removing. (O(1))
 * @return The pointer, or NULL if the list is empty.
 */
void *list_peek_front(LinkedList *list);

/**
 * Get the data pointer at the tail (back) of the list, without removing. (O(1))
 * @return The pointer, or NULL if the list is empty.
 */
void *list_peek_back(LinkedList *list);

/* ==========================================================================
 *  QUERY OPERATIONS
 * ==========================================================================
 */
/**
 * Returns true if the list is empty, false otherwise.
 */
bool list_is_empty(LinkedList *list);

/**
 * Returns the number of nodes currently in the list.
 */
size_t list_size(LinkedList *list);

/**
 * Find the index of the first node whose data pointer equals 'data'. (O(n))
 * Uses pointer (not value) comparison.
 * @return Index if found, or SIZE_MAX if not found.
 */
size_t list_find(LinkedList *list, void *data);

/* ==========================================================================
 *  ITERATION HELPERS
 * ==========================================================================
 */
/**
 * Get the first node in the list (for iteration).
 * @return Pointer to the first node, or NULL if list is empty.
 */
ListNode *list_begin(LinkedList *list);

/**
 * Advance to the next node in the list (for iteration).
 * @param node Current node.
 * @return Pointer to the next node, or NULL if at end.
 */
ListNode *list_node_next(ListNode *node);

/**
 * Retrieve the data pointer stored in a node (for iteration).
 * @param node Node pointer.
 * @return The data pointer stored in the node, or NULL.
 */
void *list_node_data(ListNode *node);

#ifdef __cplusplus
}
#endif

#endif /* LINKED_LIST_H */
