/* ============================================================================
 *  mst.h – Public interface for Command 8: Minimum Spanning Tree
 *  ----------------------------------------------------------------------------
 *  Specification summary (MCO2):
 *      • Input line:  8
 *      • Output:      MST in format:
 *                     "MST(G) = (V,E)"
 *                     "V = {v1, v2, ...}"
 *                     "E = {"
 *                     "  (u1, v1, w1),"
 *                     "  (u2, v2, w2)"
 *                     "}"
 *                     "Total Edge Weight: X"
 *
 *  Implementation uses Prim's algorithm with the heap data structure
 *  for O((V + E) log V) performance.
 * ==========================================================================*/

#ifndef MST_H
#define MST_H

#include "graph.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Command 8 handler – find minimum spanning tree using Prim's algorithm.
 *
 * Finds the MST of the graph and prints it in the required format.
 * Uses Prim's algorithm with a min-heap for efficiency.
 *
 * @param g Pointer to populated Graph (must not be NULL).
 */
void cmd_mst(Graph *g);

#ifdef __cplusplus
}
#endif

#endif /* MST_H */
