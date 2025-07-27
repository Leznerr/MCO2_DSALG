/* ============================================================================
 *  bfs.c  –  Command 5: Breadth-First Search traversal (iterative)
 *  ----------------------------------------------------------------------------
 *  Prints one line of vertex names, space-separated, in **lexicographic order
 *  of first discovery** starting from <start>.  Adheres to MCO-2 spec:
 *      ? Minimalist I/O – no trailing space, newline terminated.
 *      ? Uses the group's Queue module (iterative, proper BFS behavior).
 *      ? Time  O(V + E) ,  extra space O(V).
 *  ==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "queue.h"   /* P1 queue */
#include "graph.h"   /* opaque Graph type */
#include "bfs.h"     /* public declaration */

/* -- Shadow graph.c internals (must match layout exactly) ---------------- */
typedef struct Vertex Vertex;

typedef struct AdjNode {
    Vertex           *dst;
    int               weight;
    struct AdjNode   *next;
} AdjNode;

struct Vertex {
    char            name[257];
    AdjNode        *adj;
    Vertex         *next;
};

struct Graph {
    Vertex *v_head;
    size_t  v_count;
    size_t  e_count;
};

/* -- Helpers: build index map + binary search over vertex names ----------- */
static size_t build_index_map(const Graph *g, const Vertex ***out)
{
    size_t n = g->v_count;
    Vertex **vec = malloc(n * sizeof(Vertex*));
    if (!vec) { *out = NULL; return 0; }
    size_t i = 0; for (Vertex *v = g->v_head; v; v = v->next) vec[i++] = v;
    *out = (const Vertex **)vec;
    return n;
}

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

/* -- Public implementation (Cmd 5) ---------------------------------------- */
void cmd_bfs(Graph *g, const char *start, Queue *scratch)
{
    if (!g || !start) { putchar('\n'); return; }

    /* Build map ? index for visited bitmap */
    const Vertex **vec = NULL;
    size_t V = build_index_map(g, &vec);
    if (V == 0) { putchar('\n'); return; }

    bool *visited = calloc(V, sizeof(bool));
    if (!visited) { free((void*)vec); putchar('\n'); return; }

    ptrdiff_t s_idx = index_of(vec, V, start);
    if (s_idx < 0) { free((void*)vec); free(visited); putchar('\n'); return; }

    /* Ensure scratch queue is empty */
    while (!queue_is_empty(scratch)) (void)queue_dequeue(scratch);
    queue_enqueue(scratch, (void*)vec[s_idx]);
    visited[s_idx] = true;

    bool first_out = true;
    while (!queue_is_empty(scratch)) {
        Vertex *u = (Vertex *)queue_dequeue(scratch);
        
        if (!first_out) putchar(' ');
        first_out = false;
        fputs(u->name, stdout);

        /* Enqueue neighbours in lexicographic order */
        for (AdjNode *a = u->adj; a; a = a->next) {
            ptrdiff_t v_idx = index_of(vec, V, a->dst->name);
            if (!visited[v_idx]) {
                visited[v_idx] = true;
                queue_enqueue(scratch, a->dst);
            }
        }
    }
    putchar('\n');

    free((void*)vec);
    free(visited);
}
