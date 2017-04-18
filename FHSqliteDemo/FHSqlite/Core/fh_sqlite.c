//
//  fh_sqlite.c
//  FHSqliteDemo
//
//  Created by imac on 2017/4/18.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#include "fh_sqlite.h"
#include <pthread/pthread.h>

CF_INLINE void sqlite_lock_initialize(SqliteRef sqlite)
{
    pthread_mutex_init(&(sqlite->_sqlite_lock), NULL);
}

CF_INLINE void sqlite_lock_destory(SqliteRef sqlite)
{
    pthread_mutex_destroy(&(sqlite->_sqlite_lock));
}

CF_INLINE void sqlite_lock(SqliteRef sqlite)
{
    pthread_mutex_lock(&(sqlite->_sqlite_lock));
}
                       
CF_INLINE void sqlite_unlock(SqliteRef sqlite)
{
    pthread_mutex_unlock(&(sqlite->_sqlite_lock));
}

SqliteRef sqlite_initialize(const char* sqlite_path)
{
    if (sqlite_path == NULL || strlen(sqlite_path) <= 0) return NULL;
    
    SqliteRef sqlite = malloc(sizeof(sqlite));
    if (sqlite == NULL) return NULL;
    sqlite->_db_cache = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 10, NULL, NULL);
    sqlite->_sqlite_path = strdup(sqlite_path);
    sqlite_lock_initialize(sqlite);
    return sqlite;
}
