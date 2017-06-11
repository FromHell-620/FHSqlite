//
//  NSObject+SQLGenerator.m
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/8.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#import "NSObject+SQLGenerator.h"
#import "FHSqliteProtocol.h"
#import "FHClassInfo.h"
#import <objc/runtime.h>
#import <objc/message.h>

@implementation NSObject (SQLGenerator)

FOUNDATION_STATIC_INLINE NSString * SqliteTypeFromPropertyType(FHPropertyInfo *info) {
    switch (info.typeEncoding) {
        case FHPropertyEncodingTypeInt:
            return @"INT";
        case FHPropertyEncodingTypeBool:
            return @"BOOL";
        case FHPropertyEncodingTypeLong:
            return @"LONG";
        case FHPropertyEncodingTypeFloat:
            return @"FLOAT";
        case FHPropertyEncodingTypeDouble:
            return @"DOUBLE";
        case FHPropertyEncodingTypeCString:
            return @"TEXT";
        case FHPropertyEncodingTypeObject: {
            switch (info.objectTypeEncoding) {
                case FHPropertyObjectEncodingTypeNSString:
                case FHPropertyObjectEncodingTypeNSURL:
                case FHPropertyObjectEncodingTypeNSDate:
                case FHPropertyObjectEncodingTypeNSNumber:
                    return @"TEXT";
                case FHPropertyObjectEncodingTypeNSData:
                case FHPropertyObjectEncodingTypeNSArray:
                case FHPropertyObjectEncodingTypeUIImage:
                case FHPropertyObjectEncodingTypeNSDictionary:
                    return @"BLOB";
                    
                default:
                    return nil;
            }
        }
            
        default:
            return nil;
    }
}

+ (NSString *)__tableName {
    Class<FHSqliteProtocol> cls = self;
    NSString *tableName = nil;
    if (class_respondsToSelector(cls, @selector(tableName))) {
        tableName = [cls tableName];
    }
    if (!tableName) tableName = NSStringFromClass(cls);
    return tableName;
}

+ (NSArray<NSString *> *)__columnNames {
    Class<FHSqliteProtocol> cls = self;
    NSArray<NSString *> *columnName = nil;
    if (class_respondsToSelector(cls, @selector(columnNames))) {
        columnName = [cls columnNames];
    }
    if (!columnName) {
        FHClassInfo *info = [FHClassInfo infoWithClass:cls];
        columnName = info.propertysExceptReadonly;
    }
    return columnName;
}

+ (NSString *)sql_initializeTable {
    NSString *tableName = [self __tableName];
    NSArray<NSString *> *columnNames = [self __columnNames];
    if (columnNames == nil || columnNames.count == 0) return nil;
    
    Class<FHSqliteProtocol> cls = self;
    NSDictionary<NSString *,FHSqliteConstraint *> *constraints = nil;
    if (class_respondsToSelector(self, @selector(constraints))) {
        constraints = [cls constraints];
    }
    NSMutableString *sql = [NSMutableString stringWithFormat:@"CREATE TABLE IF NOT EXISTS %@",tableName];
    
    NSString *(^columnItemSql)(NSString *,NSString *,FHSqliteConstraint *) = ^NSString *(NSString *columnName,NSString *columnType,FHSqliteConstraint *constraint) {
        if (columnType == nil) return nil;
        NSMutableString *item_sql = [NSMutableString stringWithFormat:@"%@ %@",columnName,columnType];
        if (FHSqliteConstraintStatePrimaryKey & constraint.constraintState) {
            [item_sql appendString:@" PRIMARY KEY"];
        }
        if (FHSqliteConstraintStateNonNull & constraint.constraintState) {
            [item_sql appendString:@" NOT NULL"];
        }
        if (FHSqliteConstraintStateUnique & constraint.constraintState) {
            [item_sql appendString:@" UNIQUE"];
        }
        
        if (FHSqliteConstraintStateAutoIncrement & constraint.constraintState) {
            [item_sql appendString:@" AUTOINCREMENT"];
        }
        return [item_sql copy];
    };
    
    FHClassInfo *classInfo = [FHClassInfo infoWithClass:self];
    [columnNames enumerateObjectsUsingBlock:^(NSString * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        FHPropertyInfo *info = [classInfo.propertysInfo objectForKey:obj];
        NSString *column_type = SqliteTypeFromPropertyType(info);
        FHSqliteConstraint *constraint = [constraints objectForKey:obj];
        NSString *sub_sql = columnItemSql(obj,column_type,constraint);
        if (sub_sql == nil) return ;
        if (idx == 0) {
            [sql appendString:@"("];
        }
        [sql appendFormat:@"%@,",sub_sql];
        if (idx == columnNames.count-1) {
            [sql appendString:@");"];
        }
    }];
    [sql deleteCharactersInRange:NSMakeRange(sql.length - 3, 1)];
    return [sql copy];
}

- (NSString *)sql_insert {
    return [self sql_insertOnColumns:[[self class] __columnNames]];
}

- (NSString *)sql_insertOnColumns:(NSArray<NSString *> *)columns {
    NSAssert(columns.count>0, @"columns长度必须大于0");
    NSMutableString *sql = [NSMutableString stringWithFormat:@"insert into %@",[[self class] __tableName]];
    NSMutableString *key = [NSMutableString string];
    NSMutableString *value = [NSMutableString string];
    [columns enumerateObjectsUsingBlock:^(NSString * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        [key appendFormat:@"%@,",obj];
        [value appendFormat:@":%@,",obj];
    }];
    [key deleteCharactersInRange:NSMakeRange(key.length-1, 1)];
    [value deleteCharactersInRange:NSMakeRange(value.length-1, 1)];
    [sql appendFormat:@" (%@) values (%@);",key,value];
    return [sql copy];
}

+ (NSString *)sql_insertWithMdoels:(NSArray *)models {
    NSMutableString *sql = [NSMutableString string];
    [models enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        NSString *sub_sql = [obj sql_insert];
        [sql appendString:sub_sql];
    }];
    return [sql copy];
}

- (NSString *)sql_update {
    return [self sql_updateOnColumns:[[self class] __columnNames]];
}

- (NSString *)sql_updateWithPredicate:(NSPredicate *)predicate {
    return [self sql_updateOnColumns:[[self class] __columnNames] predicate:predicate];
}

- (NSString *)sql_updateOnColumns:(NSArray<NSString *> *)columns {
    NSString *primaryKey = nil;
    Class<FHSqliteProtocol> cls = [self class];
    if (class_respondsToSelector(cls, @selector(primaryKey))) {
        primaryKey = [cls primaryKey];
    }
    if (primaryKey == nil) return nil;
    FHPropertyInfo *info = [[FHClassInfo infoWithClass:cls].propertysInfo objectForKey:primaryKey];
    if (info == nil) return nil;
    NSPredicate *predicate = nil;
    SEL sel = sel_registerName(primaryKey.UTF8String);
    
    switch (info.typeEncoding) {
        case FHPropertyEncodingTypeInt:
            predicate = [NSPredicate predicateWithFormat:@"%@ = %d",primaryKey,((int(*)(id,SEL))objc_msgSend)(self,sel)];
            break;
        case FHPropertyEncodingTypeLong:
            predicate = [NSPredicate predicateWithFormat:@"%@ = %ld",primaryKey,((NSInteger(*)(id,SEL))objc_msgSend)(self,sel)];
            break;
        case FHPropertyEncodingTypeFloat:
        case FHPropertyEncodingTypeDouble:
            predicate = [NSPredicate predicateWithFormat:@"%@ = %f",primaryKey,((float(*)(id,SEL))objc_msgSend)(self,sel)];
            break;
        case FHPropertyEncodingTypeCString:
            predicate = [NSPredicate predicateWithFormat:@"%@ = %s",primaryKey,((char *(*)(id,SEL))objc_msgSend)(self,sel)];
            break;
        case FHPropertyEncodingTypeObject:
            if (info.objectTypeEncoding == FHPropertyObjectEncodingTypeNSString||info.objectTypeEncoding == FHPropertyObjectEncodingTypeNSNumber) {
                predicate = [NSPredicate predicateWithFormat:@"%@ = %@",primaryKey,((id(*)(id,SEL))objc_msgSend)(self,sel)];
            }else if (info.objectTypeEncoding == FHPropertyObjectEncodingTypeNSURL) {
                NSURL *URL = ((NSURL*(*)(id,SEL))objc_msgSend)(self,sel);
                predicate = [NSPredicate predicateWithFormat:@"%@ = %@",primaryKey,URL.absoluteString];
            }else if (info.objectTypeEncoding == FHPropertyObjectEncodingTypeNSDate) {
                NSDate *date = ((NSDate*(*)(id,SEL))objc_msgSend)(self,sel);
                NSDateFormatter *formatter = [NSDateFormatter new];
                formatter.dateFormat = @"yyyy-MM-dd HH:mm:ss";
                predicate = [NSPredicate predicateWithFormat:@"%@ = %@",primaryKey,[formatter stringFromDate:date]];
            }
            break;
        default:
            break;
    }
    NSParameterAssert(predicate);
    return [self sql_updateOnColumns:columns predicate:predicate];
}

- (NSString *)sql_updateOnColumns:(NSArray<NSString *> *)columns
                        predicate:(NSPredicate *)predicate {
    NSAssert(columns.count>0, @"columns长度必须大于0");
    NSMutableString *sql = [NSMutableString stringWithFormat:@"update %@ set ",[[self class] __tableName]];
    [columns enumerateObjectsUsingBlock:^(NSString * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        NSString *sub = [NSString stringWithFormat:@"%@ = :%@,",obj,obj];
        [sql appendString:sub];
    }];
    [sql deleteCharactersInRange:NSMakeRange(sql.length-1, 1)];
    [sql appendString:predicate.predicateFormat];
    return [sql copy];
}

@end
