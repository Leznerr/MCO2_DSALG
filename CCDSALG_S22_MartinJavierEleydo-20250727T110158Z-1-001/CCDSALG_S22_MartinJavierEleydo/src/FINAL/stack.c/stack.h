/* ============================================================================
 *  stack.h – Generic Stack Interface for CCDSALG MCO-2
 * ----------------------------------------------------------------------------
 *  Public API for a dynamically-growing, array-based stack supporting any data
 *  type via void* pointers. Used by DFS, path checking, and other algorithms
 *  that require Last-In-First-Out (LIFO) behavior.
 *
 *  Features & Patterns:
 *      ✔ Generic payloads (void*) for max flexibility (any struct or pointer type)
 *      ✔ O(1) amortized push/pop (resizing handled automatically)
 *      ✔ Robust memory management (never leaks; never frees user data)
 *      ✔ Memory-safe (all functions return false/NULL on failure)
 *      ✔ Foundation for many DSAL classic problems (balancing, DFS, undo, etc.)
 * ============================================================================
 */

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*  OPAQUE STACK TYPE: Implementation details hidden from users               */
/* -------------------------------------------------------------------------- */
typedef struct Stack Stack;

/* -------------------------------------------------------------------------- */
/*  LIFECYCLE                                                                 */
/* -------------------------------------------------------------------------- */
/**
 * Create a new empty stack.
 * @param init_cap  Initial capacity (array will grow if exceeded; 0 for default)
 * @return          Pointer to a new Stack, or NULL on allocation failure.
 */
Stack *stack_create(size_t init_cap);

/**
 * Destroy the stack and free all internal memory.
 * Note: Does NOT free the pointers stored in the stack (user owns those).
 */
void stack_destroy(Stack *stack);

/* -------------------------------------------------------------------------- */
/*  CORE STACK OPERATIONS                                                     */
/* -------------------------------------------------------------------------- */
/**
 * Push data onto the top of the stack.
 * O(1) amortized. Grows dynamically if needed.
 * @return true on success, false if allocation fails or stack is NULL.
 */
bool stack_push(Stack *stack, void *data);

/**
 * Pop and return the top element.
 * @return Pointer to the data from the top of the stack, or NULL if empty/invalid.
 */
void *stack_pop(Stack *stack);

/**
 * Peek at the top element without removing it.
 * @return Pointer to the data at the top, or NULL if empty/invalid.
 */
void *stack_peek(Stack *stack);

/* -------------------------------------------------------------------------- */
/*  QUERY OPERATIONS                                                          */
/* -------------------------------------------------------------------------- */
/**
 * Check if the stack is empty.
 * @return true if empty or NULL, false otherwise.
 */
bool stack_is_empty(Stack *stack);

/**
 * Get the number of elements in the stack.
 * @return Number of elements, or 0 if stack is NULL.
 */
size_t stack_size(Stack *stack);

#ifdef __cplusplus
}
#endif

#endif /* STACK_H */
