/* ============================================================================
 *  heap.c – Binary Min‑Heap implementation for CCDSALG MCO‑2
 *  ----------------------------------------------------------------------------
 *  Generic void* payload, integer priorities (keys ≥ 0).  Supports:
 *      • heap_push          O(log n)
 *      • heap_extract_min   O(log n)
 *      • heap_decrease_key  O(log n)
 *  Caller keeps the index returned by heap_push() so we avoid an internal
 *  hash map and stay within course scope.
 * ==========================================================================*/

#include "heap.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>   /* SIZE_MAX */

struct Heap {
    size_t  size;
    size_t  cap;
    int    *key;    /* parallel arrays */
    void  **data;
};

/* ───────────────── helpers ───────────────── */
static void swap(Heap *h, size_t i, size_t j)
{
    int   k_tmp   = h->key[i];
    void *d_tmp   = h->data[i];
    h->key[i]   = h->key[j];
    h->data[i]  = h->data[j];
    h->key[j]   = k_tmp;
    h->data[j]  = d_tmp;
}

static void sift_up(Heap *h, size_t i)
{
    while (i && h->key[(i-1)/2] > h->key[i]) {
        size_t p = (i-1)/2;
        swap(h, i, p);
        i = p;
    }
}

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

/* ───────────────── API ──────────────────── */
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

void heap_destroy(Heap *h)
{
    if (!h) return;
    free(h->key);
    free(h->data);
    free(h);
}

static bool grow(Heap *h)
{
    size_t new_cap = h->cap * 2;
    int   *k_new = realloc(h->key,  new_cap * sizeof(int));
    void **d_new = realloc(h->data, new_cap * sizeof(void*));
    if (!k_new || !d_new) return false;
    h->key = k_new; h->data = d_new; h->cap = new_cap;
    return true;
}

size_t heap_push(Heap *h, void *item, int key)
{
    if (!h) return SIZE_MAX;
    if (key < 0) return SIZE_MAX;  /* enforce non-negative priorities */
    if (h->size == h->cap && !grow(h)) return SIZE_MAX;
    size_t idx = h->size++;
    h->key[idx]  = key;
    h->data[idx] = item;
    sift_up(h, idx);
    return idx;
}

bool heap_is_empty(const Heap *h) { return !h || h->size == 0; }

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

bool heap_decrease_key(Heap *h, size_t idx, int new_key)
{
    if (!h || idx >= h->size || new_key >= h->key[idx]) return false;
    h->key[idx] = new_key;
    sift_up(h, idx);
    return true;
}
