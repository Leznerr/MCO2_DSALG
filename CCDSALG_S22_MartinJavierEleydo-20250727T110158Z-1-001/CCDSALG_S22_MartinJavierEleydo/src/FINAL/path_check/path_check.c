/* ============================================================================
 *  path_check.c  –  Command 7: Connectivity / Path-Check
 * ----------------------------------------------------------------------------
 *  Checks if an undirected path exists between two vertices in the graph.
 *  Prints "1\n" if a path exists from <src> to <dst>, else prints "0\n".
 *  Returns the same result as a boolean for potential use in other modules.
 *
 *  Complexity: O(V + E) time,  O(V) extra space (DFS via stack).
 * ----------------------------------------------------------------------------
 *  Design Notes:
 *    - Uses iterative Depth-First Search (DFS) to avoid stack overflow and
 *      support large graphs safely.
 *    - Builds a temporary array for efficient index-based lookup and
 *      visited marking.
 *    - Ensures lexicographic neighbor traversal for consistency.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "stack.h"           /* P1 Stack module */
#include "graph.h"           /* Public Graph API */
#include "path_check.h"      /* This module’s public declaration */

/* ============================================================================
 *  INTERNAL STRUCTURE DEFS (Shadow graph.c)
 * ----------------------------------------------------------------------------
 *  These match the private graph.c structures so we can efficiently traverse.
 *  Only safe if kept in exact sync with the main graph.c implementation.
 * ============================================================================
 */
typedef struct Vertex Vertex;

typedef struct AdjNode {
    Vertex           *dst;
    int               weight;
    struct AdjNode   *next;
} AdjNode;

struct Vertex {
    char            name[257];    /* MAX_NAME_LEN + 1 */
    AdjNode        *adj;          /* Head of adjacency list (sorted) */
    Vertex         *next;         /* Next vertex in global list      */
};

struct Graph {
    Vertex *v_head;   // Global head pointer to vertex list
    size_t  v_count;  // Number of vertices
    size_t  e_count;  // Logical (undirected) edge count
};

/* ============================================================================
 *  HELPER: build_index_map
 * ----------------------------------------------------------------------------
 *  Builds a flat array of pointers to all vertices in the graph (in lex order).
 *  This enables O(1) index-based access for 'visited' lookups.
 *  Returns the number of vertices, or 0 on allocation failure.
 * ============================================================================
 */
static size_t build_index_map(const Graph *g, const Vertex ***out_vec)
{
    size_t n = g->v_count;
    Vertex **vec = malloc(n * sizeof(Vertex *));
    if (!vec) { *out_vec = NULL; return 0; }
    size_t i = 0;
    for (Vertex *v = g->v_head; v; v = v->next) vec[i++] = v;
    *out_vec = (const Vertex **)vec;
    return n;
}

/* ============================================================================
 *  HELPER: index_of
 * ----------------------------------------------------------------------------
 *  Finds the index of a vertex by name within the (sorted) array of vertex pointers.
 *  Uses binary search for O(log V) time.
 *  Returns the index if found, or -1 if not found.
 * ============================================================================
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

/* ============================================================================
 *  PUBLIC: cmd_path (Command 7 handler)
 * ----------------------------------------------------------------------------
 *  Checks if an undirected path exists from src to dst using iterative DFS.
 *  - Uses a stack for traversal (no recursion).
 *  - Tracks visited vertices via index bitmap for O(1) lookups.
 *  - Pushes neighbors in reverse lex order so discovery order matches spec.
 *
 *  Returns true and prints "1" if a path exists; else prints "0" and returns false.
 * ============================================================================
 */
bool cmd_path(Graph *g, const char *src, const char *dst, Stack *scratch)
{
    // --- Sanity checks: null graph or names mean no path ---
    if (!g || !src || !dst) { puts("0"); return false; }

    // --- Trivial case: src and dst are the same vertex name ---
    if (strcmp(src, dst) == 0) {
        for (Vertex *v = g->v_head; v; v = v->next)
            if (strcmp(v->name, src) == 0) { puts("1"); return true; }
        puts("0");
        return false;
    }

    // --- Step 1: Build the vertex index map and 'visited' bitmap ---
    const Vertex **vec = NULL;
    size_t V = build_index_map(g, &vec);
    if (V == 0) { puts("0"); return false; }
    bool *visited = calloc(V, sizeof(bool));
    if (!visited) { free((void *)vec); puts("0"); return false; }

    ptrdiff_t s_idx = index_of(vec, V, src);
    ptrdiff_t t_idx = index_of(vec, V, dst);
    if (s_idx < 0 || t_idx < 0) {
        free((void *)vec); free(visited); puts("0"); return false;
    }

    // --- Step 2: Prepare the stack (clear, then push start vertex) ---
    while (!stack_is_empty(scratch)) (void)stack_pop(scratch);
    stack_push(scratch, (void *)vec[s_idx]);

    bool found = false;
    while (!stack_is_empty(scratch) && !found) {
        Vertex *u = (Vertex *)stack_pop(scratch);
        ptrdiff_t u_idx = index_of(vec, V, u->name);
        if (visited[u_idx]) continue;  // Already explored
        visited[u_idx] = true;
        if (u_idx == t_idx) { found = true; break; }

        // --- Step 3: Push all neighbors (reverse lex order for spec) ---
        size_t deg = 0; for (AdjNode *a = u->adj; a; a = a->next) ++deg;
        Vertex **buf = malloc(deg * sizeof(Vertex *));
        if (!buf) break; // Out of memory: fail gracefully
        size_t i = 0; for (AdjNode *a = u->adj; a; a = a->next) buf[i++] = a->dst;
        while (i--) {
            ptrdiff_t v_idx = index_of(vec, V, buf[i]->name);
            if (!visited[v_idx]) stack_push(scratch, buf[i]);
        }
        free(buf);
    }

    // --- Step 4: Output and clean up ---
    puts(found ? "1" : "0");
    free((void *)vec);
    free(visited);
    return found;
}
