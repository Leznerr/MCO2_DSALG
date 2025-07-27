#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "bfs.h"
#include "mst.h"
#include "shortestPath.h"

int main() {
    Graph* graph = graph_create();  // uses opaque struct

    char command[10], name1[256], name2[256];
    int weight;

    while (scanf("%s", command) != EOF) {
        if (strcmp(command, "11") == 0)
            break;

        if (strcmp(command, "1") == 0) {
            // Add Vertex: 1 <name>
            scanf("%s", name1);
            graph_add_vertex(graph, name1);
        }
        else if (strcmp(command, "2") == 0) {
            // Add Edge: 2 <name1> <name2> <weight>
            scanf("%s %s %d", name1, name2, &weight);
            graph_add_edge(graph, name1, name2, weight);
        }
        else if (strcmp(command, "3") == 0) {
            // Get Degree: 3 <name>
            scanf("%s", name1);
            get_degree(graph, name1);
        }
        else if (strcmp(command, "4") == 0) {
            // Edge Check: 4 <name1> <name2>
            scanf("%s %s", name1, name2);
            edge_check(graph, name1, name2);
        }
        else if (strcmp(command, "5") == 0) {
            // BFS: 5 <start>
            scanf("%s", name1);
            if (graph_vertex_exists(graph, name1))
                bfs(graph, name1);
        }
        else if (strcmp(command, "8") == 0) {
            // MST (Prim's): 8
            primMST(graph);
        }
        else if (strcmp(command, "9") == 0) {
            // Shortest Path: 9 <start> <end>
            scanf("%s %s", name1, name2);
            if (graph_vertex_exists(graph, name1) && graph_vertex_exists(graph, name2))
                shortestPath(graph, name1, name2);
        }
        else if (strcmp(command, "10") == 0) {
            // Print Graph: 10
            graph_print(graph, NULL);
        }
    }
    graph_destroy(graph);
    return 0;

    
}
