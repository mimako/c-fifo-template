#include <assert.h>

#include "xcore_vcan.h"

void xcore_vcan_write(xcore_vcan_t *vcan, vcan_msg_t *msg)
{
    vcan->mutex = 1;
    vcan_write(&(vcan->fifo), msg);
    vcan->mutex = 0;
}


int xcore_vcan_read(xcore_vcan_t *vcan, vcan_msg_t *msg)
{
    int ret;

    vcan->mutex = 1;
    ret = vcan_read(&(vcan->fifo), msg);
    vcan->mutex = 0;

    return ret;
}

void xcore_vcan_echo(xcore_vcan_t *vcan_rx,  xcore_vcan_t *vcan_tx)
{
    vcan_msg_t msg;
    if (0 == xcore_vcan_read(vcan_rx, &msg))
        xcore_vcan_write(vcan_tx, &msg);
}

void xcore_vcan_test(xcore_vcan_t *vcan_tx,  xcore_vcan_t *vcan_rx, uint32_t nb, uint32_t burst)
{
    int i, j;
    uint32_t nb_tx = 0;
    uint32_t nb_rx = 0;
    vcan_msg_t msg;

    while ((nb_tx < nb) || (nb_rx < nb))
    {
        for (j = 0; j < burst; j++)
        {
            if (nb_tx < nb)
            {
                msg.id = nb_tx;
                msg.length = nb_tx % VCAN_MAX_LENGTH;
                for (i = 0; i < msg.length; i++)
                    msg.data[i] = (uint8_t)nb_tx + i;
                xcore_vcan_write(vcan_tx, &msg);
                nb_tx++;
            }
        }

        //xcore_vcan_echo(vcan_tx, vcan_rx);
        
        if (0 == xcore_vcan_read(vcan_rx, &msg))
        {
            assert(msg.id == nb_rx);
            assert(msg.length == nb_rx % VCAN_MAX_LENGTH);
            for (i = 0; i < msg.length; i++)
                assert(msg.data[i] == (uint8_t)nb_rx + i);
            nb_rx++; 
        }
    }
}