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

typedef struct {
    const char* _sqlite_path;
    pthread_mutex_t _sqlite_lock;
    CFMutableDictionaryRef _db_cache;
}fh_sqlite;

typedef fh_sqlite* SqliteRef;

SqliteRef sqlite_initialize(const char* sqlite_path);

#endif /* fh_sqlite_h */
