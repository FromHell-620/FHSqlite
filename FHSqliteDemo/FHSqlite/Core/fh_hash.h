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
#include "fh_define.h"

typedef unsigned long FHHashCode;

typedef const void * (*FHHashRetainCallback)(const void *value);
typedef void (*FHHashReleaseCallback)(const void *value);
typedef FHBoolean (*FHHashEqualCallback)(const void *value1,const void *value2);
typedef FHHashCode (*FHHashCodeCallback)(const void *value);

typedef struct {
    FHHashRetainCallback retain;
    FHHashRetainCallback release;
    FHHashEqualCallback equal;
    FHHashCodeCallback hash;
} FHHashKeyCallback;

typedef struct {
    FHHashRetainCallback retail;
    FHHashReleaseCallback release;
    FHHashEqualCallback equal;
} FHHashValueCallback;

FH_EXTERN const FHHashKeyCallback kFHCopyStringKeyCallback;

typedef void (*FHHashApplierFunction)(const void *key,const void *value,void *context);


typedef struct hashEntry {
    const void *key;
    const void *value;
    FHHashCode hash;
    struct hashEntry *next;
} hashEntry;

typedef hashEntry * hashEntryRef;

typedef struct FHHash *FHHashRef;

FH_EXTERN
FHHashRef FHHashCreate();

FH_EXTERN
FHHashRef FHHashCreateWithOptions(FHIndex capacity,const FHHashKeyCallback *keyCallback,const FHHashValueCallback *valueCallback);

FH_EXTERN
FHIndex FHHashGetCount(FHHashRef hash);

FH_EXTERN
FHIndex FHHashContainsKey(FHHashRef hash,const void *key);

FH_EXTERN
FHBoolean FHHashContainsValue(FHHashRef hash,const void *value);

FH_EXTERN
const void *FHHashGetValue(FHHashRef hash,const void *key);

FH_EXTERN
void FHHashSetValue(FHHashRef hash,const void *key,const void *value);

FH_EXTERN
void FHHashRemoveValue(FHHashRef hash,const void *key);

FH_EXTERN
void FHHashReplaceValue(FHHashRef hash,const void *key,const void *value);

FH_EXTERN
void FHHashClean(FHHashRef hash);

FH_EXTERN
void FHHashApplyFuncation(FHHashRef hash,FHHashApplierFunction applier,void *context);

#endif /* fh_hash_h */
