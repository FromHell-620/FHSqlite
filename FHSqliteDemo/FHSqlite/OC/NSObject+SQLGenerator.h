//
//  NSObject+SQLGenerator.h
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/8.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSObject (SQLGenerator)

+ (NSString *)sql_initializeTable;

- (NSString *)sql_insert;

- (NSString *)sql_insertOncolumns:(NSArray<NSString *> *)columns;

+ (NSString *)sql_insertWithMdoels:(NSArray *)models;

@end
