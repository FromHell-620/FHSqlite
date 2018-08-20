//
//  fh_pool.h
//  FHSqliteDemo
//
//  Created by imac on 2018/8/15.
//  Copyright © 2018年 FromHell. All rights reserved.
//

#ifndef fh_pool_h
#define fh_pool_h

#include <stdio.h>
#include "fh_define.h"
#include "fh_linked.h"

typedef void *(*FHPoolValueRetainCallback)(void *value);
typedef void (*FHPoolValueReleaseCallback)(void *value);

typedef struct FHPoolValueCallback {
    FHPoolValueRetainCallback retain;
    FHPoolValueReleaseCallback release;
} FHPoolValueCallback;

typedef struct fh_pool * FHPoolRef;

FH_EXTERN
FHPoolRef FHPoolCreate(unsigned int pool_size,FHPoolValueCallback *valueCallback);

FH_EXTERN
FHPoolRef FHPoolCreateWithCallback(unsigned int pool_size,FHPoolValueCallback *valueCallback,void(*callback)(void));

FH_EXTERN
FHBoolean FHPoolIsBusy(FHPoolRef pool);

FH_EXTERN
linkNode *FHPoolQuery(FHPoolRef pool);

FH_EXTERN
void FHPoolUnuse(FHPoolRef pool,linkNode *node);

FH_EXTERN
void FHPoolRelease(FHPoolRef pool);
#endif /* fh_pool_h */
