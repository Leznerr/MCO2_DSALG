#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "heap.h"

int main(void) {
    Heap *h = heap_create(4);
    if (!h) {
        fprintf(stderr, "create failed\n");
        return 1;
    }
    if (heap_push(h, NULL, -1) != SIZE_MAX) {
        fprintf(stderr, "negative key accepted\n");
        return 1;
    }
    heap_destroy(h);
    printf("neg-key test passed\n");
    return 0;
}
