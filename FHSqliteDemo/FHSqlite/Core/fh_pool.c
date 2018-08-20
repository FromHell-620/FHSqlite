//
//  fh_pool.c
//  FHSqliteDemo
//
//  Created by imac on 2018/8/15.
//  Copyright © 2018年 FromHell. All rights reserved.
//

#include "fh_pool.h"
#include <stdlib.h>
#include <pthread/pthread.h>

typedef struct fh_pool {
    unsigned int pool_size : 4; //max size is 2 << 4
    unsigned int count : 4;
    unsigned int in_use : 4;
    void (*callback)(void);
    pthread_mutex_t lock;
    pthread_cond_t cond;
    linkList *pool;
} fh_pool;

FH_INLINE FHBoolean __FHPoolIsBusy(FHPoolRef pool) {
    if (pool == NULL) return false;
    return !(pool->pool_size > pool->count ? true : (pool->count > pool->in_use));
}

FHPoolRef FHPoolCreate(unsigned int pool_size,FHPoolValueCallback *valueCallback) {
    return FHPoolCreateWithCallback(pool_size, valueCallback, NULL);
}

FHPoolRef FHPoolCreateWithCallback(unsigned int pool_size,FHPoolValueCallback *valueCallback,void(*callback)(void)) {
    FHPoolRef pool;
    if ((pool = calloc(1, sizeof(fh_pool))) == NULL) return NULL;
    linkListNodeCallback link_callback = {valueCallback?valueCallback->retain:NULL,valueCallback?valueCallback->release:NULL,NULL};
    if((pool->pool = linkListify(&link_callback)) == NULL) {
        free(pool);
        return NULL;
    }
    if (pool_size > 16) pool_size = 16;
    pool->pool_size = pool_size;
    pool->count = pool->in_use = 0;
    pool->callback = callback;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->cond, NULL);
    return pool;
}

FHBoolean FHPoolIsBusy(FHPoolRef pool) {
    return __FHPoolIsBusy(pool);
}

linkNode *FHPoolQuery(FHPoolRef pool) {
    if (pool == NULL) return NULL;
    pthread_mutex_lock(&pool->lock);
    linkNode *node = NULL;
    if (pool->count > pool->in_use) {
        node = pool->pool->tail;
    }else {
        if (pool->count < pool->pool_size) {
            node = linkNodeify(NULL);
        }else {
            while (__FHPoolIsBusy(pool)) {
                pthread_cond_wait(&pool->cond, &pool->lock);
            }
            if (pool->count == pool->in_use) printf("pool query error %s",__func__);
            node = pool->pool->tail;
        }
    }
    if (node->next == NULL && node->prev == NULL) {
        pool->count ++;
        linkListAddHeadWithNode(pool->pool, node);
    }
    if (node->value) {
        linkListTailToHead(pool->pool);
        pool->in_use ++;
    }
    pthread_mutex_unlock(&pool->lock);
    return node;
}

void FHPoolUnuse(FHPoolRef pool,linkNode *node) {
    if (pool == NULL || node == NULL) return;
    pthread_mutex_lock(&pool->lock);
    linkListMoveToTail(pool->pool, node);
    pool->in_use --;
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->lock);
}

void FHPoolRelease(FHPoolRef pool) {
    if (pool == NULL) return;
    linkListRelease(pool->pool);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);
    free(pool);
    pool = NULL;
}
