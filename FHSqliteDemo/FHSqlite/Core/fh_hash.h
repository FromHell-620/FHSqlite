//
//  fh_hash.h
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/7/12.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#ifndef fh_hash_h
#define fh_hash_h

#include <stdio.h>

typedef struct hashEntry {
    const void *key;
    const void *value;
    struct hashEntry *next;
} hashEntry;

#endif /* fh_hash_h */
