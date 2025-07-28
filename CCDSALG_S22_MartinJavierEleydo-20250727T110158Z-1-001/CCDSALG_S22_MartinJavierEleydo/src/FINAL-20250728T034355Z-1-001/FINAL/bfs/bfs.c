/*
 * FUNCTION: bfs (Breadth-First Search)
 * ------------------------------------
 * Performs a Breadth-First Search (BFS) traversal of the graph,
 * starting from the specified vertex.
 *
 * The function visits vertices layer by layer, exploring all neighbors
 * of the current vertex before moving to the next level. It uses a queue
 * to manage traversal order and ensures that each vertex is printed only once,
 * in the order they are first discovered. Neighbor vertices are visited in
 * lexicographic order for consistent output.
 *
 * Output:
 *   - Prints the name of each visited vertex, one per line,
 *     in the order they are discovered.
 *
 * Parameters:
 *   - g: pointer to the Graph structure
 *   - startName: name of the starting vertex for BFS traversal
 */


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "graph.h"
#include "queue.h" 
#include "bfs.h"

void bfs(Graph* g, const char* startName) {
    if (!graph_vertex_exists(g, startName)) return;

    Queue* q = queue_create(0); // use default capacity
    if (!q) return;

    char visited[MAX_VERTICES][MAX_NAME_LEN];
    int visitedCount = 0;

    // Allocate and enqueue the start vertex name
    char* startCopy = strdup(startName);
    queue_enqueue(q, startCopy);
    strcpy(visited[visitedCount++], startName);
    printf("%s\n", startName);

    while (!queue_is_empty(q)) {
        // Dequeue and free memory after use
        char* current = queue_dequeue(q);
        if (!current) break;

        char neighbors[MAX_VERTICES][MAX_NAME_LEN];
        int count = graph_get_neighbors(g, current, neighbors);
        free(current); // prevent memory leak

        // Lexicographically sort neighbors
        for (int i = 0; i < count - 1; ++i) {
            for (int j = i + 1; j < count; ++j) {
                if (strcmp(neighbors[i], neighbors[j]) > 0) {
                    char tmp[MAX_NAME_LEN];
                    strcpy(tmp, neighbors[i]);
                    strcpy(neighbors[i], neighbors[j]);
                    strcpy(neighbors[j], tmp);
                }
            }
        }

        // Visit each neighbor
        for (int i = 0; i < count; ++i) {
            bool seen = false;
            for (int j = 0; j < visitedCount; ++j) {
                if (strcmp(visited[j], neighbors[i]) == 0) {
                    seen = true;
                    break;
                }
            }

            if (!seen) {
                strcpy(visited[visitedCount++], neighbors[i]);
                char* nameCopy = strdup(neighbors[i]); // allocate new string
                queue_enqueue(q, nameCopy);
                printf("%s\n", neighbors[i]);
            }
        }
    }

    queue_destroy(q);
    putchar('\n');
}