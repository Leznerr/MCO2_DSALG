/* ============================================================================
 *  shortest_path.c – Command 9: Dijkstra's Shortest Path Algorithm
 *  ----------------------------------------------------------------------------
 *  Implements Dijkstra's algorithm using the heap data structure.
 *  Output format: "path1 -> path2 -> ... -> pathN; Total edge cost = X"
 *  Or "0" if no path exists.
 * ==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#include "graph.h"
#include "heap.h"
#include "shortest_path.h"

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

static int get_edge_weight(Vertex *u, const char *v_name)
{
    for (AdjNode *adj = u->adj; adj; adj = adj->next) {
        if (strcmp(adj->dst->name, v_name) == 0) {
            return adj->weight;
        }
    }
    return 0; // No edge
}

/* -- Public implementation ------------------------------------------------ */
void cmd_shortest_path(Graph *g, const char *start, const char *end)
{
    if (!g || !start || !end) {
        printf("0\n");
        return;
    }
    
    // Build vertex array
    Vertex **vertices = NULL;
    size_t n = build_vertex_array(g, &vertices);
    if (n == 0) {
        printf("0\n");
        return;
    }
    
    int start_idx = find_vertex_index(vertices, n, start);
    int end_idx = find_vertex_index(vertices, n, end);
    
    if (start_idx == -1 || end_idx == -1) {
        free(vertices);
        printf("0\n");
        return;
    }
    
    // Initialize distances and parent tracking
    int *dist = malloc(n * sizeof(int));
    int *parent = malloc(n * sizeof(int));
    size_t *heap_indices = malloc(n * sizeof(size_t));
    
    if (!dist || !parent || !heap_indices) {
        free(vertices);
        free(dist);
        free(parent);
        free(heap_indices);
        printf("0\n");
        return;
    }
    
    for (size_t i = 0; i < n; i++) {
        dist[i] = INF;
        parent[i] = -1;
        heap_indices[i] = SIZE_MAX;
    }
    
    dist[start_idx] = 0;
    
    // Create min-heap for Dijkstra's algorithm
    Heap *pq = heap_create(n);
    if (!pq) {
        free(vertices);
        free(dist);
        free(parent);
        free(heap_indices);
        printf("0\n");
        return;
    }
    
    // Add start vertex to heap
    heap_indices[start_idx] = heap_push(pq, &vertices[start_idx], 0);
    
    while (!heap_is_empty(pq)) {
        int key;
        Vertex **u_ptr = (Vertex**)heap_extract_min(pq, &key);
        if (!u_ptr) break;
        
        Vertex *u = *u_ptr;
        int u_idx = find_vertex_index(vertices, n, u->name);
        if (u_idx == -1) continue;
        
        heap_indices[u_idx] = SIZE_MAX; // Mark as processed
        
        if (u_idx == end_idx) break; // Found shortest path to destination
        
        // Check all neighbors
        for (AdjNode *adj = u->adj; adj; adj = adj->next) {
            int v_idx = find_vertex_index(vertices, n, adj->dst->name);
            if (v_idx == -1) continue;
            
            int alt = dist[u_idx] + adj->weight;
            if (alt < dist[v_idx]) {
                dist[v_idx] = alt;
                parent[v_idx] = u_idx;
                
                if (heap_indices[v_idx] == SIZE_MAX) {
                    // Not in heap, add it
                    heap_indices[v_idx] = heap_push(pq, &vertices[v_idx], alt);
                } else {
                    // Already in heap, decrease key
                    heap_decrease_key(pq, heap_indices[v_idx], alt);
                }
            }
        }
    }
    
    // Check if path exists
    if (dist[end_idx] == INF) {
        printf("0\n");
    } else {
        // Reconstruct path
        int *path = malloc(n * sizeof(int));
        if (!path) {
            printf("0\n");
        } else {
            int path_len = 0;
            int current = end_idx;
            
            while (current != -1) {
                path[path_len++] = current;
                current = parent[current];
            }
            
            // Print path in reverse order
            for (int i = path_len - 1; i >= 0; i--) {
                printf("%s", vertices[path[i]]->name);
                if (i > 0) printf(" -> ");
            }
            printf("; Total edge cost = %d\n", dist[end_idx]);
            
            free(path);
        }
    }
    
    heap_destroy(pq);
    free(vertices);
    free(dist);
    free(parent);
    free(heap_indices);
}
