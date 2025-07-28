/* ============================================================================
 *  FILE: heap.h – Min‑Heap / Priority Queue for CCDSALG MCO‑2 (support for P2/P3)
 * ----------------------------------------------------------------------------
 *  A generic binary min-heap ("priority queue") structure for algorithms
 *  such as Prim’s and Dijkstra’s (required in most DSAL curricula).
 *
 *  Key Features:
 *    • Priorities (keys) are non-negative integers; smallest key always at root.
 *    • Payload is generic (void*) so heap can store Vertex*, struct pointers, etc.
 *    • O(log n) operations: insert, extract-min, decrease-key (all logarithmic).
 *    • Grows dynamically as needed; initial capacity set by caller for efficiency.
 *    • Designed for student use: *no internal hash map*, so index returned by
 *      heap_push() must be tracked by the caller if decrease-key will be needed.
 *    • Minimal, clear interface, in line with "Bonus – Auxiliary DS" rubric.
 *
 *  Typical Usage:
 *    - Used for greedy algorithms needing fast access to smallest element
 *      (e.g., Dijkstra’s shortest path, Prim’s MST, A* search).
 *    - Insert any pointer and integer key, and retrieve the min efficiently.
 *    - Heap does NOT manage memory for user data; only for its own structure.
 *
 *  API Summary (see details below):
 *    Heap   *heap_create(size_t cap);          // Create new heap (cap ≥ 1)
 *    void    heap_destroy(Heap*);
 *    size_t  heap_push(Heap*, void *item, int key); // Insert, returns index or SIZE_MAX
 *    bool    heap_is_empty(const Heap*);
 *    void   *heap_extract_min(Heap*, int *out_key); // Remove min, optionally get key
 *    bool    heap_decrease_key(Heap*, size_t idx, int new_key); // Lower priority
 * ============================================================================
 */

#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stddef.h>   // for size_t

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * OPAQUE HEAP TYPE
 * ----------------------------------------------------------------------------
 *  The Heap struct’s contents are hidden (information hiding), so users
 *  cannot access internal arrays directly—only via these functions.
 * ============================================================================
 */
typedef struct Heap Heap;

/* ============================================================================
 * heap_create
 * ----------------------------------------------------------------------------
 *  Allocate and initialize a new empty heap with at least @p init_cap capacity.
 *  If @p init_cap is zero, a reasonable default is used (e.g., 16).
 *  Returns pointer to heap, or NULL on allocation failure.
 * ============================================================================
 */
Heap  *heap_create(size_t init_cap);

/* ============================================================================
 * heap_destroy
 * ----------------------------------------------------------------------------
 *  Frees all internal memory for the heap structure. Does *not* free any
 *  user data (payloads) stored in the heap.
 *  Safe to call on NULL.
 * ============================================================================
 */
void    heap_destroy(Heap *h);

/* ============================================================================
 * heap_push
 * ----------------------------------------------------------------------------
 *  Inserts @p item with integer priority @p key into the heap.
 *  Returns: the index (array position) where the item was inserted.
 *  - Caller *must* track this index to use heap_decrease_key later!
 *  - Returns SIZE_MAX if allocation or resizing fails.
 *  Runs in O(log n).
 * ============================================================================
 */
size_t  heap_push(Heap *h, void *item, int key);

/* ============================================================================
 * heap_is_empty
 * ----------------------------------------------------------------------------
 *  Returns true if heap is empty or NULL, false otherwise.
 * ============================================================================
 */
bool    heap_is_empty(const Heap *h);

/* ============================================================================
 * heap_extract_min
 * ----------------------------------------------------------------------------
 *  Removes and returns the payload (void*) with the smallest key in the heap.
 *  If @p out_key is not NULL, the key is stored there.
 *  Returns NULL if heap is empty or NULL.
 *  Runs in O(log n).
 * ============================================================================
 */
void   *heap_extract_min(Heap *h, int *out_key);

/* ============================================================================
 * heap_decrease_key
 * ----------------------------------------------------------------------------
 *  Decreases the key (priority) of the element currently at @p idx to @p new_key.
 *  - @p new_key must be strictly smaller than the current key at idx.
 *  - Returns false if idx is out of bounds, if the key is not lower, or on error.
 *  - Restores the heap property after lowering the key.
 *  Runs in O(log n).
 * ============================================================================
 */
bool    heap_decrease_key(Heap *h, size_t idx, int new_key);

#ifdef __cplusplus
}
#endif

#endif /* HEAP_H */
