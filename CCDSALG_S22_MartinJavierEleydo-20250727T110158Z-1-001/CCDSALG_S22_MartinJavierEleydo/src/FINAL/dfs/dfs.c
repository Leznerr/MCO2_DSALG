/* ============================================================================
 * FILE: dfs.c   –   Command 6: Depth-First Search traversal (iterative)
 * ----------------------------------------------------------------------------
 * Prints one line of vertex names, space-separated, in **lexicographic order
 * of first discovery** starting from <start>.  Adheres to MCO-2 spec:
 * ▸ Minimalist I/O – no trailing space, newline terminated.
 * ▸ Uses the group’s Stack module (iterative, avoids recursion depth).
 * ▸ Time  O(V + E) ,   extra space O(V).
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "stack.h"   // Step 0: Use stack module for iterative traversal.
#include "graph.h"   // Step 0: Opaque Graph type.
#include "dfs.h"     // Step 0: Public declaration.

/*
 * --- Shadow Struct Definitions ---
 * ---------------------------------
 * These struct definitions are duplicated from the private implementation in graph.c
 * to allow DFS module direct access to internal members for efficient traversal.
 * KEEP IN SYNC with graph.c to preserve correctness and information hiding.
 */
typedef struct Vertex Vertex;

typedef struct AdjNode {
    Vertex          *dst;
    int             weight;
    struct AdjNode  *next;
} AdjNode;

struct Vertex {
    char            name[257];
    AdjNode         *adj;
    Vertex          *next;
};

struct Graph {
    Vertex *v_head;
    size_t  v_count;
    size_t  e_count;
};

/*
 * FUNCTION: build_index_map
 * -------------------------
 * Builds a flat array of vertex pointers from the graph's linked list
 * for O(1) access by integer index, enabling an efficient visited array.
 *
 * Parameters:
 * - g: the graph
 * - out: pointer to array of Vertex pointers (output)
 * Returns: number of vertices in the graph
 */
static size_t build_index_map(const Graph *g, const Vertex ***out)
{
    size_t n = g->v_count;
    Vertex **vec = malloc(n * sizeof(Vertex*));
    if (!vec) { *out = NULL; return 0; }
    size_t i = 0; for (Vertex *v = g->v_head; v; v = v->next) vec[i++] = v;
    *out = (const Vertex **)vec;
    return n;
}

/*
 * FUNCTION: index_of
 * ------------------
 * Performs a binary search to find the index of a vertex by name in a sorted map.
 * O(log V) lookup.
 *
 * Parameters:
 * - vec: sorted array of vertex pointers
 * - n: number of vertices
 * - name: name to search for
 * Returns: integer index if found, otherwise -1
 */
static ptrdiff_t index_of(const Vertex **vec, size_t n, const char *name)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        int cmp = strcmp(vec[mid]->name, name);
        if (cmp == 0) return (ptrdiff_t)mid;
        (cmp < 0) ? (lo = mid + 1) : (hi = mid);
    }
    return -1;
}

/*
 * FUNCTION: cmd_dfs
 * -----------------
 * Performs an iterative Depth-First Search traversal.
 * Traverses the graph from a starting vertex, exploring as far as possible
 * along each branch before backtracking. Prints each vertex upon first discovery.
 *
 * Parameters:
 * - g: pointer to the Graph structure
 * - start: name of the starting vertex
 * - scratch: pre-allocated Stack for traversal
 */
void cmd_dfs(Graph *g, const char *start, Stack *scratch)
{
    // Step 1: Validate input parameters.
    if (!g || !start) { putchar('\n'); return; }

    // Step 2: Build lookup structures for traversal.
    // Build a temporary, sorted array of vertex pointers for fast index-based lookup.
    const Vertex **vec = NULL;
    size_t V = build_index_map(g, &vec);
    if (V == 0) { putchar('\n'); return; }

    // Allocate a boolean array (bitmap) for O(1) 'visited' checks.
    bool *visited = calloc(V, sizeof(bool));
    if (!visited) { free((void*)vec); putchar('\n'); return; }

    // Find integer index of the starting vertex.
    ptrdiff_t s_idx = index_of(vec, V, start);
    if (s_idx < 0) {
        free((void*)vec); free(visited); putchar('\n'); return;
    }

    // Step 3: Initialize stack with the starting vertex.
    while (!stack_is_empty(scratch)) (void)stack_pop(scratch);
    stack_push(scratch, (void*)vec[s_idx]);

    // Step 4: Main traversal loop.
    while (!stack_is_empty(scratch)) {
        Vertex *u = (Vertex *)stack_pop(scratch);
        ptrdiff_t u_idx = index_of(vec, V, u->name);

        // Skip already visited vertices.
        if (visited[u_idx]) continue;
        visited[u_idx] = true;

        // Print newly visited vertex's name.
        fputs(u->name, stdout);
        putchar('\n');

        // Step 5: Process all neighbors of the current vertex.
        // Copy neighbor pointers into a temporary buffer.
        size_t deg = 0; for (AdjNode *a = u->adj; a; a = a->next) ++deg;
        if (deg == 0) continue;
        Vertex **buf = malloc(deg * sizeof(Vertex*));
        if (!buf) break;
        size_t i = 0; for (AdjNode *a = u->adj; a; a = a->next) buf[i++] = a->dst;

        // Push unvisited neighbors onto the stack in REVERSE lex order,
        // so they're popped and visited in correct order.
        while (i--) {
            ptrdiff_t v_idx = index_of(vec, V, buf[i]->name);
            if (!visited[v_idx]) {
                stack_push(scratch, buf[i]);
            }
        }
        free(buf);
    }

    // Step 6: Print a final newline for output formatting.
    putchar('\n');

    // Step 7: Cleanup. Free all memory allocated for this traversal.
    free((void*)vec);
    free(visited);
}
