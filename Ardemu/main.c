//
//  main.c
//  Ardemu
//
//  Created by Jimmy Lipham on 2/26/18.
//  Copyright © 2018 86pixels. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "loader.h"

int main(int argc, const char * argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <arduino hex file>\n", argv[0]);
    }
    
    uint8_t program[PROGRAM_SIZE_WORDS*2];
    uint32_t progsize = load_file(argv[1], program, sizeof(program));
    if (progsize == 0) {
        perror(argv[1]);
        exit(1);
    }
    
    printf("Loaded %d bytes of program image\n", progsize);
    
    uint8_t eeprom[512];
    uint32_t eepromsize = load_file("ardemu.eeprom", eeprom, sizeof(eeprom));
    
    printf("Loaded %d bytes of EEPROM\n", eepromsize);
}
