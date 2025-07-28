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

typedef struct {
    char u[MAX_NAME_LEN], v[MAX_NAME_LEN];
    int weight;
} Edge;

void primMST(Graph *g) {
    char names[MAX_V][MAX_NAME_LEN];
    int n = graph_get_all_vertices(g, names);
    char label[MAX_NAME_LEN];

    // Sort vertices lexicographically
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

    int key[MAX_V], inMST[MAX_V] = {0}, parent[MAX_V];
    char *heapNames[MAX_V];
    Edge edges[MAX_V];
    int edgeCount = 0, totalWeight = 0;

    Heap *minHeap = heap_create(n);
    for (int i = 0; i < n; i++) {
        key[i] = (i == 0) ? 0 : INF;
        parent[i] = -1;
        heapNames[i] = malloc(strlen(names[i]) + 1);
        strcpy(heapNames[i], names[i]);
        heap_push(minHeap, heapNames[i], key[i]);
    }

    while (!heap_is_empty(minHeap)) {
        int u_key;
        char *u_name = heap_extract_min(minHeap, &u_key);

        int u = -1;
        for (int i = 0; i < n; i++) {
            if (strcmp(names[i], u_name) == 0) {
                u = i;
                break;
            }
        }

        if (u == -1 || inMST[u]) continue;
        inMST[u] = 1;

        // Record edge
        if (parent[u] != -1) {
            const char *a = names[parent[u]];
            const char *b = names[u];
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

        for (int v = 0; v < n; v++) {
            int weight = graph_get_edge_weight(g, names[u], names[v]);
            if (!inMST[v] && weight > 0 && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;

                char *nameCopy = malloc(strlen(names[v]) + 1);
                strcpy(nameCopy, names[v]);
                heap_push(minHeap, nameCopy, weight);
            }
        }
    }

    // Free heapNames and duplicates
    for (int i = 0; i < n; i++) {
        free(heapNames[i]);
    }

    heap_destroy(minHeap);

    // Sort edges
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

    // Output
    strcpy(label, "MST");
    printf("%s = (V,E)\n", label);
    printf("V = {");
    for (int i = 0; i < n; i++) {
        printf("%s", names[i]);
        if (i != n - 1) printf(", ");
    }
    printf("}\n");
    printf("E = {\n");
    for (int i = 0; i < edgeCount; i++) {
        printf("  (%s, %s, %d)", edges[i].u, edges[i].v, edges[i].weight);
        if (i != edgeCount - 1) printf(",\n");
    }
    printf("\n}\n");
    printf("Total Edge Weight: %d\n", totalWeight);
}


