//
//  fh_sqlite.h
//  FHSqliteDemo
//
//  Created by imac on 2017/4/18.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#ifndef fh_sqlite_h
#define fh_sqlite_h

#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>
#include <sqlite3.h>

typedef struct {
    bool _in; // Whether it is a write connection.
    pthread_t _thread;
    sqlite3* _db;
    const char* _sqlite_path;
    CFMutableDictionaryRef _stmt_cache;//stmt cache. key is sql,value is sqlite3_stmt
}_sqlite_db;

typedef _sqlite_db* sqlite_db;

typedef struct {
    bool _used;
    sqlite_db _db;//a sqlite_db object
    struct _pool_node *next;
    struct _pool_node *prev;
    void(^poolDidFinishUsed)(struct _pool_node node);
}_pool_node;

typedef _pool_node *pool_node;

typedef struct {
    uint32_t _pool_count;
    uint32_t _max_count;
    uint32_t _temp_count;
    dispatch_semaphore_t _lock;
    pool_node _thefirst;
    pool_node _thelast;
    pool_node *_temp;
}_db_pool;

typedef _db_pool *db_pool;

typedef struct {
    const char* _sqlite_path;
    pthread_mutex_t _sqlite_lock;
    db_pool pool;
}_fh_sqlite;

typedef _fh_sqlite* SqliteRef;

SqliteRef sqlite_initialize(const char* sqlite_path);

sqlite_db db_get_current(SqliteRef sqlite);

bool db_is_in(sqlite_db db);

sqlite_db sqlite_open(SqliteRef sqlite);

void sqlite_delloc(SqliteRef sqlite);

void sqlite_db_delloc(sqlite_db _db);

db_pool pool_create(uint32_t max_count);

pool_node pool_query(db_pool pool);

#endif /* fh_sqlite_h */
