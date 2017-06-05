//
//  FHSqliteProtocol.m
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/5.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#import "FHSqliteProtocol.h"

@implementation FHSqliteConstraintDefault



@end

@implementation FHSqliteConstraint

+ (instancetype)constraintWithColumnName:(NSString *)columnName
                                   state:(FHSqliteConstraintState)state {
    FHSqliteConstraint *constraint = [[FHSqliteConstraint alloc] init];
    constraint.columnName = columnName;
    constraint.constraintState = state;
    return constraint;
}

@end
