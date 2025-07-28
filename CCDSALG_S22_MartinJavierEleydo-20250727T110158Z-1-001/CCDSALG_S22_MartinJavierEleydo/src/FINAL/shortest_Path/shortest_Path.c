/*
 * FUNCTION: shortestPath
 * -----------------------
 * Finds and prints the shortest path between two vertices in the graph
 * using Dijkstra’s algorithm.
 *
 * The algorithm calculates the minimum total edge cost from the start vertex
 * to every other vertex. It reconstructs and prints the specific path from
 * the start to the destination vertex, along with the total cost.
 *
 * Output:
 *   - If a path exists: prints the path in "A -> B -> C" format followed by the total cost
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

void shortestPath(Graph *g, const char *start, const char *end) {
    char names[MAX_V][256];
    int n = graph_get_all_vertices(g, names);

    int dist[MAX_V], visited[MAX_V] = {0}, parent[MAX_V];
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }

    int startIndex = -1, endIndex = -1;
    for (int i = 0; i < n; i++) {
        if (strcmp(names[i], start) == 0) startIndex = i;
        if (strcmp(names[i], end) == 0) endIndex = i;
    }

    if (startIndex == -1 || endIndex == -1) {
        printf("0\n");
        return;
    }

    dist[startIndex] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(dist, visited, n);
        if (u == -1) break;

        visited[u] = 1;

        for (int v = 0; v < n; v++) {
            int weight = graph_get_edge_weight(g, names[u], names[v]);
            if (!visited[v] && weight > 0 && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }

    if (dist[endIndex] == INF) {
        printf("0\n");
        return;
    }

    // Reconstruct path
    int path[MAX_V], len = 0;
    for (int v = endIndex; v != -1; v = parent[v])
        path[len++] = v;

    // Print path in single line
    for (int i = len - 1; i >= 0; i--) {
        printf("%s", names[path[i]]);
        if (i != 0) printf(" -> ");
    }
    printf("; Total edge cost = %d\n", dist[endIndex]);
}


