/* ============================================================================
 *  bfs.h – Public interface for Command 5: Breadth-First Search traversal
 *  ----------------------------------------------------------------------------
 *  Specification (MCO-2):
 *      • Input line:  5 <startVertex>
 *      • Output   :  One line listing vertices in the order they are FIRST
 *                     discovered by a BFS that obeys lexicographic tie-break.
 *                     Names separated by single spaces, newline terminated,
 *                     no trailing space – *nothing else*.
 *
 *  Rubric "Highest / Complete" highlights:
 *      ? Uses our custom Queue (iterative ? proper BFS behavior).
 *      ? Produces lexicographic visitation order.
 *      ? O(V + E) time,  O(V) extra space.
 *      ? Minimalist I/O, handles missing start vertex gracefully (prints just \n).
 *
 *  Implementation lives in  src/algorithms/traversal/bfs.c
 * ==========================================================================*/

#ifndef BFS_H
#define BFS_H

#include "graph.h"   /* opaque Graph type */
#include "queue.h"   /* P1 queue – scratch workspace */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Command 5 handler – iterative BFS traversal.
 *
 * Prints the BFS visitation sequence starting from @p start, respecting
 * lexicographic order.  If @p start does not exist or graph is empty, it
 * prints just a newline.  The caller supplies a @p scratch queue so that
 * multiple BFS calls can reuse the same memory.
 *
 * @param g        Populated Graph (must not be NULL).
 * @param start    Name of the starting vertex (C-string = 256 chars).
 * @param scratch  Caller-owned Queue used as working storage; emptied by the
 *                 function before return.
 */
void cmd_bfs(Graph *g, const char *start, Queue *scratch);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BFS_H */
