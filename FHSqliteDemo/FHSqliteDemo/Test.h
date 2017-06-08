//
//  Test.h
//  FHSqliteDemo
//
//  Created by 李浩 on 2017/6/8.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Test : NSObject

@property (nonatomic,assign) int i;

@property (nonatomic,assign) float f;

@property (nonatomic,assign,readonly) BOOL b_r;

@property (nonatomic,assign) BOOL b;

@property (nonatomic,copy) NSString * str;

@property (nonatomic,copy) NSNumber *number;

@end
