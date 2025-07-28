/* ===========================================================================  
 *  FILE: graph.h — Public interface for the CCDSALG MCO-2 Graph module  
 *  ---------------------------------------------------------------------------  
 *  The core API for a robust, *opaque* (information-hiding) graph structure,
 *  supporting efficient, spec-compliant undirected graph operations.
 *  
 *  Supported commands:
 *    Cmd 1  graph_add_vertex      – Add a named vertex
 *    Cmd 2  graph_add_edge        – Add or update an undirected weighted edge
 *    Cmd 3  graph_get_degree      – Query or print degree of a vertex
 *    Cmd 4  graph_edge_exists     – Query or print edge existence
 *    Cmd 10 graph_print           – Print V and E sets (spec output)
 *
 *  Design Highlights:
 *    ▸ Opaque struct: hides all internal implementation details (no direct access)
 *    ▸ API-only: all manipulation through these declared functions (safe/consistent)
 *    ▸ Minimalist I/O: functions return data or print exactly as spec requires (no extras)
 *    ▸ Lexicographically sorted storage for vertices and neighbors (deterministic order)
 *    ▸ Graceful handling of invalid input or OOM
 *
 *  For use in all MCO-2 graph-related algorithms, traversals, and tests.
 * =========================================================================== */

#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTICES 100     // Maximum number of vertices supported (API-wide)
#define MAX_NAME_LEN 256     // Max allowed length for vertex names (for buffers/validation)

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>  // For size_t

/* ─────────────────────────────────────────────────────────────────────────────
 *  OPAQUE GRAPH TYPE
 *  ---------------------------------------------------------------------------
 *  The actual data structure is hidden (encapsulation). Only a pointer to
 *  'struct Graph' is exposed here, so users can't access or modify internals.
 *  All operations must go through this API — supporting information hiding and
 *  clean abstraction, a key DSAL concept.
 * ───────────────────────────────────────────────────────────────────────────*/
typedef struct Graph Graph;  // Opaque type, actual fields in graph.c

/* ─────────────────────────────────────────────────────────────────────────────
 *  LIFECYCLE FUNCTIONS
 *  ---------------------------------------------------------------------------
 *  Graph creation and destruction.
 *  These allow users to manage the lifetime of the graph structure and ensure
 *  all memory is properly allocated/freed.
 * ───────────────────────────────────────────────────────────────────────────*/
// Create a new, empty graph. Returns a pointer, or NULL if out of memory.
Graph *graph_create(void);

// Free all memory associated with a graph. Safe to call on NULL.
void graph_destroy(Graph *g);

/* ─────────────────────────────────────────────────────────────────────────────
 *  INSERTION COMMANDS (Cmd 1 & 2)
 *  ---------------------------------------------------------------------------
 *  These functions add content to the graph.
 *  - Vertices are identified by unique names (alphanumeric + underscore, up to 256 chars).
 *  - Edges are undirected and can have weights from 1 to 100.
 *  - Duplicate vertices are not added; reinserting an existing edge updates its weight.
 * ───────────────────────────────────────────────────────────────────────────*/
// Add a vertex (name must be unique, valid, and not already in the graph).
// Returns false if name is invalid, duplicate, or OOM.
bool graph_add_vertex(Graph *g, const char *name);

// Add or update an undirected edge (u, v) with a specified weight (1–100).
// Returns false if either vertex is missing, if weight is out of range, or OOM.
bool graph_add_edge(Graph *g, const char *u_name, const char *v_name, int weight);

/* ─────────────────────────────────────────────────────────────────────────────
 *  QUERY COMMANDS (Cmd 3 & 4)
 *  ---------------------------------------------------------------------------
 *  Allow querying the graph for properties or relationships:
 *  - Degree: number of neighbors (edges) for a vertex
 *  - Edge existence: check if two vertices are directly connected
 *  - Printing: print degree or edge presence in required format for MCO-2
 * ───────────────────────────────────────────────────────────────────────────*/
// Get the degree (number of neighbors) of a named vertex; -1 if not found.
int graph_get_degree(Graph *g, const char *name);

// Check if an edge (u, v) exists in the graph (undirected).
bool graph_edge_exists(Graph *g, const char *u_name, const char *v_name);

// Command 3: Print the degree of a vertex (prints nothing if invalid)
void get_degree(Graph *g, const char *name);

// Command 4: Print "1" if an edge exists, "0" if not (spec output)
void edge_check(Graph *g, const char *u_name, const char *v_name);

/* ─────────────────────────────────────────────────────────────────────────────
 *  UTILITY HELPERS
 *  ---------------------------------------------------------------------------
 *  These support traversal, visualization, or advanced queries.
 * ───────────────────────────────────────────────────────────────────────────*/
// Get all neighbors of a vertex. Fills 'neighbors' array with up to MAX_VERTICES
// names (strings, each up to MAX_NAME_LEN). Returns the neighbor count, or -1 if missing.
size_t graph_get_neighbors(const Graph *g, const char *name, char neighbors[][MAX_NAME_LEN]);

// Returns true if the vertex with 'name' exists in the graph, else false.
bool graph_vertex_exists(const Graph *g, const char *name);

// Fills 'names' array with all vertex names (each string up to MAX_NAME_LEN).
// Returns the total number of vertices found.
int graph_get_all_vertices(Graph *g, char names[][MAX_NAME_LEN]);

// Returns the weight of the edge between u and v, or -1 if no such edge exists.
int graph_get_edge_weight(Graph *g, const char *u, const char *v);

/* ─────────────────────────────────────────────────────────────────────────────
 *  OUTPUT (Cmd 10)
 *  ---------------------------------------------------------------------------
 *  Print the entire graph (vertex set and edge set) in spec-compliant format:
 *    Label = (V,E)
 *    V = {v1, v2, ...}
 *    E = {
 *      (u1, v1, w1),
 *      (u2, v2, w2)
 *    }
 *  Output is deterministic (sorted), with no extra whitespace or prompts.
 * ───────────────────────────────────────────────────────────────────────────*/
// Print the entire graph (vertices and edges) in the required format.
void graph_print(const Graph *g, const char *label);

/* ────────────────────────────────────────────────────────────────────────── */
#ifdef __cplusplus
}
#endif

#endif /* GRAPH_H */
