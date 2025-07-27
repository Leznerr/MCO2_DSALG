/* ============================================================================
 *  shortest_path.h – Public interface for Command 9: Dijkstra's Algorithm
 *  ----------------------------------------------------------------------------
 *  Specification summary (MCO2):
 *      • Input line:  9 <src> <dst>
 *      • Output:      Path in format "v1 -> v2 -> ... -> vN; Total edge cost = X"
 *                     Or "0" if no path exists
 *
 *  Implementation uses Dijkstra's algorithm with the heap data structure
 *  for O((V + E) log V) performance.
 * ==========================================================================*/

#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include "graph.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Command 9 handler – find shortest path using Dijkstra's algorithm.
 *
 * Finds the shortest weighted path from src to dst and prints it in the
 * required format. Uses Dijkstra's algorithm with a min-heap for efficiency.
 *
 * @param g     Pointer to populated Graph (must not be NULL).
 * @param start Source vertex name (C-string = 256 chars).
 * @param end   Destination vertex name (C-string = 256 chars).
 */
void cmd_shortest_path(Graph *g, const char *start, const char *end);

#ifdef __cplusplus
}
#endif

#endif /* SHORTEST_PATH_H */
