/* ============================================================================
 *  stack.h – Generic Stack for CCDSALG MCO-2
 *  ----------------------------------------------------------------------------
 *  Provides a generic stack with void* payloads for maximum flexibility.
 *  Used by DFS and path checking algorithms.
 *
 *  Features:
 *      • Generic void* data storage
 *      • O(1) push/pop operations
 *      • Dynamic growth
 *      • Memory-safe operations
 * ==========================================================================*/

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  OPAQUE TYPE                                                               */
/* -------------------------------------------------------------------------- */
typedef struct Stack Stack;

/* -------------------------------------------------------------------------- */
/*  LIFECYCLE                                                                 */
/* -------------------------------------------------------------------------- */
/**
 * Create a new empty stack.
 * @param init_cap Initial capacity (will default to reasonable size if 0)
 * @return New Stack instance, or NULL on allocation failure
 */
Stack *stack_create(size_t init_cap);

/**
 * Destroy the stack and free all memory.
 * Does not free the data pointers stored in the stack.
 */
void stack_destroy(Stack *stack);

/* -------------------------------------------------------------------------- */
/*  OPERATIONS                                                                */
/* -------------------------------------------------------------------------- */
/**
 * Push data onto the top of the stack (O(1) amortized).
 * @return true on success, false on allocation failure
 */
bool stack_push(Stack *stack, void *data);

/**
 * Pop and return the top element (O(1)).
 * @return The data from the top of the stack, or NULL if empty
 */
void *stack_pop(Stack *stack);

/**
 * Peek at the top element without removing it (O(1)).
 * @return The data from the top of the stack, or NULL if empty
 */
void *stack_peek(Stack *stack);

/* -------------------------------------------------------------------------- */
/*  QUERY                                                                     */
/* -------------------------------------------------------------------------- */
/**
 * Check if the stack is empty.
 */
bool stack_is_empty(Stack *stack);

/**
 * Get the number of elements in the stack.
 */
size_t stack_size(Stack *stack);

#ifdef __cplusplus
}
#endif

#endif /* STACK_H */
