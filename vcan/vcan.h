#ifndef VCAN_H_
#define VCAN_H_

#include <stdint.h>

#define VCAN_MAX_LENGTH 8

typedef struct {
    uint32_t id;
    uint8_t length;
    uint8_t data[8];
} vcan_msg_t;

#include "fifo_template.h"
FIFO_H(vcan_msg_t)

typedef fifo_vcan_msg_t vcan_t;

#define VCAN(name, size) FIFO(name, vcan_msg_t, size)

#define VCAN_INIT(name) FIFO_INIT(name)

#define vcan_write fifo_vcan_msg_t_push
#define vcan_read fifo_vcan_msg_t_pop

#endif