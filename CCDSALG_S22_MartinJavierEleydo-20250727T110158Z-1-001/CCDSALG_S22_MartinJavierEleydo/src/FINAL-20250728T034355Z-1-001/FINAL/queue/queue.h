/* ============================================================================
 *  queue.h – Generic Queue for CCDSALG MCO-2
 *  ----------------------------------------------------------------------------
 *  Provides a generic queue with void* payloads for maximum flexibility.
 *  Used by BFS algorithm.
 *
 *  Features:
 *      • Generic void* data storage
 *      • O(1) enqueue/dequeue operations
 *      • Dynamic growth
 *      • Circular buffer implementation for efficiency
 * ==========================================================================*/

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  OPAQUE TYPE                                                               */
/* -------------------------------------------------------------------------- */
typedef struct Queue Queue;

/* -------------------------------------------------------------------------- */
/*  LIFECYCLE                                                                 */
/* -------------------------------------------------------------------------- */
/**
 * Create a new empty queue.
 * @param init_cap Initial capacity (will default to reasonable size if 0)
 * @return New Queue instance, or NULL on allocation failure
 */
Queue *queue_create(size_t init_cap);

/**
 * Destroy the queue and free all memory.
 * Does not free the data pointers stored in the queue.
 */
void queue_destroy(Queue *queue);

/* -------------------------------------------------------------------------- */
/*  OPERATIONS                                                                */
/* -------------------------------------------------------------------------- */
/**
 * Add data to the back of the queue (O(1) amortized).
 * @return true on success, false on allocation failure
 */
bool queue_enqueue(Queue *queue, void *data);

/**
 * Remove and return the front element (O(1)).
 * @return The data from the front of the queue, or NULL if empty
 */
void *queue_dequeue(Queue *queue);

/**
 * Peek at the front element without removing it (O(1)).
 * @return The data from the front of the queue, or NULL if empty
 */
void *queue_peek(Queue *queue);

/* -------------------------------------------------------------------------- */
/*  QUERY                                                                     */
/* -------------------------------------------------------------------------- */
/**
 * Check if the queue is empty.
 */
bool queue_is_empty(Queue *queue);

/**
 * Get the number of elements in the queue.
 */
size_t queue_size(Queue *queue);

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_H */
