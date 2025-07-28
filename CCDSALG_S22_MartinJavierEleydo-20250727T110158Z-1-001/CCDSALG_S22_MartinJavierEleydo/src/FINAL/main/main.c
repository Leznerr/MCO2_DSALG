/* ============================================================================
 *  Command-Line Interface for Graph Operations (CCDSALG MCO-2)
 * ----------------------------------------------------------------------------
 *  Parses and dispatches commands for an interactive graph system.
 *  
 *
 *  Supported Commands:
 *    1  <name>           - Add vertex
 *    2  <u> <v> <weight> - Add edge (undirected, weighted)
 *    3  <name>           - Get vertex degree
 *    4  <u> <v>          - Check if edge exists
 *    5  <start>          - BFS traversal
 *    6  <start>          - DFS traversal  
 *    7  <src> <dst>      - Check path connectivity
 *    8                   - Find MST (Prim’s)
 *    9  <src> <dst>      - Find shortest path (Dijkstra’s)
 *    10                  - Print graph (vertices/edges)
 *    11                  - Exit
 *
 *  Design Notes:
 *    - Input is line-based; commands must be precisely formatted.
 *    - Unknown or malformed commands are ignored or output minimal default.
 *    - Each command is dispatched to a handler for modularity and clarity.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Core data structures
#include "graph.h"
#include "stack.h"
#include "queue.h"
#include "heap.h"

// Algorithm modules (each provides a spec-compliant function)
#include "dfs.h"
#include "bfs.h"
#include "path_check.h"
#include "mst.h"
#include "shortest_Path.h"

#define MAX_LINE_LEN 1024
#define MAX_TOKEN_LEN 257

/* ============================================================================
 *  INPUT PARSING HELPERS
 * ============================================================================
 */

/**
 * Remove leading and trailing whitespace from a string in-place.
 * Leaves a clean, null-terminated string.
 */
static void trim_whitespace(char *str)
{
    if (!str) return;
    // Trim leading
    char *start = str;
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r') {
        start++;
    }
    // Trim trailing
    char *end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }
    // Move if trimmed off front
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

/**
 * Split a string into whitespace-separated tokens, up to max_tokens.
 * Returns the number of tokens parsed.
 */
static int parse_tokens(char *line, char tokens[][MAX_TOKEN_LEN], int max_tokens)
{
    if (!line || !tokens) return 0;
    int count = 0;
    char *token = strtok(line, " \t\n\r");
    while (token && count < max_tokens) {
        strncpy(tokens[count], token, MAX_TOKEN_LEN - 1);
        tokens[count][MAX_TOKEN_LEN - 1] = '\0';
        count++;
        token = strtok(NULL, " \t\n\r");
    }
    return count;
}

/* ============================================================================
 *  COMMAND HANDLERS
 *  ---------------------------------------------------------------------------
 *  Each function processes one command, taking the tokenized input and
 *  dispatching to the appropriate graph/data-structure/algorithm function.
 *  Spec requirements:
 *    - Minimalist output (no prompts, only what spec requests)
 *    - Invalid command formats are ignored or return minimal output
 * ============================================================================
 */

static void handle_add_vertex(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 2) return; // Spec: ignore bad format
    const char *name = tokens[1];
    graph_add_vertex(g, name);
}

static void handle_add_edge(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 4) return;
    const char *u = tokens[1];
    const char *v = tokens[2];
    int weight = atoi(tokens[3]);
    graph_add_edge(g, u, v, weight);
}

static void handle_get_degree(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 2) return;
    const char *name = tokens[1];
    int degree = graph_get_degree(g, name);
    if (degree >= 0) {
        printf("%d\n", degree);
    }
}

static void handle_edge_exists(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 3) return;
    const char *u = tokens[1];
    const char *v = tokens[2];
    bool exists = graph_edge_exists(g, u, v);
    printf("%d\n", exists ? 1 : 0);
}

static void handle_bfs(Graph *g, Queue *scratch_queue, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 2) {
        putchar('\n'); // Per spec: print empty line for bad input
        return;
    }
    const char *start = tokens[1];
    bfs(g, start);
}

static void handle_dfs(Graph *g, Stack *scratch_stack, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 2) {
        putchar('\n'); // Per spec: print empty line for bad input
        return;
    }
    const char *start = tokens[1];
    cmd_dfs(g, start, scratch_stack);
}

static void handle_path_check(Graph *g, Stack *scratch_stack, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 3) {
        printf("0\n"); // Per spec: print 0 if format invalid (no path)
        return;
    }
    const char *src = tokens[1];
    const char *dst = tokens[2];
    cmd_path(g, src, dst, scratch_stack);
}

static void handle_mst(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    (void)tokens; (void)token_count; // Unused: only '8' triggers this
    primMST(g);
}

static void handle_shortest_path(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 3) {
        printf("0\n"); // Per spec: print 0 if bad input
        return;
    }
    const char *src = tokens[1];
    const char *dst = tokens[2];
    shortestPath(g, src, dst);
}

static void handle_print_graph(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    (void)tokens; (void)token_count; // Unused: only '10' triggers this
    graph_print(g, NULL);
}

/* ============================================================================
 *  MAIN PROGRAM LOOP
 *  ---------------------------------------------------------------------------
 *  Initializes data structures, reads and parses input, dispatches commands,
 *  and ensures safe cleanup of all memory/resources before exit.
 * ============================================================================
 */
int main(void)
{
    // --- Initialization (create all needed core data structures) ---
    Graph *graph = graph_create();
    if (!graph) {
        fprintf(stderr, "Error: Failed to create graph\n");
        return EXIT_FAILURE;
    }
    Stack *scratch_stack = stack_create(0);
    if (!scratch_stack) {
        graph_destroy(graph);
        fprintf(stderr, "Error: Failed to create stack\n");
        return EXIT_FAILURE;
    }
    Queue *scratch_queue = queue_create(0);
    if (!scratch_queue) {
        graph_destroy(graph);
        stack_destroy(scratch_stack);
        fprintf(stderr, "Error: Failed to create queue\n");
        return EXIT_FAILURE;
    }

    // --- Command processing loop (reads stdin line-by-line) ---
    char line[MAX_LINE_LEN];
    char tokens[10][MAX_TOKEN_LEN]; // Up to 10 tokens per command
    while (fgets(line, sizeof(line), stdin)) {
        trim_whitespace(line);
        // Ignore empty lines
        if (strlen(line) == 0) continue;

        // Tokenize command input
        int token_count = parse_tokens(line, tokens, 10);
        if (token_count == 0) continue;

        // Parse command code (must be integer 1–11)
        int cmd = atoi(tokens[0]);
        switch (cmd) {
            case 11:
                // Command 11: Exit (clean up and break out)
                goto cleanup;
            case 1:
                handle_add_vertex(graph, tokens, token_count);        break;
            case 2:
                handle_add_edge(graph, tokens, token_count);          break;
            case 3:
                handle_get_degree(graph, tokens, token_count);        break;
            case 4:
                handle_edge_exists(graph, tokens, token_count);       break;
            case 5:
                handle_bfs(graph, scratch_queue, tokens, token_count);break;
            case 6:
                handle_dfs(graph, scratch_stack, tokens, token_count);break;
            case 7:
                handle_path_check(graph, scratch_stack, tokens, token_count); break;
            case 8:
                handle_mst(graph, tokens, token_count);               break;
            case 9:
                handle_shortest_path(graph, tokens, token_count);     break;
            case 10:
                handle_print_graph(graph, tokens, token_count);       break;
            default:
                // Unrecognized command: ignore (per spec)
                break;
        }
    }

cleanup:
    // --- Clean up all data structures before exit ---
    graph_destroy(graph);
    stack_destroy(scratch_stack);
    queue_destroy(scratch_queue);

    return EXIT_SUCCESS;
}
