/*
 * FUNCTION: shortestPath
 * -----------------------
 * Finds and prints the shortest path between two vertices in the graph
 * using Dijkstra’s algorithm (classic greedy shortest path on weighted graphs).
 *
 * Key algorithm pattern:
 *   - Maintains a distance array (dist[]) holding the shortest-known distance
 *     from the start vertex to every other vertex.
 *   - Repeatedly selects the closest unvisited vertex and "relaxes" its neighbors.
 *   - Remembers the best parent for each vertex to reconstruct the path.
 *
 * Output:
 *   - If a path exists: prints the path in "A -> B -> C" format and total cost
 *   - If no path exists or a vertex is invalid: prints "0"
 *
 * Parameters:
 *   - g: pointer to the Graph structure
 *   - start: name of the starting vertex
 *   - end: name of the destination vertex
 */

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "graph.h"
#include "shortest_Path.h"

#define MAX_V 100
#define INF 999999

/*
 * Helper: minDistance
 * -------------------
 * Scan all vertices to find the unvisited one with the smallest current
 * distance. Returns its index (or -1 if none left).
 *
 * Pattern: Classic Dijkstra with array-based implementation (no heap).
 */
int minDistance(int dist[], int visited[], int n) {
    int min = INF, min_index = -1;
    for (int i = 0; i < n; i++) {
        if (!visited[i] && dist[i] <= min) {
            min = dist[i];
            min_index = i;
        }
    }
    return min_index;
}

/*
 * Main function: shortestPath
 * ---------------------------
 * Implements Dijkstra’s algorithm for the shortest path.
 * Handles all edge/error cases as required by the DSAL project spec.
 */
void shortestPath(Graph *g, const char *start, const char *end) {
    // --- Step 1: Get all vertices in the graph (by name, for index mapping) ---
    char names[MAX_V][256];
    int n = graph_get_all_vertices(g, names);

    // --- Step 2: Initialize Dijkstra arrays ---
    int dist[MAX_V], visited[MAX_V] = {0}, parent[MAX_V];
    for (int i = 0; i < n; i++) {
        dist[i] = INF;      // All distances unknown (infinity)
        parent[i] = -1;     // No path found yet
    }

    // --- Step 3: Map start and end vertex names to indices ---
    int startIndex = -1, endIndex = -1;
    for (int i = 0; i < n; i++) {
        if (strcmp(names[i], start) == 0) startIndex = i;
        if (strcmp(names[i], end) == 0) endIndex = i;
    }

    // --- Step 4: If either start or end does not exist, output "0" ---
    if (startIndex == -1 || endIndex == -1) {
        printf("0\n");
        return;
    }

    dist[startIndex] = 0; // Start vertex has zero cost to itself

    // --- Step 5: Main Dijkstra loop (repeat for all vertices) ---
    for (int count = 0; count < n - 1; count++) {
        // Find the closest unvisited vertex
        int u = minDistance(dist, visited, n);
        if (u == -1) break; // All reachable nodes have been visited

        visited[u] = 1; // Mark as processed

        // Try relaxing all neighbors of u
        for (int v = 0; v < n; v++) {
            int weight = graph_get_edge_weight(g, names[u], names[v]);
            // If (v) is not visited, there is an edge, and path through u is shorter
            if (!visited[v] && weight > 0 && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u; // Record that best path to v is through u
            }
        }
    }

    // --- Step 6: If no path to destination, output "0" ---
    if (dist[endIndex] == INF) {
        printf("0\n");
        return;
    }

    // --- Step 7: Reconstruct path from endIndex back to startIndex ---
    int path[MAX_V], len = 0;
    for (int v = endIndex; v != -1; v = parent[v])
        path[len++] = v;

    // --- Step 8: Print path in required format ---
    for (int i = len - 1; i >= 0; i--) {
        printf("%s", names[path[i]]);
        if (i != 0) printf(" -> ");
    }
    printf("; Total edge cost = %d\n", dist[endIndex]);
}
