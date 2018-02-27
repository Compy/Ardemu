//
//  cpu.h
//  Ardemu
//
//  Created by Jimmy Lipham on 2/26/18.
//  Copyright © 2018 86pixels. All rights reserved.
//

#ifndef cpu_h
#define cpu_h

#include <ctype.h>
#include "util.h"

#define PROGRAM_SIZE_WORDS  0x10000
#define DATA_SIZE_BYTES     0x900

#define PIN_PORTB           0
#define PIN_PORTC           8
#define PIN_PORTD           16
#define PIN_COUNT           (PIN_PORTD + 8)

#define CPU_RUN             0
#define CPU_HALT            1

typedef uint8_t (*ReadFunction)(uint16_t addr);
typedef void    (*WriteFunction)(uint16_t addr, uint8_t value);
typedef void    (*PollFunction)();
typedef void    (*PinFunction)(int pin, bool state);

void irq(int n);
void register_io(uint16_t addr, ReadFunction rf, WriteFunction wf);
void register_poll(PollFunction pf);
void out_pin(int pin, bool state);

void cpu_init();
void cpu_load_flash(uint8_t *buf, uint32_t bufsize);
void cpu_load_eeprom(uint8_t *buf, uint32_t bufsize);
void cpu_reset();
int cpu_run();
void cpu_set_pin(int pin, bool state);
void cpu_pin_callback(int pin, PinFunction f);
uint32_t cpu_get_cycles();

#endif /* cpu_h */
