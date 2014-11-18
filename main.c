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
 * Read and write the Gumstix Duovero Parlor expansion board EEPROM attached
 * to I2C3 at address 0x51.
 *
 * The device is an Atmel AT24CA part. I'm unclear on how to address more 
 * then 256 bytes of the thing. Not even sure what the size is.
 *
 * I'm treating it as 128 16-byte words.
 * 
 * If I ever need more then that, I'll go back and figure out how to
 * address the thing.
 *
 * Works at 100 kHz and 400 kHz.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "duovero_eeprom.h"

int get_int(char *s, int max);

void usage(char *argv_0)
{
    printf("\nUsage: %s [-h] <address> [<value>]\n", argv_0);
    printf("  address     Address of 16-bit word, range 0-127\n");
    printf("  value       Value to write, range 0-65535\n");
    printf("  -h          Show this help\n\n");
}

int main(int argc, char **argv)
{
    int address, value;

    if (argc < 2) {
        usage(argv[0]);
        exit(1);
    }

    address = get_int(argv[1], 256);

    if (address < 0) {
        printf("Invalid start address: %s\n", argv[1]);
        usage(argv[0]);
        exit(1);
    }

    // treating the eeprom as storage for 16-bit words
    address *= 2;

    if (argc == 3) {
        value = get_int(argv[2], 65535);

        if (value < 0) {
            printf("Invalid word value to write: %s\n", argv[2]);
            usage(argv[0]);
            exit(1);
        }

        if (write_eeprom(address, value) < 0)
            printf("Error writing to eeprom\n"); 
    }
    else {
        value = read_eeprom(address);

        if (value < 0)
            printf("Error reading eeprom\n");
        else
            printf("eeprom[%d]: %d\n", address / 2, value);
    }

    return 0;
}

int get_int(char *s, int max)
{
    int val;
    char *endp;

    errno = 0;

    val = strtol(s, &endp, 0);

    if (errno) {
        perror("strtol");
        return -1;
    }

    if (endp == s) {
        printf("No digits found in %s\n", s);
        return -1;
    }

    if (val < 0 || val > max)
        return -1;

    return val;
}

