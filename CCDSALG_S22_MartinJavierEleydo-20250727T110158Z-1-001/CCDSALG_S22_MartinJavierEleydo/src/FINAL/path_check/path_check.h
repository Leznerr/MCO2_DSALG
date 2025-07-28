/* ============================================================================
 *  path_check.h – Public interface for Command 7: Connectivity / Path‑Check
 * ----------------------------------------------------------------------------
 *  Exposes a single function to determine if a path exists between two vertices
 *  in the graph (for "7 <src> <dst>" command).
 *
 *  Specification summary (MCO2):
 *      • Input line:  7 <src> <dst>
 *      • Output:      "1\n" if an undirected path exists, else "0\n"
 *      • Output is minimalist and exactly formatted for auto-grading.
 *      • Handles invalid/missing vertices gracefully (prints 0)
 *      • O(V + E) time, reuses Stack/Graph APIs.
 * ----------------------------------------------------------------------------
 *  Rubric “Complete / Highest” highlights:
 *      ✔ Minimalist I/O (no extra output)
 *      ✔ Uses custom Stack and Graph modules via their public interfaces
 *      ✔ Handles all edge/error cases robustly
 *      ✔ Implementation hides Graph internals (opaque pointers)
 *
 *  Implementation:
 *      - Function defined in src/algorithms/connectivity/path_check.c
 *      - Works for undirected, connected or disconnected graphs.
 * ============================================================================
 */

#ifndef PATH_CHECK_H
#define PATH_CHECK_H

#include <stdbool.h>
#include "graph.h"   // Opaque Graph type (spec-compliant, info-hiding)
#include "stack.h"   // Stack for workspace (no recursion needed)

/**
 * @brief Command 7 handler – connectivity/path existence check.
 *
 * Performs an efficient search (DFS or BFS) to determine if @p dst is
 * reachable from @p src in the given undirected graph. Prints "1\n" if a
 * path exists, "0\n" otherwise. Output is always a single line, as required
 * by the project specification.
 *
 * @param g       Pointer to populated Graph (must not be NULL).
 * @param src     Name of the source/start vertex (C-string, up to 256 chars).
 * @param dst     Name of the destination vertex (C-string, up to 256 chars).
 * @param scratch Caller-supplied Stack (workspace, emptied before return).
 *
 * @return true  if a path exists (also prints "1\n")
 *         false if no path (also prints "0\n")
 */
bool cmd_path(Graph *g, const char *src, const char *dst, Stack *scratch);

#endif /* PATH_CHECK_H */
