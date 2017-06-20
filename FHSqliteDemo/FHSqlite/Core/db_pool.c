//
//  db_pool.c
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/20.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "db_pool.h"
#include <stdlib.h>

db_pool pool_create(uint32_t max_count) {
    db_pool pool = malloc(sizeof(_db_pool));
    if (pool == NULL) return NULL;
    pool->_max_count = max_count;
    pool->_pool_count = 0;
    pool->_thefirst = NULL;
    return pool;
}
