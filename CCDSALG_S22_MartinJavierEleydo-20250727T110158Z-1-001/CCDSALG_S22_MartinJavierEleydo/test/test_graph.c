/* test_graph.c – revised white-box/black-box unit tests for graph.c
 *
 *  ✦ 2025-07-23 – fixes “incomplete type ‘struct Graph’” diagnostics by
 *    using a private mirror struct instead of touching Graph’s real name.
 *
 *  Build:
 *      gcc -Wall -Wextra -pedantic -std=c11 \
 *          test_graph.c ../src/graph/graph.c  -o test_graph
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "graph.h"    /* public API – still opaque */

#define REQUIRE(cond)                                                        \
    do {                                                                     \
        if (!(cond)) {                                                       \
            fprintf(stderr, "[FAIL] %s:%d  %s\n",                            \
                    __FILE__, __LINE__, #cond);                              \
            exit(EXIT_FAILURE);                                              \
        }                                                                    \
    } while (0)

/* --------------------------------------------------------------------------
 *  White-box peek helper
 * --------------------------------------------------------------------------*/
/* Mirror only the fields we care about; order matches graph.c implementation */
typedef struct {
    void   *v_head;      /* we never dereference this in the test */
    size_t  v_count;
    size_t  e_count;
} GraphPriv;

#define GP(gptr)  ((GraphPriv *)(gptr))

/* --------------------------------------------------------------------------
 *  Tests
 * --------------------------------------------------------------------------*/
static void test_vertex_insertion(void)
{
    Graph *g = graph_create();
    REQUIRE(g);

    REQUIRE( graph_add_vertex(g, "B") );
    REQUIRE( graph_add_vertex(g, "A") );
    REQUIRE(!graph_add_vertex(g, "A") );        /* duplicate */
    REQUIRE( GP(g)->v_count == 2 );             /* white-box counter check */

    graph_destroy(g);
}

static void test_edge_logic(void)
{
    Graph *g = graph_create();
    graph_add_vertex(g, "A");
    graph_add_vertex(g, "B");
    graph_add_vertex(g, "C");

    REQUIRE(!graph_edge_exists(g, "A", "B"));
    REQUIRE( graph_add_edge(g, "A", "B", 5) );
    REQUIRE( graph_edge_exists(g, "A", "B") );
    REQUIRE( graph_get_degree(g, "A") == 1 );

    size_t saved_e = GP(g)->e_count;

    /* overwrite – same edge, different weight */
    REQUIRE( graph_add_edge(g, "B", "A", 3) );
    REQUIRE( GP(g)->e_count == saved_e );  /* still one logical edge */

    /* bad attempts */
    REQUIRE(!graph_add_edge(g, "A", "Z", 1));
    REQUIRE(!graph_add_edge(g, "A", "A", 2));
    REQUIRE(!graph_add_edge(g, "A", "B", 101));

    graph_destroy(g);
}

static void test_print_example(void)
{
    Graph *g = graph_create();
    graph_add_vertex(g, "C");
    graph_add_vertex(g, "A");
    graph_add_vertex(g, "B");
    graph_add_edge(g, "A", "B", 7);
    graph_add_edge(g, "B", "C", 2);

    puts("\n--- graph_print demo (verify visually) ---");
    graph_print(g, "G");
    puts("-------------------------------------------\n");

    graph_destroy(g);
}

int main(void)
{
    test_vertex_insertion();
    test_edge_logic();
    test_print_example();

    puts("All graph tests passed ✔");
    return 0;
}
