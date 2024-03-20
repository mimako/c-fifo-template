#ifndef VCANFD_H_
#define VCANFD_H_

#include <stdint.h>

#define VCANFD_MAX_LENGTH 64

typedef struct {
    uint32_t id;
    uint8_t length;
    uint8_t data[64];
} vcanfd_msg_t;

#include "fifo_template.h"
FIFO_H(vcanfd_msg_t)
typedef fifo_vcanfd_msg_t vcanfd_t;

#define VCANFD(name, size) \
vcanfd_t name; \
vcanfd_msg_t name##_buffer[size];

#define VCANFD_INIT(name) fifo_vcanfd_msg_t_init(&name, name##_buffer, sizeof(name##_buffer) / sizeof(vcanfd_msg_t), NULL, NULL)
#define vcanfd_write fifo_vcanfd_msg_t_push
#define vcanfd_read fifo_vcanfd_msg_t_pop

#endif