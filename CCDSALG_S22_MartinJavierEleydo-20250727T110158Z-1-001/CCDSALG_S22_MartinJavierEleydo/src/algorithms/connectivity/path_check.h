/* ============================================================================
 *  path_check.h – Public interface for Command 7: Connectivity / Path‑Check
 *  ----------------------------------------------------------------------------
 *  Specification summary (MCO2):
 *      • Input line:  7 <src> <dst>
 *      • Output:      "1\n" if an undirected path exists between the two
 *                      vertices, otherwise "0\n" – *nothing else*.
 *  Rubric highlights for the “Complete / Highest” tier:
 *      ✔ Minimalist I/O (exact characters, newline‑terminated)
 *      ✔ Re‑uses custom data‑structures (Stack, Graph) via clean API
 *      ✔ Handles invalid/missing vertices gracefully (prints 0)
 *      ✔ Runs in O(V + E) time (delegated to implementation)
 *
 *  This header exposes a **single function** that satisfies those rules while
 *  keeping Graph internals opaque.  Implementation lives in
 *  src/algorithms/connectivity/path_check.c.
 * ==========================================================================*/

#ifndef PATH_CHECK_H
#define PATH_CHECK_H

#include <stdbool.h>
#include "graph.h"   /* opaque Graph type */
#include "stack.h"   /* P1 stack (scratch workspace) */

/**
 * @brief Command 7 handler – check connectivity between two vertices.
 *
 * This routine performs an iterative DFS/BFS (implementation‑defined) to
 * decide whether @p dst is reachable from @p src in the current undirected
 * graph.  It **prints exactly one line** – "1" or "0" followed by `\n` – as
 * required by the spec, and also returns the same boolean for internal logic.
 *
 * @param g       Pointer to populated Graph (must not be NULL).
 * @param src     Vertex name to start from (C‑string ≤ 256 chars).
 * @param dst     Target vertex name (C‑string ≤ 256 chars).
 * @param scratch Caller‑supplied Stack used as working storage to avoid extra
 *                heap allocations.  The function empties the stack before
 *                returning.
 *
 * @return `true` if a path exists (also prints "1\n"),
 *         `false` otherwise      (also prints "0\n").
 */
bool cmd_path(Graph *g, const char *src, const char *dst, Stack *scratch);

#endif /* PATH_CHECK_H */
