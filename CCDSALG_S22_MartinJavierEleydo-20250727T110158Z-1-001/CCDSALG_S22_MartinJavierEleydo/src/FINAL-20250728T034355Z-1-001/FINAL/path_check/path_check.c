/* ============================================================================
 *  path_check.c  –  Command 7: Connectivity / Path-Check
 *  ----------------------------------------------------------------------------
 *  Prints "1\n" if an undirected path exists between <src> and <dst>, else
 *  "0\n".  Returns the same boolean for callers.
 *  Complexity: O(V + E) time,  O(V) extra space.
 * ==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "stack.h"           /* P1 Stack module                          */
#include "graph.h"           /* Public Graph API                         */
#include "path_check.h"      /* This module’s public declaration         */

/* ─── Shadow graph.c internals (must match layout exactly) ──────────────── */
typedef struct Vertex Vertex;

typedef struct AdjNode {
    Vertex           *dst;
    int               weight;
    struct AdjNode   *next;
} AdjNode;

struct Vertex {
    char            name[257];    /* MAX_NAME_LEN + 1                     */
    AdjNode        *adj;          /* head of adjacency list (sorted)      */
    Vertex         *next;         /* next vertex in global list           */
};

struct Graph {
    Vertex *v_head;
    size_t  v_count;
    size_t  e_count;
};

/* ─── Helper: build Vertex* → index array (lexicographic) ───────────────── */
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

/* ─── Public Command 7 handler ──────────────────────────────────────────── */
bool cmd_path(Graph *g, const char *src, const char *dst, Stack *scratch)
{
    if (!g || !src || !dst) { puts("0"); return false; }

    /* Trivial src == dst check */
    if (strcmp(src, dst) == 0) {
        for (Vertex *v = g->v_head; v; v = v->next)
            if (strcmp(v->name, src) == 0) { puts("1"); return true; }
        puts("0");
        return false;
    }

    /* Build index map + visited bitmap */
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

    /* Clear any residual items in scratch stack */
    while (!stack_is_empty(scratch)) (void)stack_pop(scratch);
    stack_push(scratch, (void *)vec[s_idx]);

    bool found = false;
    while (!stack_is_empty(scratch) && !found) {
        Vertex *u = (Vertex *)stack_pop(scratch);
        ptrdiff_t u_idx = index_of(vec, V, u->name);
        if (visited[u_idx]) continue;
        visited[u_idx] = true;
        if (u_idx == t_idx) { found = true; break; }

        /* Push neighbours in reverse lexicographic order */
        size_t deg = 0; for (AdjNode *a = u->adj; a; a = a->next) ++deg;
        Vertex **buf = malloc(deg * sizeof(Vertex *));
        if (!buf) break;                       /* graceful out-of-mem fallback */
        size_t i = 0; for (AdjNode *a = u->adj; a; a = a->next) buf[i++] = a->dst;
        while (i--) {
            ptrdiff_t v_idx = index_of(vec, V, buf[i]->name);
            if (!visited[v_idx]) stack_push(scratch, buf[i]);
        }
        free(buf);
    }

    puts(found ? "1" : "0");
    free((void *)vec);
    free(visited);
    return found;
}
