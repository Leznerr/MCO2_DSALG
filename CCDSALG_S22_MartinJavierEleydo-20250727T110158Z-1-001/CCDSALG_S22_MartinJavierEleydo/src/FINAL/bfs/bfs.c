#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "graph.h"
#include "queue.h" 
#include "bfs.h"

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
 * - Prints the name of each visited vertex, one per line,
 * in the order they are discovered.
 *
 * Parameters:
 * - g: pointer to the Graph structure
 * - startName: name of the starting vertex for BFS traversal
 */
void bfs(Graph* g, const char* startName) {
    // Step 1: Validate that the starting vertex exists in the graph.
    // If not, the traversal cannot begin, so the function returns immediately.
    if (!graph_vertex_exists(g, startName)) return;

    // Step 2: Initialize the queue data structure.
    // This queue will hold the names of vertices that have been discovered but whose
    // neighbors have not yet been explored.
    Queue* q = queue_create(0); // Use the queue's default capacity.
    if (!q) return; // Exit if memory for the queue could not be allocated.

    // Step 3: Initialize a data structure to track visited vertices.
    // A 2D char array stores the names of visited nodes to prevent cycles
    // and ensure each vertex is processed only once.
    char visited[MAX_VERTICES][MAX_NAME_LEN];
    int visitedCount = 0;

    // Step 4: Begin the traversal from the starting vertex.
    // A copy of the start name is allocated because the queue will take ownership
    // of the memory and free it later.
    char* startCopy = strdup(startName);
    queue_enqueue(q, startCopy);
    strcpy(visited[visitedCount++], startName); // Mark the start vertex as visited.
    printf("%s\n", startName);                  // Print the start vertex upon discovery.

    // Step 5: Main traversal loop.
    // Continue processing vertices as long as the queue is not empty.
    while (!queue_is_empty(q)) {
        // Dequeue the next vertex in the traversal order.
        char* current = queue_dequeue(q);
        if (!current) break; // Safety check in case of an empty queue.

        // Retrieve all neighbors of the current vertex from the graph.
        char neighbors[MAX_VERTICES][MAX_NAME_LEN];
        int count = graph_get_neighbors(g, current, neighbors);
        free(current); // The dequeued name is no longer needed; free its memory.

        // --- Sort Neighbors for Deterministic Order ---
        // To meet the lexicographical ordering requirement, this section sorts the
        // retrieved neighbor names using a Bubble Sort algorithm before processing.
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

        // Step 6: Iterate through the sorted neighbors.
        for (int i = 0; i < count; ++i) {
            // Check if the current neighbor has already been visited by searching
            // the 'visited' array.
            bool seen = false;
            for (int j = 0; j < visitedCount; ++j) {
                if (strcmp(visited[j], neighbors[i]) == 0) {
                    seen = true;
                    break;
                }
            }

            // If the neighbor has not been visited yet:
            if (!seen) {
                // Mark the neighbor as visited.
                strcpy(visited[visitedCount++], neighbors[i]);
                // Allocate a new copy of the name to be enqueued.
                char* nameCopy = strdup(neighbors[i]);
                queue_enqueue(q, nameCopy);
                // Print the neighbor's name upon discovery.
                printf("%s\n", neighbors[i]);
            }
        }
    }

    // Step 7: Clean up resources.
    // The queue and all its remaining allocated strings are destroyed.
    queue_destroy(q);
    // Print a final newline for correct output formatting as per the spec.
    putchar('\n');
}