#ifndef XCORE_VCAN_H_
#define XCORE_VCAN_H_

#include "vcan.h"

typedef struct {
    fifo_vcan_msg_t fifo;
    int mutex;
} xcore_vcan_t;


#define XCORE_VCAN(name, size) \
extern xcore_vcan_t name; \
extern vcan_msg_t name##_buffer[size];

#define XCORE_VCAN_INIT(name) \
do \
{ \
    fifo_vcan_msg_t_init(&(name.fifo), name##_buffer, sizeof(name##_buffer) / sizeof(name##_buffer[0]), NULL, NULL); \
    name.mutex = 0; \
} while (0);

extern void xcore_vcan_write(xcore_vcan_t *vcan, vcan_msg_t *msg);
extern int xcore_vcan_read(xcore_vcan_t *vcan, vcan_msg_t *msg);

extern void xcore_vcan_echo(xcore_vcan_t *vcan_rx,  xcore_vcan_t *vcan_tx);
extern void xcore_vcan_test(xcore_vcan_t *vcan_tx,  xcore_vcan_t *vcan_rx, uint32_t nb, uint32_t burst);

#endif