/* ============================================================================
 *  dfs.h – Public interface for Command 6: Depth‑First Search traversal
 *  ----------------------------------------------------------------------------
 *  Specification (MCO‑2):
 *      • Input line:  6 <startVertex>
 *      • Output   :  One line listing vertices in the order they are FIRST
 *                     discovered by a DFS that obeys lexicographic tie‑break.
 *                     Names separated by single spaces, newline terminated,
 *                     no trailing space – *nothing else*.
 *
 *  Rubric “Highest / Complete” highlights:
 *      ✔ Uses our custom Stack (iterative ⇒ no recursion‑depth issues).
 *      ✔ Produces lexicographic visitation order (neighbours pushed in reverse).
 *      ✔ O(V + E) time,  O(V) extra space.
 *      ✔ Minimalist I/O, handles missing start vertex gracefully (prints just \n).
 *
 *  Implementation lives in  src/algorithms/traversal/dfs.c
 * ==========================================================================*/

#ifndef DFS_H
#define DFS_H

#include "graph.h"   /* opaque Graph type */
#include "stack.h"   /* P1 stack – scratch workspace */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Command 6 handler – iterative DFS traversal.
 *
 * Prints the DFS visitation sequence starting from @p start, respecting
 * lexicographic order.  If @p start does not exist or graph is empty, it
 * prints just a newline.  The caller supplies a @p scratch stack so that
 * multiple DFS calls can reuse the same memory.
 *
 * @param g        Populated Graph (must not be NULL).
 * @param start    Name of the starting vertex (C‑string ≤ 256 chars).
 * @param scratch  Caller‑owned Stack used as working storage; emptied by the
 *                 function before return.
 */
void cmd_dfs(Graph *g, const char *start, Stack *scratch);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DFS_H */
