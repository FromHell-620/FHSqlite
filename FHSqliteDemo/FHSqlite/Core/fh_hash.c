//
//  fh_hash.c
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/7/12.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "fh_hash.h"
#include <string.h>
#include <stdlib.h>

static const void *__FHHashStringRetainCallback(const void *key) {
    return strdup(key);
}

static void __FHHashStringReleaseCallback(const void *key) {
    free((void *)key);
}

static FHBoolean __FHHashStringEqualCallback(const void *key1,const void *key2) {
    return strcmp(key1, key2) == 0;
}

struct FHHash {
    FHIndex capacity;
    FHIndex count;
    hashEntry *buckets;
    FHHashKeyCallback key_callback;
    FHHashValueCallback value_callback;
};

const FHHashKeyCallback kFHCopyStringKeyCallback = {
    
}

FHHashRef FHHashCreate() {
    return FHHashCreateWithOptions(0, NULL, NULL);
}

FHHashRef FHHashCreateWithOptions(FHIndex capacity,const FHHashKeyCallback *keyCallback,const FHHashValueCallback *valueCallback) {
    
}
