//
//  fh_pool.c
//  FHSqliteDemo
//
//  Created by imac on 2018/8/15.
//  Copyright © 2018年 FromHell. All rights reserved.
//

#include "fh_pool.h"
#include <stdlib.h>

typedef struct fh_pool {
    unsigned int pool_size : 4; //max size is 2 << 4
    unsigned int count : 4;
    unsigned int in_use : 4;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    linkList *pool;
} fh_pool;

FHPoolRef FHPoolCreate(unsigned int pool_size,FHPoolValueCallback *callback) {
    FHPoolRef pool;
    if ((pool = calloc(1, sizeof(fh_pool))) == NULL) return NULL;
    linkListNodeCallback link_callback = {callback?callback->retain:NULL,callback?callback->release:NULL,NULL};
    if((pool->pool = linkListify(&link_callback)) == NULL) {
        free(pool);
        return NULL;
    }
    if (pool_size > 16) pool_size = 16;
    pool->pool_size = pool_size;
    pool->count = pool->in_use = 0;
    return pool;
}
