/* =======================================================================
 *  test_heap.c  –  Unit & regression tests for heap.[ch]
 *  -----------------------------------------------------------------------
 *  Compile (from project root):
 *
 *      gcc -Wall -Wextra -pedantic -std=c11 \
 *          test/test_heap.c \
 *          src/data_structures/heap/heap.c \
 *          -o test_heap
 *
 *  Run:
 *      ./test_heap
 *
 *  The program prints a short PASSED / FAILED summary and returns
 *  EXIT_SUCCESS on full pass, EXIT_FAILURE otherwise.
 * =======================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "heap.h"
#include <stdint.h>

/* ---------- helpers ---------- */
static void fail(const char *msg) {
    fprintf(stderr, "❌  %s\n", msg);
    exit(EXIT_FAILURE);
}
#define REQUIRE(expr)  do { if(!(expr)) fail(#expr); } while(0)

/* ---------- test cases ---------- */
static void test_basic_push_extract(void)
{
    Heap *h = heap_create(0);
    REQUIRE(h);

    /* push keys 9..0, extract ascending 0..9 */
    for (int k = 9; k >= 0; --k)
        REQUIRE(heap_push(h, (void*)(long)k, k) != SIZE_MAX);

    for (int expect = 0; expect < 10; ++expect) {
        int key;
        int val = (int)(long)heap_extract_min(h, &key);
        REQUIRE(key == expect && val == expect);
    }
    REQUIRE(heap_is_empty(h));
    heap_destroy(h);
}

static void test_decrease_key_stability(void)
{
    const int N = 1000;
    Heap *h = heap_create(16);
    REQUIRE(h);

    size_t idx[N];
    for (int i = 0; i < N; ++i)
        idx[i] = heap_push(h, (void*)(long)i, i + 1000);

    /* decrease last element’s key to - becomes new min */
    REQUIRE(heap_decrease_key(h, idx[N-1], 0));

    int key;
    int val = (int)(long)heap_extract_min(h, &key);
    REQUIRE(key == 0 && val == N-1);

    heap_destroy(h);
}

static void test_random_sequence(void)
{
    const int N = 50000;
    Heap *h = heap_create(32);
    REQUIRE(h);

    int *keys = malloc(N * sizeof(int));
    REQUIRE(keys);

    srand((unsigned)time(NULL));
    for (int i = 0; i < N; ++i) {
        keys[i] = rand() & 0x7FFFFFFF;
        REQUIRE(heap_push(h, NULL, keys[i]) != SIZE_MAX);
    }

    /* extract-min must now yield non-decreasing sequence */
    int prev = -1;
    for (int i = 0; i < N; ++i) {
        int key;
        REQUIRE(heap_extract_min(h, &key) != NULL);
        REQUIRE(key >= prev);
        prev = key;
    }
    REQUIRE(heap_is_empty(h));

    free(keys);
    heap_destroy(h);
}

static void test_negative_key_rejected(void)
{
    Heap *h = heap_create(4);
    REQUIRE(h);
    REQUIRE(heap_push(h, NULL, -5) == SIZE_MAX);
    heap_destroy(h);
}

/* ---------- driver ---------- */
int main(void)
{
    printf("Running heap unit tests…\n");

    test_basic_push_extract();
    test_decrease_key_stability();
    test_random_sequence();
    test_negative_key_rejected();

    printf("✅  All heap tests PASSED\n");
    return EXIT_SUCCESS;
}
