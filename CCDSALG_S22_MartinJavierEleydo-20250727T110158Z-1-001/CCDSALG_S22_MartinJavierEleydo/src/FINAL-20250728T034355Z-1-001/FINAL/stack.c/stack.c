/* ============================================================================
 *  stack.c – Generic Stack implementation
 *  ----------------------------------------------------------------------------
 *  Dynamic array-based stack with automatic resizing.
 * ==========================================================================*/

#include "stack.h"
#include <stdlib.h>
#include <stdint.h>

#define DEFAULT_CAPACITY 16

struct Stack {
    void **data;
    size_t size;
    size_t capacity;
};

/* -------------------------------------------------------------------------- */
/*  LIFECYCLE                                                                 */
/* -------------------------------------------------------------------------- */
Stack *stack_create(size_t init_cap)
{
    if (init_cap == 0) init_cap = DEFAULT_CAPACITY;
    
    Stack *stack = malloc(sizeof(Stack));
    if (!stack) return NULL;
    
    stack->data = malloc(init_cap * sizeof(void*));
    if (!stack->data) {
        free(stack);
        return NULL;
    }
    
    stack->size = 0;
    stack->capacity = init_cap;
    return stack;
}

void stack_destroy(Stack *stack)
{
    if (!stack) return;
    free(stack->data);
    free(stack);
}

/* -------------------------------------------------------------------------- */
/*  HELPER FUNCTIONS                                                          */
/* -------------------------------------------------------------------------- */
static bool stack_grow(Stack *stack)
{
    size_t new_capacity = stack->capacity * 2;
    void **new_data = realloc(stack->data, new_capacity * sizeof(void*));
    if (!new_data) return false;
    
    stack->data = new_data;
    stack->capacity = new_capacity;
    return true;
}

/* -------------------------------------------------------------------------- */
/*  OPERATIONS                                                                */
/* -------------------------------------------------------------------------- */
bool stack_push(Stack *stack, void *data)
{
    if (!stack) return false;
    
    if (stack->size >= stack->capacity) {
        if (!stack_grow(stack)) return false;
    }
    
    stack->data[stack->size++] = data;
    return true;
}

void *stack_pop(Stack *stack)
{
    if (!stack || stack->size == 0) return NULL;
    return stack->data[--stack->size];
}

void *stack_peek(Stack *stack)
{
    if (!stack || stack->size == 0) return NULL;
    return stack->data[stack->size - 1];
}

/* -------------------------------------------------------------------------- */
/*  QUERY                                                                     */
/* -------------------------------------------------------------------------- */
bool stack_is_empty(Stack *stack)
{
    return !stack || stack->size == 0;
}

size_t stack_size(Stack *stack)
{
    return stack ? stack->size : 0;
}
