/**
 * @file bfs.h
 * @brief Public interface for the Breadth-First Search (BFS) traversal algorithm.
 *
 * This header file declares the primary function for performing a BFS traversal
 * on a graph data structure. It should be included by any module that needs to
 * initiate a BFS operation.
 */

// Header guard to prevent the file from being included multiple times,
// which would cause redefinition errors.
#ifndef BFS_H
#define BFS_H

// Includes the definition for the Graph struct, which is a required
// parameter for the bfs function.
#include "graph.h"

/**
 * @brief Performs a Breadth-First Search (BFS) on a graph from a starting vertex.
 *
 * This function traverses the graph in a breadth-first manner, visiting all
 * neighbors of a vertex before moving to the next level. It prints the name of each
 * visited vertex to standard output on a new line. The order in which neighbors
 * are visited depends on the lexicographically sorted adjacency lists provided
 * by the graph structure.
 *
 * @param g A pointer to the Graph structure to be traversed.
 * @param startName A constant character pointer to the name of the starting vertex.
 * This vertex must exist in the graph for the traversal to start.
 */
void bfs(Graph* g, const char* startName);

#endif // BFS_H