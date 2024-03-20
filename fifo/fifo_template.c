#ifndef FIFO_TEMPLATE_C_
#define FIFO_TEMPLATE_C_

#include <stdint.h>
#include <stddef.h>

#define FIFO_C(type) \
 \
void fifo_##type##_init(fifo_##type *fifo, type *buffer, uint32_t size, \
                void (*enter_critical)(void), \
                void (*exit_critical)(void)) \
{ \
    fifo->head = 0; \
    fifo->tail = 0; \
    fifo->count = 0; \
    fifo->size = size; \
    fifo->overflow_count = 0; \
    fifo->enter_critical = enter_critical; \
    fifo->exit_critical = exit_critical; \
    fifo->buffer = buffer; \
} \
 \
void fifo_##type##_push(fifo_##type *fifo, type *elem) \
{ \
    if (NULL != fifo->enter_critical) \
        fifo->enter_critical(); \
 \
    fifo->buffer[fifo->head] = *elem; \
    fifo->head++; \
    if (fifo->head >= fifo->size) \
        fifo->head = 0; \
    fifo->count++; \
 \
    if (fifo->count > fifo->size) \
    { \
        fifo->count = fifo->size; \
        fifo->overflow_count++; \
        fifo->tail++; \
        if (fifo->tail >= fifo->size) \
            fifo->tail = 0; \
    } \
 \
    if (NULL != fifo->exit_critical) \
        fifo->exit_critical(); \
} \
 \
int fifo_##type##_pop(fifo_##type *fifo, type *elem) \
{ \
    int ret; \
 \
    if (NULL != fifo->enter_critical) \
        fifo->enter_critical(); \
 \
    if (fifo->count > 0) { \
        *elem = fifo->buffer[fifo->tail]; \
        fifo->tail++; \
        if (fifo->tail >= fifo->size) \
            fifo->tail = 0; \
        fifo->count--; \
        ret = 0; \
    } \
    else \
        ret = -1; \
 \
    if (NULL != fifo->exit_critical) \
        fifo->exit_critical(); \
 \
    return ret; \
} \
 \
uint32_t fifo_##type##_get_space_to_end(fifo_##type *fifo) \
{ \
    return (fifo->size - fifo->head); \
} \
 \
uint32_t fifo_##type##_get_count_to_end(fifo_##type *fifo) \
{ \
    if (fifo->head >= fifo->tail) \
        return (fifo->head - fifo->tail); \
    else \
        return (fifo->size - fifo->tail); \
} \
 \
void fifo_##type##_move_head(fifo_##type *fifo, uint32_t nb) \
{ \
    uint32_t space = fifo->size - fifo->count; \
 \
    fifo->head += nb; \
    if (fifo->head >= fifo->size) \
        fifo->head = 0; \
    if (nb < space) \
    { \
        fifo->count += nb; \
    } \
    else \
    { \
        fifo->tail = fifo->head; \
        fifo->count = fifo->size; \
    } \
} \
 \
void fifo_##type##_move_tail(fifo_##type *fifo, uint32_t nb) \
{ \
    fifo->count -= nb; \
    fifo->tail += nb; \
    if (fifo->tail >= fifo->size) \
        fifo->tail = 0; \
}

#endif