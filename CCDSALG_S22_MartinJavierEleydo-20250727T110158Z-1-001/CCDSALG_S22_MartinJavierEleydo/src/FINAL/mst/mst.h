/* ============================================================================
 *  mst.h – Public interface for Minimum Spanning Tree (MST) operations (Prim’s)
 * ----------------------------------------------------------------------------
 *  Provides the function prototype for Prim’s MST algorithm using the project’s
 *  Graph module. This header should be included in any file that needs to
 *  compute or print the MST for a given graph.
 *
 *  Specification:
 *    - primMST(Graph *g): Computes and prints the MST of the provided graph
 *      using Prim’s algorithm (min-heap optimized). Output is lexicographically
 *      sorted for both vertices and edges, and total MST weight is printed.
 * ============================================================================
 */

#ifndef MST_H
#define MST_H

#include "graph.h"

/**
 * Computes and prints the Minimum Spanning Tree (MST) of the graph using Prim’s algorithm.
 *
 * @param g Pointer to a populated Graph object.
 *
 * Output:
 *   - Prints the set of vertices and the edges that form the MST,
 *     both in lexicographic order, as required by project spec.
 *   - Also prints the total MST edge weight.
 *
 * Requirements:
 *   - Graph must be undirected and connected for a valid MST.
 *   - Output format is deterministic and minimalist (auto-grader friendly).
 */
void primMST(Graph *g);

#endif /* MST_H */
