//
//  loader.h
//  Ardemu
//
//  Created by Jimmy Lipham on 2/26/18.
//  Copyright © 2018 86pixels. All rights reserved.
//

#ifndef loader_h
#define loader_h

#include <stdio.h>

uint32_t load_file(const char *filename, uint8_t *buf, uint32_t bufsize);

#endif /* loader_h */
