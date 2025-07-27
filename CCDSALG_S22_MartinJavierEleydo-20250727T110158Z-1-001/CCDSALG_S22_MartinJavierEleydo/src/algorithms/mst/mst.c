/* ============================================================================
 *  mst.c – Command 8: Minimum Spanning Tree using Prim's Algorithm
 *  ----------------------------------------------------------------------------
 *  Implements Prim's algorithm using the heap data structure.
 *  Produces MST with lexicographically sorted output.
 * ==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#include "graph.h"
#include "heap.h"
#include "mst.h"

#define MAX_VERTICES 1000
#define INF INT_MAX

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

/* -- MST Edge structure ---------------------------------------------------- */
typedef struct {
    char u[257], v[257];
    int weight;
} MST_Edge;

/* -- Helper functions ---------------------------------------------------- */
static size_t build_vertex_array(const Graph *g, Vertex ***out_vertices)
{
    size_t n = g->v_count;
    if (n == 0) { *out_vertices = NULL; return 0; }
    
    Vertex **vertices = malloc(n * sizeof(Vertex*));
    if (!vertices) { *out_vertices = NULL; return 0; }
    
    size_t i = 0;
    for (Vertex *v = g->v_head; v; v = v->next) {
        vertices[i++] = v;
    }
    
    *out_vertices = vertices;
    return n;
}

static int find_vertex_index(Vertex **vertices, size_t n, const char *name)
{
    for (size_t i = 0; i < n; i++) {
        if (strcmp(vertices[i]->name, name) == 0) {
            return (int)i;
        }
    }
    return -1;
}

static int compare_edges(const void *a, const void *b)
{
    const MST_Edge *ea = (const MST_Edge*)a;
    const MST_Edge *eb = (const MST_Edge*)b;
    
    int cmp = strcmp(ea->u, eb->u);
    if (cmp == 0) {
        cmp = strcmp(ea->v, eb->v);
    }
    return cmp;
}

/* -- Public implementation ------------------------------------------------ */
void cmd_mst(Graph *g)
{
    if (!g || g->v_count == 0) {
        printf("MST(G) = (V,E)\nV = {}\nE = {\n}\nTotal Edge Weight: 0\n");
        return;
    }
    
    // Build vertex array
    Vertex **vertices = NULL;
    size_t n = build_vertex_array(g, &vertices);
    if (n == 0) {
        printf("MST(G) = (V,E)\nV = {}\nE = {\n}\nTotal Edge Weight: 0\n");
        return;
    }
    
    // Initialize arrays for Prim's algorithm
    int *key = malloc(n * sizeof(int));
    int *parent = malloc(n * sizeof(int));
    bool *in_mst = malloc(n * sizeof(bool));
    size_t *heap_indices = malloc(n * sizeof(size_t));
    
    if (!key || !parent || !in_mst || !heap_indices) {
        free(vertices);
        free(key);
        free(parent);
        free(in_mst);
        free(heap_indices);
        printf("MST(G) = (V,E)\nV = {}\nE = {\n}\nTotal Edge Weight: 0\n");
        return;
    }
    
    // Initialize all keys to infinity, parent to -1, not in MST
    for (size_t i = 0; i < n; i++) {
        key[i] = INF;
        parent[i] = -1;
        in_mst[i] = false;
        heap_indices[i] = SIZE_MAX;
    }
    
    MST_Edge *mst_edges = malloc((n-1) * sizeof(MST_Edge));
    if (!mst_edges) {
        free(vertices);
        free(key); free(parent); free(in_mst); free(heap_indices);
        printf("MST(G) = (V,E)\nV = {}\nE = {\n}\nTotal Edge Weight: 0\n");
        return;
    }
    
    int total_weight = 0;
    size_t edge_count = 0;
    
    // Create min-heap for Prim's algorithm
    Heap *pq = heap_create(n);
    if (!pq) {
        free(vertices);
        free(key); free(parent); free(in_mst); free(heap_indices); free(mst_edges);
        printf("MST(G) = (V,E)\nV = {}\nE = {\n}\nTotal Edge Weight: 0\n");
        return;
    }
    
    // Start with vertex 0 (lexicographically first)
    key[0] = 0;
    heap_indices[0] = heap_push(pq, &vertices[0], 0);
    
    while (!heap_is_empty(pq)) {
        int min_key;
        Vertex **u_ptr = (Vertex**)heap_extract_min(pq, &min_key);
        if (!u_ptr) break;
        
        Vertex *u = *u_ptr;
        int u_idx = find_vertex_index(vertices, n, u->name);
        if (u_idx == -1 || in_mst[u_idx]) continue;
        
        in_mst[u_idx] = true;
        heap_indices[u_idx] = SIZE_MAX;
        
        // Add edge to MST (except for the first vertex)
        if (parent[u_idx] != -1) {
            const char *p_name = vertices[parent[u_idx]]->name;
            const char *u_name = u->name;
            
            // Store edge with lexicographically smaller vertex first
            if (strcmp(p_name, u_name) < 0) {
                strcpy(mst_edges[edge_count].u, p_name);
                strcpy(mst_edges[edge_count].v, u_name);
            } else {
                strcpy(mst_edges[edge_count].u, u_name);
                strcpy(mst_edges[edge_count].v, p_name);
            }
            mst_edges[edge_count].weight = key[u_idx];
            total_weight += key[u_idx];
            edge_count++;
        }
        
        // Update keys of adjacent vertices
        for (AdjNode *adj = u->adj; adj; adj = adj->next) {
            int v_idx = find_vertex_index(vertices, n, adj->dst->name);
            if (v_idx == -1 || in_mst[v_idx]) continue;
            
            if (adj->weight < key[v_idx]) {
                key[v_idx] = adj->weight;
                parent[v_idx] = u_idx;
                
                if (heap_indices[v_idx] == SIZE_MAX) {
                    // Not in heap, add it
                    heap_indices[v_idx] = heap_push(pq, &vertices[v_idx], adj->weight);
                } else {
                    // Already in heap, decrease key
                    heap_decrease_key(pq, heap_indices[v_idx], adj->weight);
                }
            }
        }
    }
    
    // Sort MST edges lexicographically
    qsort(mst_edges, edge_count, sizeof(MST_Edge), compare_edges);
    
    // Print MST
    printf("MST(G) = (V,E)\n");
    printf("V = {");
    for (size_t i = 0; i < n; i++) {
        printf("%s", vertices[i]->name);
        if (i < n - 1) printf(", ");
    }
    printf("}\n");
    
    printf("E = {\n");
    for (size_t i = 0; i < edge_count; i++) {
        printf("  (%s, %s, %d)", mst_edges[i].u, mst_edges[i].v, mst_edges[i].weight);
        if (i < edge_count - 1) printf(",");
        printf("\n");
    }
    printf("}\n");
    
    printf("Total Edge Weight: %d\n", total_weight);
    
    // Cleanup
    heap_destroy(pq);
    free(vertices);
    free(key); free(parent); free(in_mst); free(heap_indices);
    free(mst_edges);
}
