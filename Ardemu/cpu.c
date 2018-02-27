//
//  cpu.c
//  Ardemu
//
//  Created by Jimmy Lipham on 2/26/18.
//  Copyright © 2018 86pixels. All rights reserved.
//

#include "cpu.h"
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#include <stdio.h>

typedef void (*Handler)(uint16_t instr);

#define MAX_POLL_FUNCTIONS  16
#define MAX_IRQ             27

typedef struct {
    union {
        struct {
            union {
                uint8_t Reg[32];
                struct {
                    uint8_t dummy1[24];
                    union {
                        uint16_t RegW[24];
                        struct {
                            uint16_t r24;
                            uint16_t X;
                            uint16_t Y;
                            uint16_t Z;
                        };
                    };
                };
            };
            uint8_t dummy2[0x5d-0x20];
            uint16_t SP __attribute__((packed));
            union {
                struct {
                    char C: 1;
                    char Z: 1;
                    char N: 1;
                    char V: 1;
                    char S: 1;
                    char H: 1;
                    char T: 1;
                    char I: 1;
                };
                uint8_t bits;
            } SREG;
        };
        uint8_t _Bytes[DATA_SIZE_BYTES];
    };
} TData;

static uint8_t ioread(uint16_t addr);
static void iowrite(uint16_t addr, uint8_t value);

uint16_t Program[PROGRAM_SIZE_WORDS];
TData Data;
ReadFunction IORead[0x100];
WriteFunction IOWrite[0x100];
PollFunction PollFunctions[MAX_POLL_FUNCTIONS];
int PollFunctionCount;
int PendingIRQ[MAX_IRQ];
int PendingIRQCount;
int State;
uint16_t PC;
uint32_t Cycle;
uint32_t LastPoll;
PinFunction PinCallback[PIN_COUNT];

//COMPILE_ASSERT(sizeof(Data.SREG) == 1);
//COMPILE_ASSERT(((uint8_t *)&Data.SP) - Data._Bytes == 0x5d);
//COMPILE_ASSERT(((uint8_t *)&Data.SREG) - Data._Bytes == 0x5f);

static uint8_t read(uint16_t addr)
{
    if ((addr & 0xff00) == 0) {
        return ioread(addr);
    } else {
        return Data._Bytes[addr];
    }
}

