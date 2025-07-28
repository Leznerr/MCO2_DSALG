/* ============================================================================
 *  heap.h – Min‑Heap / Priority Queue for CCDSALG MCO‑2 (support for P2/P3)
 *  ----------------------------------------------------------------------------
 *  Feature scope (rubric “Bonus – Auxiliary DS” & required by Prim/Dijkstra):
 *      • Binary min‑heap keyed by non‑negative integer priorities.
 *      • Generic payload (void*) so caller can store Vertex* or any struct.
 *      • O(log n)  insert, extract‑min, and decrease‑key.
 *      • Dynamic growth; initial capacity given by caller.
 *      • Caller tracks index returned by heap_push() to use heap_decrease().
 *        (Simpler than embedding a hash‑map).
 *
 *  API summary
 *      Heap *heap_create(size_t cap);
 *      void  heap_destroy(Heap*);
 *      size_t heap_push(Heap*, void *item, int key);   // returns index or SIZE_MAX
 *      bool   heap_is_empty(const Heap*);
 *      void  *heap_extract_min(Heap*, int *out_key);   // returns payload
 *      bool   heap_decrease_key(Heap*, size_t idx, int new_key);
 * ==========================================================================*/

#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Heap Heap;

Heap  *heap_create(size_t init_cap);
void    heap_destroy(Heap *h);

/**
 * Insert @p item with integer priority @p key.
 * @return index position inside the heap (for later decrease‑key), or SIZE_MAX
 *         if allocation fails.
 */
size_t  heap_push(Heap *h, void *item, int key);

bool    heap_is_empty(const Heap *h);

/**
 * Remove and return the payload with smallest key.  Stores the key in *out_key
 * if out_key != NULL.  Returns NULL when heap empty.
 */
void   *heap_extract_min(Heap *h, int *out_key);

/**
 * Decrease the key of the element currently at @p idx to @p new_key (must be
 * smaller).  Returns false on invalid idx or key increase.
 */
bool    heap_decrease_key(Heap *h, size_t idx, int new_key);

#ifdef __cplusplus
}
#endif

#endif /* HEAP_H */
