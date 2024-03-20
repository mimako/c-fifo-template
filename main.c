#include <assert.h>
#include <stdio.h>

#include "vcan.h"
#include "vcanfd.h"
#include "xcore_vcan.h"

/* Test double inclusion */
#include "vcan.h"
#include "vcanfd.h"
#include "xcore_vcan.h"


void main(void)
{
    int ret;
    int i, j, k;
    uint32_t size;
    printf("Hello world\n");

    size = 1234;
    VCAN(vcan0, size);
    VCAN_INIT(vcan0);

    vcan_msg_t vcan_msg_tx;
    vcan_msg_tx.id = 5;
    vcan_msg_tx.length = 4;
    for (i = 0; i < vcan_msg_tx.length; i++)
        vcan_msg_tx.data[i] = i;
    vcan_write(&vcan0, &vcan_msg_tx);
    vcan_msg_t vcan_msg_rx;
    ret = vcan_read(&vcan0, &vcan_msg_rx);
    assert(ret == 0);
    assert(vcan_msg_rx.id == vcan_msg_tx.id);
    assert(vcan_msg_rx.length == vcan_msg_tx.length);
    for (i = 0; i < vcan_msg_rx.length; i++)
        assert(vcan_msg_rx.data[i] == vcan_msg_tx.data[i]);

    ret = vcan_read(&vcan0, &vcan_msg_rx);
    assert(ret == -1);
    ret = vcan_read(&vcan0, &vcan_msg_rx);
    assert(ret == -1);
    
    vcan_msg_t vcan_msg_tx1;
    vcan_msg_tx1.id = 0xDEADBEAF;
    vcan_msg_tx1.length = VCAN_MAX_LENGTH;
    vcan_msg_tx1.data[0] = 0x00;
    vcan_msg_tx1.data[1] = 0x10;
    vcan_msg_tx1.data[2] = 0x20;
    vcan_msg_tx1.data[3] = 0x30;
    vcan_msg_tx1.data[0] = 0x40;
    vcan_msg_tx1.data[1] = 0x50;
    vcan_msg_tx1.data[2] = 0x60;
    vcan_msg_tx1.data[3] = 0x70;
    vcan_write(&vcan0, &vcan_msg_tx1);
    vcan_msg_t vcan_msg_tx2;
    vcan_msg_tx2.id = 0xCAFECAFE;
    vcan_msg_tx2.length = 7;
    vcan_msg_tx2.data[0] = 0xFF;
    vcan_msg_tx2.data[1] = 0xEE;
    vcan_msg_tx2.data[2] = 0xDD;
    vcan_msg_tx2.data[3] = 0xCC;
    vcan_msg_tx2.data[0] = 0xBB;
    vcan_msg_tx2.data[1] = 0xAA;
    vcan_msg_tx2.data[2] = 0x99;
    vcan_msg_tx2.data[3] = 0x88;
    vcan_write(&vcan0, &vcan_msg_tx2);
    ret = vcan_read(&vcan0, &vcan_msg_rx);
    assert(ret == 0);
    assert(vcan_msg_rx.id == vcan_msg_tx1.id);
    assert(vcan_msg_rx.length == vcan_msg_tx1.length);
    for (i = 0; i < vcan_msg_rx.length; i++)
        assert(vcan_msg_rx.data[i] == vcan_msg_tx1.data[i]);
    ret = vcan_read(&vcan0, &vcan_msg_rx);
    assert(ret == 0);
    assert(vcan_msg_rx.id == vcan_msg_tx2.id);
    assert(vcan_msg_rx.length == vcan_msg_tx2.length);
    for (i = 0; i < vcan_msg_rx.length; i++)
        assert(vcan_msg_rx.data[i] == vcan_msg_tx2.data[i]);

    ret = vcan_read(&vcan0, &vcan_msg_rx);
    assert(ret == -1);
    ret = vcan_read(&vcan0, &vcan_msg_rx);
    assert(ret == -1);

    for (i = 0; i < size; i++)
    {
        vcan_msg_tx1.id = i;
        vcan_msg_tx1.length = i % VCAN_MAX_LENGTH;
        for (j = 0; j < VCAN_MAX_LENGTH; j++)
            vcan_msg_tx1.data[j] = i + j;
        vcan_write(&vcan0, &vcan_msg_tx1);
    }
    assert(vcan0.overflow_count == 0);
    for (i = 0; i < size; i++)
    {
        ret = vcan_read(&vcan0, &vcan_msg_rx);
        assert(ret == 0);
        assert(vcan_msg_rx.id == i);
        assert(vcan_msg_rx.length == i % VCAN_MAX_LENGTH);
        for (j = 0; j < VCAN_MAX_LENGTH; j++)
            assert(vcan_msg_rx.data[j] == (uint8_t)(i + j));
    }

    ret = vcan_read(&vcan0, &vcan_msg_rx);
    assert(ret == -1);
    ret = vcan_read(&vcan0, &vcan_msg_rx);
    assert(ret == -1);

    k = 1000000;
    for (i = 0; i < (size + k); i++)
    {
        vcan_msg_tx1.id = i;
        vcan_msg_tx1.length = i % VCAN_MAX_LENGTH;
        for (j = 0; j < VCAN_MAX_LENGTH; j++)
            vcan_msg_tx1.data[j] = i + j;
        vcan_write(&vcan0, &vcan_msg_tx1);
    }
    assert(vcan0.overflow_count == k);
    for (i = 0; i < size; i++)
    {
        ret = vcan_read(&vcan0, &vcan_msg_rx);
        assert(ret == 0);
        assert(vcan_msg_rx.id == i + k);
        assert(vcan_msg_rx.length == (uint8_t)((i + k) % VCAN_MAX_LENGTH));
        for (j = 0; j < VCAN_MAX_LENGTH; j++)
            assert(vcan_msg_rx.data[j] == (uint8_t)(i + j + k));
    }

    ret = vcan_read(&vcan0, &vcan_msg_rx);
    assert(ret == -1);
    ret = vcan_read(&vcan0, &vcan_msg_rx);
    assert(ret == -1);

    /***** VCANFD *******/
    size = 1234;
    VCANFD(vcanfd0, size);
    VCANFD_INIT(vcanfd0);

    vcanfd_msg_t vcanfd_msg_tx;
    vcanfd_msg_tx.id = 5;
    vcanfd_msg_tx.length = 4;
    for (i = 0; i < vcanfd_msg_tx.length; i++)
        vcanfd_msg_tx.data[i] = i;
    vcanfd_write(&vcanfd0, &vcanfd_msg_tx);
    vcanfd_msg_t vcanfd_msg_rx;
    ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
    assert(ret == 0);
    assert(vcanfd_msg_rx.id == vcanfd_msg_tx.id);
    assert(vcanfd_msg_rx.length == vcanfd_msg_tx.length);
    for (i = 0; i < vcanfd_msg_rx.length; i++)
        assert(vcanfd_msg_rx.data[i] == vcanfd_msg_tx.data[i]);

    ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
    assert(ret == -1);
    ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
    assert(ret == -1);
    
    vcanfd_msg_t vcanfd_msg_tx1;
    vcanfd_msg_tx1.id = 0xDEADBEAF;
    vcanfd_msg_tx1.length = 8;
    vcanfd_msg_tx1.data[0] = 0x00;
    vcanfd_msg_tx1.data[1] = 0x10;
    vcanfd_msg_tx1.data[2] = 0x20;
    vcanfd_msg_tx1.data[3] = 0x30;
    vcanfd_msg_tx1.data[0] = 0x40;
    vcanfd_msg_tx1.data[1] = 0x50;
    vcanfd_msg_tx1.data[2] = 0x60;
    vcanfd_msg_tx1.data[3] = 0x70;
    vcanfd_write(&vcanfd0, &vcanfd_msg_tx1);
    vcanfd_msg_t vcanfd_msg_tx2;
    vcanfd_msg_tx2.id = 0xCAFECAFE;
    vcanfd_msg_tx2.length = 7;
    vcanfd_msg_tx2.data[0] = 0xFF;
    vcanfd_msg_tx2.data[1] = 0xEE;
    vcanfd_msg_tx2.data[2] = 0xDD;
    vcanfd_msg_tx2.data[3] = 0xCC;
    vcanfd_msg_tx2.data[0] = 0xBB;
    vcanfd_msg_tx2.data[1] = 0xAA;
    vcanfd_msg_tx2.data[2] = 0x99;
    vcanfd_msg_tx2.data[3] = 0x88;
    vcanfd_write(&vcanfd0, &vcanfd_msg_tx2);
    ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
    assert(ret == 0);
    assert(vcanfd_msg_rx.id == vcanfd_msg_tx1.id);
    assert(vcanfd_msg_rx.length == vcanfd_msg_tx1.length);
    for (i = 0; i < vcanfd_msg_rx.length; i++)
        assert(vcanfd_msg_rx.data[i] == vcanfd_msg_tx1.data[i]);
    ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
    assert(ret == 0);
    assert(vcanfd_msg_rx.id == vcanfd_msg_tx2.id);
    assert(vcanfd_msg_rx.length == vcanfd_msg_tx2.length);
    for (i = 0; i < vcanfd_msg_rx.length; i++)
        assert(vcanfd_msg_rx.data[i] == vcanfd_msg_tx2.data[i]);

    ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
    assert(ret == -1);
    ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
    assert(ret == -1);

    for (i = 0; i < size; i++)
    {
        vcanfd_msg_tx1.id = i;
        vcanfd_msg_tx1.length = i % VCANFD_MAX_LENGTH;
        for (j = 0; j < VCANFD_MAX_LENGTH; j++)
            vcanfd_msg_tx1.data[j] = i + j;
        vcanfd_write(&vcanfd0, &vcanfd_msg_tx1);
    }
    assert(vcanfd0.overflow_count == 0);
    for (i = 0; i < size; i++)
    {
        ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
        assert(ret == 0);
        assert(vcanfd_msg_rx.id == i);
        assert(vcanfd_msg_rx.length == i % VCANFD_MAX_LENGTH);
        for (j = 0; j < VCANFD_MAX_LENGTH; j++)
            assert(vcanfd_msg_rx.data[j] == (uint8_t)(i + j));
    }

    ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
    assert(ret == -1);
    ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
    assert(ret == -1);

    k = 1000000;
    for (i = 0; i < (size + k); i++)
    {
        vcanfd_msg_tx1.id = i;
        vcanfd_msg_tx1.length = i % VCANFD_MAX_LENGTH;
        for (j = 0; j < VCANFD_MAX_LENGTH; j++)
            vcanfd_msg_tx1.data[j] = i + j;
        vcanfd_write(&vcanfd0, &vcanfd_msg_tx1);
    }
    assert(vcanfd0.overflow_count == k);
    for (i = 0; i < size; i++)
    {
        ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
        assert(ret == 0);
        assert(vcanfd_msg_rx.id == i + k);
        assert(vcanfd_msg_rx.length == (uint8_t)((i + k) % VCANFD_MAX_LENGTH));
        for (j = 0; j < VCANFD_MAX_LENGTH; j++)
            assert(vcanfd_msg_rx.data[j] == (uint8_t)(i + j + k));
    }

    ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
    assert(ret == -1);
    ret = vcanfd_read(&vcanfd0, &vcanfd_msg_rx);
    assert(ret == -1);

    /***** XCORE_VCAN *******/
    size = 32;
    XCORE_VCAN(xcore_vcan0, size);
    XCORE_VCAN_INIT(xcore_vcan0);

    vcan_msg_tx.id = 5;
    vcan_msg_tx.length = 4;
    for (i = 0; i < vcan_msg_tx.length; i++)
        vcan_msg_tx.data[i] = i;
    xcore_vcan_write(&xcore_vcan0, &vcan_msg_tx);
    vcan_msg_rx;
    ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
    assert(ret == 0);
    assert(vcan_msg_rx.id == vcan_msg_tx.id);
    assert(vcan_msg_rx.length == vcan_msg_tx.length);
    for (i = 0; i < vcan_msg_rx.length; i++)
        assert(vcan_msg_rx.data[i] == vcan_msg_tx.data[i]);

    ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
    assert(ret == -1);
    ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
    assert(ret == -1);
    
    vcan_msg_tx1;
    vcan_msg_tx1.id = 0xDEADBEAF;
    vcan_msg_tx1.length = 8;
    vcan_msg_tx1.data[0] = 0x00;
    vcan_msg_tx1.data[1] = 0x10;
    vcan_msg_tx1.data[2] = 0x20;
    vcan_msg_tx1.data[3] = 0x30;
    vcan_msg_tx1.data[0] = 0x40;
    vcan_msg_tx1.data[1] = 0x50;
    vcan_msg_tx1.data[2] = 0x60;
    vcan_msg_tx1.data[3] = 0x70;
    xcore_vcan_write(&xcore_vcan0, &vcan_msg_tx1);
    vcan_msg_tx2;
    vcan_msg_tx2.id = 0xCAFECAFE;
    vcan_msg_tx2.length = 7;
    vcan_msg_tx2.data[0] = 0xFF;
    vcan_msg_tx2.data[1] = 0xEE;
    vcan_msg_tx2.data[2] = 0xDD;
    vcan_msg_tx2.data[3] = 0xCC;
    vcan_msg_tx2.data[0] = 0xBB;
    vcan_msg_tx2.data[1] = 0xAA;
    vcan_msg_tx2.data[2] = 0x99;
    vcan_msg_tx2.data[3] = 0x88;
    xcore_vcan_write(&xcore_vcan0, &vcan_msg_tx2);
    ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
    assert(ret == 0);
    assert(vcan_msg_rx.id == vcan_msg_tx1.id);
    assert(vcan_msg_rx.length == vcan_msg_tx1.length);
    for (i = 0; i < vcan_msg_rx.length; i++)
        assert(vcan_msg_rx.data[i] == vcan_msg_tx1.data[i]);
    ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
    assert(ret == 0);
    assert(vcan_msg_rx.id == vcan_msg_tx2.id);
    assert(vcan_msg_rx.length == vcan_msg_tx2.length);
    for (i = 0; i < vcan_msg_rx.length; i++)
        assert(vcan_msg_rx.data[i] == vcan_msg_tx2.data[i]);

    ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
    assert(ret == -1);
    ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
    assert(ret == -1);

    for (i = 0; i < size; i++)
    {
        vcan_msg_tx1.id = i;
        vcan_msg_tx1.length = i % VCAN_MAX_LENGTH;
        for (j = 0; j < VCAN_MAX_LENGTH; j++)
            vcan_msg_tx1.data[j] = i + j;
        xcore_vcan_write(&xcore_vcan0, &vcan_msg_tx1);
    }
    assert(xcore_vcan0.fifo.overflow_count == 0);
    for (i = 0; i < size; i++)
    {
        ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
        assert(ret == 0);
        assert(vcan_msg_rx.id == i);
        assert(vcan_msg_rx.length == i % VCAN_MAX_LENGTH);
        for (j = 0; j < VCAN_MAX_LENGTH; j++)
            assert(vcan_msg_rx.data[j] == (uint8_t)(i + j));
    }

    ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
    assert(ret == -1);
    ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
    assert(ret == -1);

    k = 1000000;
    for (i = 0; i < (size + k); i++)
    {
        vcan_msg_tx1.id = i;
        vcan_msg_tx1.length = i % VCAN_MAX_LENGTH;
        for (j = 0; j < VCAN_MAX_LENGTH; j++)
            vcan_msg_tx1.data[j] = i + j;
        xcore_vcan_write(&xcore_vcan0, &vcan_msg_tx1);
    }
    assert(xcore_vcan0.fifo.overflow_count == k);
    for (i = 0; i < size; i++)
    {
        ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
        assert(ret == 0);
        assert(vcan_msg_rx.id == i + k);
        assert(vcan_msg_rx.length == (uint8_t)((i + k) % VCAN_MAX_LENGTH));
        for (j = 0; j < VCAN_MAX_LENGTH; j++)
            assert(vcan_msg_rx.data[j] == (uint8_t)(i + j + k));
    }

    ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
    assert(ret == -1);
    ret = xcore_vcan_read(&xcore_vcan0, &vcan_msg_rx);
    assert(ret == -1);

    size = 32;
    XCORE_VCAN(xcore_vcan1, size);
    XCORE_VCAN_INIT(xcore_vcan1);
    vcan_msg_tx.id = 5;
    vcan_msg_tx.length = 4;
    for (i = 0; i < vcan_msg_tx.length; i++)
        vcan_msg_tx.data[i] = i;
    xcore_vcan_write(&xcore_vcan0, &vcan_msg_tx);
    xcore_vcan_echo(&xcore_vcan0, &xcore_vcan1);
    xcore_vcan_echo(&xcore_vcan0, &xcore_vcan1);
    ret = xcore_vcan_read(&xcore_vcan1, &vcan_msg_rx);
    assert(ret == 0);
    assert(vcan_msg_rx.id == vcan_msg_tx.id);
    assert(vcan_msg_rx.length == vcan_msg_tx.length);
    for (i = 0; i < vcan_msg_rx.length; i++)
        assert(vcan_msg_rx.data[i] == vcan_msg_tx.data[i]);
    ret = xcore_vcan_read(&xcore_vcan1, &vcan_msg_rx);
    assert(ret == -1);

    xcore_vcan_test(&xcore_vcan0, &xcore_vcan1, size + 1, size);

    printf("Success\n");
}