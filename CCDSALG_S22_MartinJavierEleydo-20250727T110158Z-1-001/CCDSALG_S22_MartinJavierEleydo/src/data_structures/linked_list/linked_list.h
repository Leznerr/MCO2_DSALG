/* ============================================================================
 *  linked_list.h – Generic Singly Linked List for CCDSALG MCO-2
 *  ----------------------------------------------------------------------------
 *  Provides a generic linked list with void* payloads for maximum flexibility.
 *  Used throughout the project for various data storage needs.
 *
 *  Features:
 *      • Generic void* data storage
 *      • O(1) prepend, O(n) append/search/delete
 *      • Iterator support for traversal
 *      • Memory management with cleanup callbacks
 * ==========================================================================*/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  OPAQUE TYPES                                                              */
/* -------------------------------------------------------------------------- */
typedef struct LinkedList LinkedList;
typedef struct ListNode ListNode;

/* Callback for freeing node data when destroying list */
typedef void (*list_free_func_t)(void *data);

/* -------------------------------------------------------------------------- */
/*  LIFECYCLE                                                                 */
/* -------------------------------------------------------------------------- */
/**
 * Create a new empty linked list.
 * @param free_func Optional callback to free node data (can be NULL)
 * @return New LinkedList instance, or NULL on allocation failure
 */
LinkedList *list_create(list_free_func_t free_func);

/**
 * Destroy the linked list and all its nodes.
 * Calls the free_func on each node's data if provided during creation.
 */
void list_destroy(LinkedList *list);

/* -------------------------------------------------------------------------- */
/*  INSERTION                                                                 */
/* -------------------------------------------------------------------------- */
/**
 * Insert data at the beginning of the list (O(1)).
 * @return true on success, false on allocation failure
 */
bool list_prepend(LinkedList *list, void *data);

/**
 * Insert data at the end of the list (O(n)).
 * @return true on success, false on allocation failure
 */
bool list_append(LinkedList *list, void *data);

/**
 * Insert data at the specified index (O(n)).
 * @param index 0-based index; if >= size, appends to end
 * @return true on success, false on allocation failure
 */
bool list_insert_at(LinkedList *list, size_t index, void *data);

/* -------------------------------------------------------------------------- */
/*  REMOVAL                                                                   */
/* -------------------------------------------------------------------------- */
/**
 * Remove and return the first element (O(1)).
 * @return The data from the removed node, or NULL if list is empty
 */
void *list_pop_front(LinkedList *list);

/**
 * Remove and return the last element (O(n)).
 * @return The data from the removed node, or NULL if list is empty
 */
void *list_pop_back(LinkedList *list);

/**
 * Remove the first occurrence of data (O(n)).
 * Uses pointer comparison, not value comparison.
 * @return true if found and removed, false otherwise
 */
bool list_remove(LinkedList *list, void *data);

/**
 * Remove element at specified index (O(n)).
 * @return The data from the removed node, or NULL if index out of bounds
 */
void *list_remove_at(LinkedList *list, size_t index);

/* -------------------------------------------------------------------------- */
/*  ACCESS                                                                    */
/* -------------------------------------------------------------------------- */
/**
 * Get data at specified index without removing (O(n)).
 * @return The data at index, or NULL if index out of bounds
 */
void *list_get_at(LinkedList *list, size_t index);

/**
 * Get the first element without removing (O(1)).
 * @return The first element's data, or NULL if list is empty
 */
void *list_peek_front(LinkedList *list);

/**
 * Get the last element without removing (O(n)).
 * @return The last element's data, or NULL if list is empty
 */
void *list_peek_back(LinkedList *list);

/* -------------------------------------------------------------------------- */
/*  QUERY                                                                     */
/* -------------------------------------------------------------------------- */
/**
 * Check if the list is empty.
 */
bool list_is_empty(LinkedList *list);

/**
 * Get the number of elements in the list.
 */
size_t list_size(LinkedList *list);

/**
 * Find the first occurrence of data (O(n)).
 * Uses pointer comparison, not value comparison.
 * @return Index of first occurrence, or SIZE_MAX if not found
 */
size_t list_find(LinkedList *list, void *data);

/* -------------------------------------------------------------------------- */
/*  ITERATION                                                                 */
/* -------------------------------------------------------------------------- */
/**
 * Get the first node for iteration.
 * @return Pointer to first node, or NULL if list is empty
 */
ListNode *list_begin(LinkedList *list);

/**
 * Get the next node in iteration.
 * @return Pointer to next node, or NULL if at end
 */
ListNode *list_node_next(ListNode *node);

/**
 * Get the data from a node during iteration.
 * @return The node's data
 */
void *list_node_data(ListNode *node);

#ifdef __cplusplus
}
#endif

#endif /* LINKED_LIST_H */
