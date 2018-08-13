//
//  fh_hash.c
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/7/12.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "fh_hash.h"

struct FHHash {
    FHIndex capacity;
    FHIndex count;
    hashEntry *buckets;
    FHHashKeyCallback key_callback;
    FHHashValueCallback value_callback;
};
