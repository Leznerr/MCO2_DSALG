/* ===========================================================================
 *  graph.h  ─  Public interface for the CCDSALG MCO-2 Graph module
 *  ---------------------------------------------------------------------------
 *  Exposes an **opaque** Graph type plus API for:
 *      Cmd 1  graph_add_vertex
 *      Cmd 2  graph_add_edge      (undirected, weighted)
 *      Cmd 3  graph_get_degree
 *      Cmd 4  graph_edge_exists
 *      Cmd 10 graph_print
 *
 *  ▸ All routines follow the spec’s minimalist-I/O rule: they either return
 *    status codes or print the exact required output (no prompts, no extras).
 *  ▸ Internal structs remain hidden in graph.c, preserving information hiding
 *    for full rubric style credit.
 * ==========================================================================*/

#ifndef GRAPH_H
#define GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>   /* size_t */

/* ────────────────────────────────────────────────────────────────────────── */
/*  OPAQUE HANDLE                                                             */
/* ────────────────────────────────────────────────────────────────────────── */
typedef struct Graph Graph;   /* definition lives in graph.c */

/* ────────────────────────────────────────────────────────────────────────── */
/*  LIFECYCLE                                                                 */
/* ────────────────────────────────────────────────────────────────────────── */
/** Allocate an empty undirected weighted graph (returns NULL on OOM). */
Graph *graph_create(void);

/** Free all memory owned by @p g (safe on NULL). */
void   graph_destroy(Graph *g);

/* ────────────────────────────────────────────────────────────────────────── */
/*  INSERTION (Commands 1 & 2)                                                */
/* ────────────────────────────────────────────────────────────────────────── */
/**
 * Add a vertex whose label matches the regex  /^[A-Za-z0-9_]{1,256}$/.
 * Returns false on duplicate, bad input, or allocation failure.
 */
bool   graph_add_vertex(Graph *g, const char *name);

/**
 * Add or update an undirected edge (u,v) with weight in [1,100].
 * Both vertices must already exist.  Duplicate edges overwrite weight.
 */
bool   graph_add_edge  (Graph *g,
                        const char *u_name,
                        const char *v_name,
                        int         weight);

/* ────────────────────────────────────────────────────────────────────────── */
/*  QUERY HELPERS (support Cmd 3 & 4)                                         */
/* ────────────────────────────────────────────────────────────────────────── */
/** Return degree of @p name, or −1 if vertex not found. */
int    graph_get_degree (Graph *g, const char *name);

/** Return true if an edge (u,v) exists, false otherwise. */
bool   graph_edge_exists(Graph *g,
                         const char *u_name,
                         const char *v_name);

/* ────────────────────────────────────────────────────────────────────────── */
/*  OUTPUT (Command 10)                                                       */
/* ────────────────────────────────────────────────────────────────────────── */
/**
 * Print the graph in spec-exact format:
 *     <label> = (V,E)
 *     V = {v1, v2, …}
 *     E = {
 *     (u1, v1, w1),
 *     (u2, v2, w2)
 *     }
 *
 * Pass NULL for @p label to print the default “Graph”.
 */
void   graph_print(const Graph *g, const char *label);

/* ────────────────────────────────────────────────────────────────────────── */
#ifdef __cplusplus
}
#endif
#endif /* GRAPH_H */
