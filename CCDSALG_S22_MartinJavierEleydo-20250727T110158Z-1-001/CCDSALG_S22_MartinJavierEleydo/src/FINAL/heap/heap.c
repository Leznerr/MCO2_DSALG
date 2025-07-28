/* ============================================================================
 *  heap.c – Binary Min‑Heap implementation for CCDSALG MCO‑2
 * ----------------------------------------------------------------------------
 *  Implements a binary min-heap (priority queue) that:
 *    • Stores generic (void*) user data with integer keys ≥ 0 (priorities)
 *    • Supports three efficient operations, all O(log n):
 *         – heap_push          : Insert item with a key; returns index for tracking
 *         – heap_extract_min   : Remove and return item with smallest key
 *         – heap_decrease_key  : Decrease key of an item, given its index
 *
 *  Notes:
 *    - No internal hash map: caller keeps the index from heap_push() if decrease-key is needed.
 *    - Used for graph algorithms (e.g., Dijkstra, Prim), event simulation, and anywhere
 *      a fast min-priority queue is required.
 *    - Heap is implemented as parallel arrays for keys and data, with resizing.
 *    - The min-heap property ensures the smallest key is always at the root (index 0).
 * ============================================================================
 */

#include "heap.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>   /* SIZE_MAX */

/* ============================================================================
 * Heap Structure
 * ----------------------------------------------------------------------------
 * Maintains three main fields:
 *   - size:  Current number of elements stored
 *   - cap:   Current array capacity (auto-doubles as needed)
 *   - key:   Array of integer keys (priorities)
 *   - data:  Array of pointers to user data (void*)
 *
 * All heap operations (push, extract, decrease) work by manipulating
 * these arrays to preserve the min-heap property: for every node i,
 * key[i] ≤ key[left(i)], key[i] ≤ key[right(i)]
 * ============================================================================
 */
struct Heap {
    size_t  size;   // Number of elements currently in the heap
    size_t  cap;    // Total capacity of the heap arrays
    int    *key;    // Array of priority keys (integers)
    void  **data;   // Array of user data pointers (void*)
};

/* ============================================================================
 * swap
 * ----------------------------------------------------------------------------
 * Exchanges two elements (both key and data) in the heap arrays.
 * Used internally by sift_up and sift_down during reordering.
 * ============================================================================
 */
static void swap(Heap *h, size_t i, size_t j)
{
    int   k_tmp   = h->key[i];
    void *d_tmp   = h->data[i];
    h->key[i]   = h->key[j];
    h->data[i]  = h->data[j];
    h->key[j]   = k_tmp;
    h->data[j]  = d_tmp;
}

/* ============================================================================
 * sift_up
 * ----------------------------------------------------------------------------
 * Restores the min-heap property by moving a node up the tree
 * if its key is less than its parent's key. Used after insert (push) and decrease-key.
 * Runs in O(log n) time.
 * ============================================================================
 */
static void sift_up(Heap *h, size_t i)
{
    while (i && h->key[(i-1)/2] > h->key[i]) {
        size_t p = (i-1)/2;
        swap(h, i, p);
        i = p;
    }
}

/* ============================================================================
 * sift_down
 * ----------------------------------------------------------------------------
 * Restores the min-heap property by moving a node down the tree
 * if its key is greater than either child. Used after extract-min.
 * Runs in O(log n) time.
 * ============================================================================
 */
static void sift_down(Heap *h, size_t i)
{
    while (1) {
        size_t l = 2*i + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < h->size && h->key[l] < h->key[smallest]) smallest = l;
        if (r < h->size && h->key[r] < h->key[smallest]) smallest = r;
        if (smallest == i) break;
        swap(h, i, smallest);
        i = smallest;
    }
}

/* ============================================================================
 * heap_create
 * ----------------------------------------------------------------------------
 * Allocates a new heap structure with the given initial capacity.
 * If init_cap is zero, defaults to 16. Returns NULL on allocation failure.
 * ============================================================================
 */
Heap *heap_create(size_t init_cap)
{
    if (init_cap == 0) init_cap = 16;
    Heap *h = malloc(sizeof(Heap));
    if (!h) return NULL;
    h->size = 0;
    h->cap  = init_cap;
    h->key  = malloc(init_cap * sizeof(int));
    h->data = malloc(init_cap * sizeof(void*));
    if (!h->key || !h->data) { free(h->key); free(h->data); free(h); return NULL; }
    return h;
}

/* ============================================================================
 * heap_destroy
 * ----------------------------------------------------------------------------
 * Frees all memory associated with the heap. Safe to call on NULL.
 * Does NOT free the user's data (void* values), as those are owned externally.
 * ============================================================================
 */
void heap_destroy(Heap *h)
{
    if (!h) return;
    free(h->key);
    free(h->data);
    free(h);
}

/* ============================================================================
 * grow
 * ----------------------------------------------------------------------------
 * Internal helper that doubles the heap's capacity.
 * If reallocation fails, returns false and the heap remains unchanged.
 * ============================================================================
 */
static bool grow(Heap *h)
{
    size_t new_cap = h->cap * 2;
    int   *k_new = realloc(h->key,  new_cap * sizeof(int));
    void **d_new = realloc(h->data, new_cap * sizeof(void*));
    if (!k_new || !d_new) return false;
    h->key = k_new; h->data = d_new; h->cap = new_cap;
    return true;
}

/* ============================================================================
 * heap_push
 * ----------------------------------------------------------------------------
 * Inserts a new item with the given priority key.
 * Automatically resizes if full. Returns the array index at which the item
 * was inserted (for use with decrease-key), or SIZE_MAX if an error occurs.
 * O(log n)
 * ============================================================================
 */
size_t heap_push(Heap *h, void *item, int key)
{
    if (!h) return SIZE_MAX;
    if (h->size == h->cap && !grow(h)) return SIZE_MAX;
    size_t idx = h->size++;
    h->key[idx]  = key;
    h->data[idx] = item;
    sift_up(h, idx);
    return idx;
}

/* ============================================================================
 * heap_is_empty
 * ----------------------------------------------------------------------------
 * Returns true if the heap is empty or NULL, false otherwise.
 * O(1)
 * ============================================================================
 */
bool heap_is_empty(const Heap *h) { return !h || h->size == 0; }

/* ============================================================================
 * heap_extract_min
 * ----------------------------------------------------------------------------
 * Removes and returns the user data pointer with the minimum key (priority).
 * Stores the key in *out_key if out_key is not NULL.
 * If the heap is empty or NULL, returns NULL.
 * O(log n)
 * ============================================================================
 */
void *heap_extract_min(Heap *h, int *out_key)
{
    if (!h || h->size == 0) return NULL;
    if (out_key) *out_key = h->key[0];
    void *out = h->data[0];
    h->size--;
    if (h->size) {
        h->key[0]  = h->key[h->size];
        h->data[0] = h->data[h->size];
        sift_down(h, 0);
    }
    return out;
}

/* ============================================================================
 * heap_decrease_key
 * ----------------------------------------------------------------------------
 * Decreases the key (priority) of the item at the specified index to new_key,
 * as long as new_key < current key. Restores heap property as needed.
 * Returns true if successful; false if index is out of bounds, new_key is not lower,
 * or heap is NULL.
 * O(log n)
 * ============================================================================
 */
bool heap_decrease_key(Heap *h, size_t idx, int new_key)
{
    if (!h || idx >= h->size || new_key >= h->key[idx]) return false;
    h->key[idx] = new_key;
    sift_up(h, idx);
    return true;
}
