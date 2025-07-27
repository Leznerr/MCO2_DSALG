/* ============================================================================
 *  queue.c – Generic Queue implementation  
 *  ----------------------------------------------------------------------------
 *  Circular buffer-based queue with automatic resizing.
 * ==========================================================================*/

#include "queue.h"
#include <stdlib.h>
#include <stdint.h>

#define DEFAULT_CAPACITY 16

struct Queue {
    void **data;
    size_t front;
    size_t rear;
    size_t size;
    size_t capacity;
};

/* -------------------------------------------------------------------------- */
/*  LIFECYCLE                                                                 */
/* -------------------------------------------------------------------------- */
Queue *queue_create(size_t init_cap)
{
    if (init_cap == 0) init_cap = DEFAULT_CAPACITY;
    
    Queue *queue = malloc(sizeof(Queue));
    if (!queue) return NULL;
    
    queue->data = malloc(init_cap * sizeof(void*));
    if (!queue->data) {
        free(queue);
        return NULL;
    }
    
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->capacity = init_cap;
    return queue;
}

void queue_destroy(Queue *queue)
{
    if (!queue) return;
    free(queue->data);
    free(queue);
}

/* -------------------------------------------------------------------------- */
/*  HELPER FUNCTIONS                                                          */
/* -------------------------------------------------------------------------- */
static bool queue_grow(Queue *queue)
{
    size_t new_capacity = queue->capacity * 2;
    void **new_data = malloc(new_capacity * sizeof(void*));
    if (!new_data) return false;
    
    // Copy elements to new array (unwrap circular buffer)
    for (size_t i = 0; i < queue->size; i++) {
        new_data[i] = queue->data[(queue->front + i) % queue->capacity];
    }
    
    free(queue->data);
    queue->data = new_data;
    queue->front = 0;
    queue->rear = queue->size;
    queue->capacity = new_capacity;
    return true;
}

/* -------------------------------------------------------------------------- */
/*  OPERATIONS                                                                */
/* -------------------------------------------------------------------------- */
bool queue_enqueue(Queue *queue, void *data)
{
    if (!queue) return false;
    
    if (queue->size >= queue->capacity) {
        if (!queue_grow(queue)) return false;
    }
    
    queue->data[queue->rear] = data;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size++;
    return true;
}

void *queue_dequeue(Queue *queue)
{
    if (!queue || queue->size == 0) return NULL;
    
    void *data = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return data;
}

void *queue_peek(Queue *queue)
{
    if (!queue || queue->size == 0) return NULL;
    return queue->data[queue->front];
}

/* -------------------------------------------------------------------------- */
/*  QUERY                                                                     */
/* -------------------------------------------------------------------------- */
bool queue_is_empty(Queue *queue)
{
    return !queue || queue->size == 0;
}

size_t queue_size(Queue *queue)
{
    return queue ? queue->size : 0;
}
