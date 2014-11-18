/*
 * Copyright (c) 2014 Jumpnow Technologies, LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <dev/iicbus/iic.h>

#define I2C_BUS "/dev/iic2"
#define EEPROM_ADDRESS 0x51

int open_device()
{
    int fd = open(I2C_BUS, O_RDWR);

    if (fd < 0) {
        perror("open");
        return -1;
    }

    return fd;
}

int read_eeprom(int address)
{
    int fd, ret;
    uint8_t wbuf[2];
    uint8_t rbuf[2];
    struct iic_msg msg[2];
    struct iic_rdwr_data rdwr;

    if ((fd = open_device()) < 0)
        return -1;

    msg[0].slave = EEPROM_ADDRESS << 1;
    msg[0].flags = 0;
    msg[0].len = 1;
    wbuf[0] = 0xff & address;
    msg[0].buf = wbuf;

    rbuf[0] = 0;
    rbuf[1] = 0;

    msg[1].slave = (EEPROM_ADDRESS << 1) | IIC_M_RD;
    msg[1].flags = IIC_M_RD;
    msg[1].len = 2;
    msg[1].buf = rbuf;

    rdwr.msgs = msg;
    rdwr.nmsgs = 2;

    ret = ioctl(fd, I2CRDWR, &rdwr);

    if (ret < 0) {
        perror("ioctl(I2CRDWR)");
        ret = -1;
    }
    else {
        ret = (rbuf[0] << 8) + rbuf[1];
    }

    close(fd);

    return ret;
}

int write_eeprom(int address, int val)
{
    int fd, ret;
    uint8_t buf[4];
    struct iic_msg msg;
    struct iic_rdwr_data rdwr;

    if ((fd = open_device()) < 0)
        return -1;

    msg.slave = EEPROM_ADDRESS << 1;
    msg.flags = 0;
    msg.len = 3;
    buf[0] = 0xff & address;
    buf[1] = (val >> 8) & 0xff; 
    buf[2] = val & 0xff;
    msg.buf = buf;

    rdwr.msgs = &msg;
    rdwr.nmsgs = 1;

    ret = ioctl(fd, I2CRDWR, &rdwr);

    if (ret < 0)
        perror("ioctl(I2CRDWR)");

    close(fd);

    return ret;
}















 
