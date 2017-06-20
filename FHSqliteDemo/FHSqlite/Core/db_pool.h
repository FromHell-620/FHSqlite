//
//  db_pool.h
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/20.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#ifndef db_pool_h
#define db_pool_h

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    void *_value;//a sqlite_db object
    bool _used;
    struct _pool_node *next;
    struct _pool_node *prev;
}_pool_node;

typedef _pool_node *pool_node;

typedef struct {
    uint32_t _pool_count;
    uint32_t _max_count;
    pool_node _thefirst;
}_db_pool;

typedef _db_pool *db_pool;

db_pool pool_create(uint32_t max_count);
#endif /* db_pool_h */
