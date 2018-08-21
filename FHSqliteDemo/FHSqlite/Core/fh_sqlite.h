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

struct sqlite_configuration {
    unsigned int allow_cache_stmt : 1;
    unsigned int pool_size : 4;
    void (*callback)(void);//pool 有空闲的node时调用
};

typedef union fh_value {
    int int_value;
    float float_value;
    char *str_value;
    struct {
        void *blob;
        unsigned int len;
    } blob_value;
} fh_value;

typedef struct sqlite_configuration * SqliteConfigurationref;

typedef struct fh_sqlite * FHSqliteRef;

typedef struct fh_db * FHDBRef;

FH_EXTERN
FHSqliteRef FHSqliteCreate(const char *path);

FH_EXTERN
FHSqliteRef FHSqliteCreateWithOptions(const char *path,SqliteConfigurationref congifuration);

FH_EXTERN
void FHSqliteExec(FHSqliteRef sqlite,FHDBRef db,char *sql);

FH_EXTERN
void FHSqlitePrepare(FHSqliteRef sqlite,FHDBRef db,char *sql,FHBoolean unuseDB);

FH_EXTERN
void FHSqliteRelease(FHSqliteRef sqlite);


#endif /* fh_sqlite_h */
