//
//  loader.c
//  Ardemu
//
//  Created by Jimmy Lipham on 2/26/18.
//  Copyright © 2018 86pixels. All rights reserved.
//

#include "loader.h"

#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t load_binary(const char *filename, uint8_t *buf, uint32_t bufsize)
{
    fprintf(stderr, "emulino: Loading binary image: %s\n", filename);
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        perror(filename);
        exit(1);
    }
    uint32_t r = fread(buf, 1, bufsize, f);
    fclose(f);
    return r;
}

uint32_t load_hex(const char *filename, uint8_t *buf, uint32_t bufsize)
{
    fprintf(stderr, "emulino: Loading hex image: %s\n", filename);
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return 0;
    }
    char s[100];
    int eof = 0;
    uint32_t m = 0;
    while (!eof && fgets(s, sizeof(s), f)) {
        assert(s[0] == ':');
        uint8_t c = 0;
        int i;
        for (i = 1; isalnum(s[i]); i += 2) {
            int x;
            sscanf(s+i, "%02x", &x);
            c += x;
        }
        assert(c == 0);
        int n, a, t;
        sscanf(s+1, "%02x%04x%02x", &n, &a, &t);
        switch (t) {
            case 0x00:
                for (i = 0; i < n; i++) {
                    int x;
                    sscanf(s+9+i*2, "%02x", &x);
                    buf[a+i] = x;
                    if (a+i >= m) {
                        m = a+i+1;
                    }
                }
                break;
            case 0x01:
                eof = 1;
                break;
            default:
                fprintf(stderr, "unsupported hex type: %02x\n", t);
                exit(1);
        }
    }
    fclose(f);
    return m;
}

uint32_t load_file(const char *filename, uint8_t *buf, uint32_t bufsize)
{
    // Open the given filename
    FILE *f = fopen(filename, "r");
    if (f == 0) {
        return 0;
    }
    
    char s[100];
    fgets(s, sizeof(s), f);
    fclose(f);
    
    uint32_t r = 0;
    int n;
    if (sscanf(s, ":%02x", &n) == 1 && strcspn(s, "\r\n") == 11+2*n) {
        r = load_hex(filename, buf, bufsize);
        if (r == 0) {
            perror(filename);
            exit(1);
        }
    } else {
        r = load_binary(filename, buf, bufsize);
    }
    
    return r;
}
