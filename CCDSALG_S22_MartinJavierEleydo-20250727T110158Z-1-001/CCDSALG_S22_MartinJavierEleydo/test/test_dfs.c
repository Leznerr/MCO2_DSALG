/* =======================================================================
 *  test_dfs.c  –  Unit & regression tests for dfs.[ch]
 *  -----------------------------------------------------------------------
 *  Compile (from project root):
 *
 *      gcc -Wall -Wextra -pedantic -std=c11 \
 *          test/test_dfs.c \
 *          src/graph/graph.c \
 *          src/algorithms/traversal/dfs.c \
 *          src/data_structures/stack/stack.c \
 *          -o test_dfs
 *
 *  Run:
 *      ./test_dfs
 *
 *  PASS ⇒ program exits 0 and prints a short summary.
 * =======================================================================
 */
#define _POSIX_C_SOURCE 200809L   /* for dup/fileno on non-POSIX builds */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     /* dup, fileno */

#include "graph.h"
#include "stack.h"
#include "dfs.h"

/* ---------- tiny assertion wrapper ---------- */
static void fail(const char *msg)
{
    fprintf(stderr, "❌  %s\n", msg);
    exit(EXIT_FAILURE);
}
#define REQUIRE(expr)  do { if(!(expr)) fail(#expr); } while(0)

/* ---------- capture helper ---------- */
static char *capture_cmd_dfs(Graph *g,
                             const char *start,
                             Stack *scratch,
                             char *buf,
                             size_t buf_sz)
{
    fflush(stdout);
    FILE *tmp = tmpfile();
    if(!tmp) fail("tmpfile failed");

    int saved_fd = dup(fileno(stdout));
    dup2(fileno(tmp), fileno(stdout));

    cmd_dfs(g, start, scratch);

    fflush(stdout);
    fseek(tmp, 0, SEEK_SET);
    fgets(buf, (int)buf_sz, tmp);

    dup2(saved_fd, fileno(stdout));
    close(saved_fd);
    fclose(tmp);
    return buf;
}

/* ---------- graph builder ---------- */
static Graph *make_graph(void)
/* Graph:  A--B--C   ,  A--D   ,  E (isolated) */
{
    Graph *g = graph_create();
    graph_add_vertex(g, "A"); graph_add_vertex(g, "B");
    graph_add_vertex(g, "C"); graph_add_vertex(g, "D");
    graph_add_vertex(g, "E");

    graph_add_edge(g, "A", "B", 1);
    graph_add_edge(g, "B", "C", 2);
    graph_add_edge(g, "A", "D", 3);
    return g;
}

/* ---------- tests ---------- */
static void test_dfs_order(void)
{
    Graph *g = make_graph();
    Stack *st = stack_create(16);

    char out[64];
    capture_cmd_dfs(g, "A", st, out, sizeof out);
    /* Expected lexicographic visitation: A B C D */
    REQUIRE(strcmp(out, "A B C D\n") == 0);

    stack_destroy(st);
    graph_destroy(g);
}

static void test_isolated_vertex(void)
{
    Graph *g = make_graph();
    Stack *st = stack_create(4);

    char out[16];
    capture_cmd_dfs(g, "E", st, out, sizeof out);
    REQUIRE(strcmp(out, "E\n") == 0);

    stack_destroy(st);
    graph_destroy(g);
}

static void test_missing_start_vertex(void)
{
    Graph *g = make_graph();
    Stack *st = stack_create(4);

    char out[4];
    capture_cmd_dfs(g, "Z", st, out, sizeof out);
    REQUIRE(strcmp(out, "\n") == 0);   /* prints just newline */

    stack_destroy(st);
    graph_destroy(g);
}

/* ---------- driver ---------- */
int main(void)
{
    puts("Running dfs unit tests…");

    test_dfs_order();
    test_isolated_vertex();
    test_missing_start_vertex();

    puts("✅  All dfs tests PASSED");
    return EXIT_SUCCESS;
}
