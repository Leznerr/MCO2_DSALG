/* ===========================================================================  
 *  graph.h — Public interface for the CCDSALG MCO-2 Graph module  
 *  ---------------------------------------------------------------------------  
 *  Opaque struct-based graph with API support for:  
 *    Cmd 1  graph_add_vertex  
 *    Cmd 2  graph_add_edge      (undirected, weighted)  
 *    Cmd 3  graph_get_degree  
 *    Cmd 4  graph_edge_exists  
 *    Cmd 10 graph_print  
 *  
 *  Notes:  
 *  ▸ Uses opaque struct to encapsulate internal details (information hiding)  
 *  ▸ Minimalist I/O: functions either return values or print spec-required output only  
 * =========================================================================== */

#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTICES 100     // Maximum number of vertices supported
#define MAX_NAME_LEN 256     // Maximum allowed length for vertex names

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>  // for size_t

/* ────────────────────────────────────────────────────────────────────────── */
/*  OPAQUE GRAPH TYPE                                                         */
/* ────────────────────────────────────────────────────────────────────────── */

typedef struct Graph Graph;  // Definition is hidden in graph.c


/* ────────────────────────────────────────────────────────────────────────── */
/*  LIFECYCLE FUNCTIONS                                                       */
/* ────────────────────────────────────────────────────────────────────────── */

// Allocate and return a new empty graph
Graph *graph_create(void);

// Free all memory associated with the graph (safe to call on NULL)
void graph_destroy(Graph *g);


/* ────────────────────────────────────────────────────────────────────────── */
/*  INSERTION COMMANDS (Cmd 1 & 2)                                            */
/* ────────────────────────────────────────────────────────────────────────── */

// Add a vertex with name (1–256 chars, alphanumeric + underscore)
// Returns false if name is invalid, duplicate, or OOM
bool graph_add_vertex(Graph *g, const char *name);

// Add/update undirected edge (u, v) with weight in [1, 100]
// Returns false if vertices are missing or weight invalid
bool graph_add_edge(Graph *g, const char *u_name, const char *v_name, int weight);


/* ────────────────────────────────────────────────────────────────────────── */
/*  QUERY COMMANDS (Cmd 3 & 4)                                                */
/* ────────────────────────────────────────────────────────────────────────── */

// Get degree of vertex (returns -1 if vertex not found)
int graph_get_degree(Graph *g, const char *name);

// Check if edge (u, v) exists (returns true if connected)
bool graph_edge_exists(Graph *g, const char *u_name, const char *v_name);

// Cmd 3: Print degree of vertex (or nothing if invalid)
void get_degree(Graph *g, const char *name);

// Cmd 4: Print "1" if edge exists, "0" otherwise
void edge_check(Graph *g, const char *u_name, const char *v_name);


/* ────────────────────────────────────────────────────────────────────────── */
/*  UTILITY HELPERS                                                           */
/* ────────────────────────────────────────────────────────────────────────── */

// Get neighbors of a vertex (fills array of names, returns count)
size_t graph_get_neighbors(const Graph *g, const char *name, char neighbors[][MAX_NAME_LEN]);

// Check if a vertex exists in the graph
bool graph_vertex_exists(const Graph *g, const char *name);

// Fill array with all vertex names, return total vertex count
int graph_get_all_vertices(Graph *g, char names[][MAX_NAME_LEN]);

// Return weight of edge between u and v; -1 if no connection
int graph_get_edge_weight(Graph *g, const char *u, const char *v);


/* ────────────────────────────────────────────────────────────────────────── */
/*  OUTPUT (Cmd 10)                                                           */
/* ────────────────────────────────────────────────────────────────────────── */

// Print graph in the format:
// Label = (V,E)
// V = {v1, v2, ...}
// E = {
// (u1, v1, w1),
// (u2, v2, w2)
// }
void graph_print(const Graph *g, const char *label);


/* ────────────────────────────────────────────────────────────────────────── */
#ifdef __cplusplus
}
#endif

#endif /* GRAPH_H */
