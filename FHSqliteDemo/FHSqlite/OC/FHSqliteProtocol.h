//
//  FHSqliteProtocol.h
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/5.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSInteger, FHSqliteConstraintState) {
    FHSqliteConstraintStateNonNull = 1<<2,
    FHSqliteConstraintStateUnique = 1<<3,
    FHSqliteConstraintStatePrimaryKey = 1<<4,
    FHSqliteConstraintStateAutoIncrement = 1<<5,
    FHSqliteConstraintStateDefaultValue = 1<<6,//Unimplemented
    FHSqliteConstraintStateForeignKey = 1<<7//Unimplemented
};

@interface FHSqliteConstraintDefault : NSObject

+ (instancetype)defaultWithInt:(int)i;

+ (instancetype)defaultWithFloat:(float)f;

+ (instancetype)defaultWithCString:(const char *)cStr;

+ (instancetype)defaultWithObject:(NSObject *)obj;

- (int)intValue;

- (float)floatValue;

- (const char *)cStringValue;

- (NSObject *)objectValue;

@end

@interface FHSqliteConstraint : NSObject

@property (nonatomic,assign) FHSqliteConstraintState constraintState;

@property (nonatomic,strong,nullable) FHSqliteConstraintDefault *defaultValue;

+ (instancetype)constraintWithstate:(FHSqliteConstraintState)state;

@end

@protocol FHSqliteProtocol <NSObject>

@optional

@property (nonatomic,copy,class,nullable) NSString *tableName;

@property (nonatomic,copy,class,nullable) NSString *primaryKey;

@property (nonatomic,copy,class,nullable) NSArray<NSString *> *columnNames;

@property (nonatomic,copy,class,nullable) NSArray<NSString *> *indexNames;

@property (nonatomic,copy,class,nullable) NSDictionary<NSString *,FHSqliteConstraint *> *constraints;

@end

NS_ASSUME_NONNULL_END

