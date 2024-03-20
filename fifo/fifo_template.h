#ifndef FIFO_TEMPLATE_H_
#define FIFO_TEMPLATE_H_

#include <stdint.h>

#define FIFO_H(type) \
typedef struct { \
    uint32_t head; \
    uint32_t tail; \
    uint32_t count; \
    uint32_t size; \
    uint32_t overflow_count; \
    void (*enter_critical)(void); \
    void (*exit_critical)(void); \
    type *buffer; \
} fifo_##type; \
 \
extern void fifo_##type##_init(fifo_##type *fifo, type *buffer, uint32_t size, \
                void (*enter_critical)(void), \
                void (*exit_critical)(void)); \
 \
extern void fifo_##type##_push(fifo_##type *fifo, type *elem); \
extern int fifo_##type##_pop(fifo_##type *fifo, type *elem); \
 \
extern uint32_t fifo_##type##_get_space_to_end(fifo_##type *fifo); \
extern uint32_t fifo_##type##_get_count_to_end(fifo_##type *fifo); \
extern void fifo_##type##_move_head(fifo_##type *fifo, uint32_t nb); \
extern void fifo_##type##_move_tail(fifo_##type *fifo, uint32_t nb);

#endif

/// @brief Helper to declare a new fifo
#define FIFO(name, type, size) \
fifo_##type name; \
type name##_buffer[size]

/// @brief Helper to init a fifo
#define FIFO_INIT(name) \
do \
{ \
    fifo_vcan_msg_t_init(&name, name##_buffer, sizeof(name##_buffer) / sizeof(name##_buffer[0]), NULL, NULL); \
} while (0);
