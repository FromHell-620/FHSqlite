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
#include "fh_linked.h"

typedef struct fh_db {
    bool is_used;
    sqlite3 *db;
    CFMutableDictionaryRef stmt_cache;
} fh_db;

typedef struct fh_pool {
    unsigned long used_count;
    unsigned long max_count;
    linkListNodeCallback *callback;
    linkList *pool;
    linkList *temp_pool;
} fh_pool;


typedef struct fh_sqlite {
    const char *sqlite_path;
    pthread_mutex_t sqlite_lock;
    fh_pool *pool;
} fh_sqlite;

fh_pool *pool_create(unsigned long max_count,linkListNodeCallback *callback);

typedef struct _sqlite_db {
    bool _in; // Whether it is a write connection.
    pthread_t _thread;
    sqlite3* _db;
    const char* _sqlite_path;
    CFMutableDictionaryRef _stmt_cache;//stmt cache. key is sql,value is sqlite3_stmt
}_sqlite_db;

typedef _sqlite_db* sqlite_db;

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



#endif /* fh_sqlite_h */
