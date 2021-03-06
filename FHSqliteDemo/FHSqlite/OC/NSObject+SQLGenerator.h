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

+ (NSArray<NSString *> *)sql_initializeIndexes;

- (NSString *)sql_insert;

- (NSString *)sql_insertOnColumns:(NSArray<NSString *> *)columns;

+ (NSString *)sql_insertWithMdoels:(NSArray *)models;

- (NSString *)sql_update;

- (NSString *)sql_updateWithPredicate:(NSPredicate *)predicate;

- (NSString *)sql_updateOnColumns:(NSArray<NSString *> *)columns;

- (NSString *)sql_updateOnColumns:(NSArray<NSString *> *)columns
                        predicate:(NSPredicate *)predicate;

- (NSString *)sql_delete;

+ (NSString *)sql_deleteWithPredicate:(NSPredicate *)predicate;

+ (NSString *)sql_selectWithPredicate:(NSPredicate *)predicate;

+ (NSString *)sql_selectWithPredicate:(NSPredicate *)predicate orderBy:(NSString *)column asc:(BOOL)asc;

@end
