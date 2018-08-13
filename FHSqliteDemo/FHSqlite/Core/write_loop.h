//
//  write_loop.h
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/19.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#ifndef write_loop_h
#define write_loop_h

#include <stdio.h>
#include "fh_sqlite.h"
typedef struct {
    pthread_t _thread;
}_write_loop;

typedef struct _write_loop* write_loop;

write_loop loop_initializeify();

#endif /* write_loop_h */
