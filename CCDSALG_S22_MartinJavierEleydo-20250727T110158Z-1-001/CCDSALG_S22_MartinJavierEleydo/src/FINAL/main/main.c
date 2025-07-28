/* ============================================================================
 *  Implements the command-line interface for all graph operations.
 *  Parses input commands and dispatches to appropriate modules.
 *
 *  Commands supported:
 *    1  <name>           - Add vertex
 *    2  <u> <v> <weight> - Add edge (undirected, weighted)
 *    3  <name>           - Get vertex degree
 *    4  <u> <v>          - Check if edge exists
 *    5  <start>          - BFS traversal
 *    6  <start>          - DFS traversal  
 *    7  <src> <dst>      - Check path connectivity
 *    8                   - Find MST
 *    9  <src> <dst>      - Find shortest path
 *    10                  - Print graph
 *    11                   - Exit
 * ==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Core data structures
#include "graph.h"
#include "stack.h"
#include "queue.h"
#include "heap.h"

// Algorithm modules
#include "dfs.h"
#include "bfs.h"
#include "path_check.h"
#include "mst.h"
#include "shortest_Path.h"

#define MAX_LINE_LEN 1024
#define MAX_TOKEN_LEN 257

/* -------------------------------------------------------------------------- */
/*  INPUT PARSING HELPERS                                                     */
/* -------------------------------------------------------------------------- */

/**
 * Trim whitespace from both ends of a string in-place
 */
static void trim_whitespace(char *str)
{
    if (!str) return;
    
    // Trim leading whitespace
    char *start = str;
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r') {
        start++;
    }
    
    // Trim trailing whitespace
    char *end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }
    
    // Move trimmed string to beginning if needed
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

/**
 * Parse a line into tokens separated by whitespace
 * Returns number of tokens found
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

/* -------------------------------------------------------------------------- */
/*  COMMAND HANDLERS                                                          */
/* -------------------------------------------------------------------------- */

static void handle_add_vertex(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 2) {
        return; // Invalid format - just ignore per spec
    }
    
    const char *name = tokens[1];
    graph_add_vertex(g, name);
}

static void handle_add_edge(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 4) {
        return; // Invalid format - just ignore per spec
    }
    
    const char *u = tokens[1];
    const char *v = tokens[2];
    int weight = atoi(tokens[3]);
    
    graph_add_edge(g, u, v, weight);
}

static void handle_get_degree(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 2) {
        return; // Invalid format - just ignore per spec
    }
    
    const char *name = tokens[1];
    int degree = graph_get_degree(g, name);
    
    if (degree >= 0) {
        printf("%d\n", degree);
    }
}

static void handle_edge_exists(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 3) {
        return; // Invalid format - just ignore per spec
    }
    
    const char *u = tokens[1];
    const char *v = tokens[2];
    bool exists = graph_edge_exists(g, u, v);
    
    printf("%d\n", exists ? 1 : 0);
}

static void handle_bfs(Graph *g, Queue *scratch_queue, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 2) {
        putchar('\n'); // Invalid format - print empty line
        return;
    }
    
    const char *start = tokens[1];
    bfs(g, start);
}

static void handle_dfs(Graph *g, Stack *scratch_stack, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 2) {
        putchar('\n'); // Invalid format - print empty line
        return;
    }
    
    const char *start = tokens[1];
    cmd_dfs(g, start, scratch_stack);
}

static void handle_path_check(Graph *g, Stack *scratch_stack, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 3) {
        printf("0\n"); // Invalid format - no path
        return;
    }
    
    const char *src = tokens[1];
    const char *dst = tokens[2];
    cmd_path(g, src, dst, scratch_stack);
}

static void handle_mst(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    (void)tokens; (void)token_count; // Unused parameters
    primMST(g);
}

static void handle_shortest_path(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    if (token_count != 3) {
        printf("0\n"); // Invalid format - no path
        return;
    }
    
    const char *src = tokens[1];
    const char *dst = tokens[2];
    shortestPath(g, src, dst);
}

static void handle_print_graph(Graph *g, char tokens[][MAX_TOKEN_LEN], int token_count)
{
    (void)tokens; (void)token_count; // Unused parameters
    graph_print(g, NULL);
}

/* -------------------------------------------------------------------------- */
/*  MAIN PROGRAM                                                              */
/* -------------------------------------------------------------------------- */

int main(void)
{
    // Initialize core data structures
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
    
    // Main command processing loop
    char line[MAX_LINE_LEN];
    char tokens[10][MAX_TOKEN_LEN]; // Max 10 tokens per command
    
    while (fgets(line, sizeof(line), stdin)) {
        trim_whitespace(line);
        
        // Skip empty lines
        if (strlen(line) == 0) {
            continue;
        }
        
        // Parse command tokens
        int token_count = parse_tokens(line, tokens, 10);
        if (token_count == 0) {
            continue;
        }
        
        // Get command number
        int cmd = atoi(tokens[0]);
        
        // Dispatch to appropriate handler
        switch (cmd) {
            case 11:
                // Exit program
                goto cleanup;
                
            case 1:
                handle_add_vertex(graph, tokens, token_count);
                break;
                
            case 2:
                handle_add_edge(graph, tokens, token_count);
                break;
                
            case 3:
                handle_get_degree(graph, tokens, token_count);
                break;
                
            case 4:
                handle_edge_exists(graph, tokens, token_count);
                break;
                
            case 5:
                handle_bfs(graph, scratch_queue, tokens, token_count);
                break;
                
            case 6:
                handle_dfs(graph, scratch_stack, tokens, token_count);
                break;
                
            case 7:
                handle_path_check(graph, scratch_stack, tokens, token_count);
                break;
                
            case 8:
                handle_mst(graph, tokens, token_count);
                break;
                
            case 9:
                handle_shortest_path(graph, tokens, token_count);
                break;
                
            case 10:
                handle_print_graph(graph, tokens, token_count);
                break;
                
            default:
                // Unknown command - just ignore per spec
                break;
        }
    }
    
cleanup:
    // Clean up resources
    graph_destroy(graph);
    stack_destroy(scratch_stack);
    queue_destroy(scratch_queue);
    
    return EXIT_SUCCESS;
}
