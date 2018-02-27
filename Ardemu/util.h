//
//  util.h
//  Ardemu
//
//  Created by Compy on 2/26/18.
//  Copyright © 2018 86pixels. All rights reserved.
//

#ifndef util_h
#define util_h

typedef int bool;
#define false   0
#define true    (!false)

#define BIT(b) (1 << (b))
#define LENGTHOF(x) (sizeof(x) / sizeof((x)[0]))
#define COMPILE_ASSERT(x) extern int __assertion[(x) ? 1 : -1]

#endif /* util_h */
