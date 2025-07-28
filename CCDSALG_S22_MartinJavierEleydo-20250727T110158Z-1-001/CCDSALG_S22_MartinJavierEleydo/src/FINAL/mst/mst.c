/*
 * FUNCTION: primMST
 * -----------------
 * Computes and prints the Minimum Spanning Tree (MST) of the graph
 * using Prim’s algorithm with a binary min-heap for efficiency.
 *
 * The MST is built by starting from an arbitrary vertex and repeatedly
 * adding the smallest-weight edge that connects a visited vertex to
 * an unvisited one. A heap is used to always select the next cheapest edge.
 *
 * Output:
 *   - Prints the set of vertices (V) and the selected edges (E) that form the MST
 *   - The vertices and edges are displayed in lexicographic order
 *   - Also prints the total weight of all MST edges
 *
 * Parameters:
 *   - g: pointer to the Graph structure
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "heap.h"

#define MAX_V 100
#define INF 999999
#define MAX_NAME_LEN 256

// Struct to hold edge information for printing
typedef struct {
    char u[MAX_NAME_LEN], v[MAX_NAME_LEN];
    int weight;
} Edge;

void primMST(Graph *g) {
    // --------------------------------------------------------------------------
    // STEP 1: Get all vertices and sort them lexicographically
    // --------------------------------------------------------------------------
    char names[MAX_V][MAX_NAME_LEN];
    int n = graph_get_all_vertices(g, names);
    char label[MAX_NAME_LEN];

    // Sorting vertices: ensures that both the MST and output are deterministic
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp(names[i], names[j]) > 0) {
                char tmp[MAX_NAME_LEN];
                strcpy(tmp, names[i]);
                strcpy(names[i], names[j]);
                strcpy(names[j], tmp);
            }
        }
    }

    // --------------------------------------------------------------------------
    // STEP 2: Initialize Prim’s Algorithm Structures
    // --------------------------------------------------------------------------
    int key[MAX_V];        // key[v]: minimum weight to connect vertex v to MST
    int inMST[MAX_V] = {0}; // inMST[v]: whether vertex v is included in MST
    int parent[MAX_V];     // parent[v]: parent of v in MST
    char *heapNames[MAX_V];// heapNames[i]: dynamic names for min-heap
    Edge edges[MAX_V];     // To record MST edges for printing
    int edgeCount = 0, totalWeight = 0;

    // Create a min-heap for vertex selection by key
    Heap *minHeap = heap_create(n);
    for (int i = 0; i < n; i++) {
        key[i] = (i == 0) ? 0 : INF; // Start from the first vertex (arbitrary)
        parent[i] = -1;
        heapNames[i] = malloc(strlen(names[i]) + 1);
        strcpy(heapNames[i], names[i]);
        heap_push(minHeap, heapNames[i], key[i]);
    }

    // --------------------------------------------------------------------------
    // STEP 3: Prim’s Algorithm Main Loop
    // --------------------------------------------------------------------------
    while (!heap_is_empty(minHeap)) {
        int u_key;
        char *u_name = heap_extract_min(minHeap, &u_key);

        // Find the index of u_name in names[] (for O(1) access to key/parent)
        int u = -1;
        for (int i = 0; i < n; i++) {
            if (strcmp(names[i], u_name) == 0) {
                u = i;
                break;
            }
        }

        // If vertex already included, or error, skip
        if (u == -1 || inMST[u]) continue;
        inMST[u] = 1;

        // If this is not the root, record MST edge (parent[u] - u)
        if (parent[u] != -1) {
            const char *a = names[parent[u]];
            const char *b = names[u];
            // Always record edge in lex order (for deterministic print)
            if (strcmp(a, b) < 0) {
                strcpy(edges[edgeCount].u, a);
                strcpy(edges[edgeCount].v, b);
            } else {
                strcpy(edges[edgeCount].u, b);
                strcpy(edges[edgeCount].v, a);
            }
            edges[edgeCount].weight = key[u];
            totalWeight += key[u];
            edgeCount++;
        }

        // Update neighbors: for every vertex v, if edge u-v exists, and v not in MST,
        // and weight is lower than current key[v], update key and parent.
        for (int v = 0; v < n; v++) {
            int weight = graph_get_edge_weight(g, names[u], names[v]);
            if (!inMST[v] && weight > 0 && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;

                // Instead of decrease-key (no heap handles), push new (name, weight)
                // This may leave outdated heap entries, but correctness is preserved
                char *nameCopy = malloc(strlen(names[v]) + 1);
                strcpy(nameCopy, names[v]);
                heap_push(minHeap, nameCopy, weight);
            }
        }
    }

    // --------------------------------------------------------------------------
    // STEP 4: Cleanup Dynamic Names and Heap Memory
    // --------------------------------------------------------------------------
    for (int i = 0; i < n; i++) {
        free(heapNames[i]);
    }
    heap_destroy(minHeap);

    // --------------------------------------------------------------------------
    // STEP 5: Sort and Print MST Output
    // --------------------------------------------------------------------------
    // Sort edges in lexicographic order for deterministic output
    for (int i = 0; i < edgeCount - 1; i++) {
        for (int j = i + 1; j < edgeCount; j++) {
            if (strcmp(edges[i].u, edges[j].u) > 0 ||
                (strcmp(edges[i].u, edges[j].u) == 0 && strcmp(edges[i].v, edges[j].v) > 0)) {
                Edge temp = edges[i];
                edges[i] = edges[j];
                edges[j] = temp;
            }
        }
    }

    // Print the MST in the required format
    strcpy(label, "MST");
    printf("%s = (V,E)\n", label);

    // Print vertices set
    printf("V = {");
    for (int i = 0; i < n; i++) {
        printf("%s", names[i]);
        if (i != n - 1) printf(", ");
    }
    printf("}\n");

    // Print edges set
    printf("E = {\n");
    for (int i = 0; i < edgeCount; i++) {
        printf("  (%s, %s, %d)", edges[i].u, edges[i].v, edges[i].weight);
        if (i != edgeCount - 1) printf(",\n");
    }
    printf("\n}\n");

    // Print total weight of the MST
    printf("Total Edge Weight: %d\n", totalWeight);
}
