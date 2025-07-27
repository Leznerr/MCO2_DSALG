/* =======================================================================
 *  test_path.c  –  Unit & regression tests for path_check.[ch]
 *  -----------------------------------------------------------------------
 *  Compile (from project root):
 *
 *      gcc -Wall -Wextra -pedantic -std=c11 \
 *          test/test_path.c \
 *          src/graph/graph.c \
 *          src/algorithms/connectivity/path_check.c \
 *          src/data_structures/stack/stack.c  \
 *          -o test_path
 *
 *  Run:
 *      ./test_path
 *
 *  Each REQUIRE-failure prints a message and aborts with EXIT_FAILURE.
 * =======================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "graph.h"
#include "stack.h"
#include "path_check.h"

/* ---------- tiny assert macro ---------- */
static void fail(const char *msg)
{
    fprintf(stderr, "❌  %s\n", msg);
    exit(EXIT_FAILURE);
}
#define REQUIRE(expr)  do { if(!(expr)) fail(#expr); } while(0)

/* ---------- helpers ---------- */
static Graph *make_simple_graph(void)
/* Constructs:   A—B—C   and  D (isolated) */
{
    Graph *g = graph_create();
    graph_add_vertex(g, "A");
    graph_add_vertex(g, "B");
    graph_add_vertex(g, "C");
    graph_add_vertex(g, "D");

    graph_add_edge(g, "A", "B", 1);
    graph_add_edge(g, "B", "C", 2);
    return g;
}

/* Capture the single-line stdout that cmd_path prints */
static char *capture_cmd_path(Graph *g,
                              const char *src,
                              const char *dst,
                              Stack *scratch)
{
    /* redirect stdout to a tmpfile */
    fflush(stdout);
    FILE *tmp = tmpfile();
    if(!tmp) fail("tmpfile failed");

    fflush(stdout);
    int saved_fd = dup(fileno(stdout));
    dup2(fileno(tmp), fileno(stdout));

    bool result = cmd_path(g, src, dst, scratch);

    fflush(stdout);
    fseek(tmp, 0, SEEK_SET);
    static char buf[4]; /* will hold \"0\\n\" or \"1\\n\" + NUL */
    fgets(buf, sizeof buf, tmp);

    /* restore stdout */
    dup2(saved_fd, fileno(stdout));
    close(saved_fd);
    fclose(tmp);

    /* sanity: return value should match first char of buf */
    REQUIRE( (result && buf[0]=='1') || (!result && buf[0]=='0') );
    return buf;
}

/* ---------- test cases ---------- */
static void test_connected_pair(void)
{
    Graph *g = make_simple_graph();
    Stack *st = stack_create(8);

    char *out = capture_cmd_path(g, "A", "C", st);
    REQUIRE(strcmp(out, "1\n") == 0);

    stack_destroy(st);
    graph_destroy(g);
}

static void test_disconnected_pair(void)
{
    Graph *g = make_simple_graph();
    Stack *st = stack_create(4);

    char *out = capture_cmd_path(g, "A", "D", st);
    REQUIRE(strcmp(out, "0\n") == 0);

    stack_destroy(st);
    graph_destroy(g);
}

static void test_same_vertex(void)
{
    Graph *g = make_simple_graph();
    Stack *st = stack_create(2);

    char *out = capture_cmd_path(g, "B", "B", st);
    REQUIRE(strcmp(out, "1\n") == 0);

    stack_destroy(st);
    graph_destroy(g);
}

static void test_invalid_vertex(void)
{
    Graph *g = make_simple_graph();
    Stack *st = stack_create(2);

    char *out = capture_cmd_path(g, "A", "Z", st);
    REQUIRE(strcmp(out, "0\n") == 0);

    stack_destroy(st);
    graph_destroy(g);
}

/* ---------- driver ---------- */
int main(void)
{
    puts("Running path_check unit tests…");

    test_connected_pair();
    test_disconnected_pair();
    test_same_vertex();
    test_invalid_vertex();

    puts("✅  All path_check tests PASSED");
    return EXIT_SUCCESS;
}
