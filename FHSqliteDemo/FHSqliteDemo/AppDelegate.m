//
//  AppDelegate.m
//  FHSqliteDemo
//
//  Created by imac on 2017/4/18.
//  Copyright © 2017年 FromHell. All rights reserved.
//

#import "AppDelegate.h"
#import "NSObject+SQLGenerator.h"
#import "Test.h"
#import <sqlite3.h>
#include "fh_hash.h"

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    FHHashRef hash = FHHashCreateWithOptions(1000, &kFHCopyStringKeyCallback, NULL);
    CFMutableDictionaryRef cf_hash = CFDictionaryCreateMutable(CFAllocatorGetDefault(), 0, &kCFCopyStringDictionaryKeyCallBacks, NULL);
    for (int i=0; i<2000; i++) {
        const void *key = @(i).stringValue.UTF8String;
        const void *value = @(i).stringValue.UTF8String;
        FHHashSetValue(hash, key, value);
    }
    for (int i=0; i<2000; i++) {
        const void *key =  (__bridge const void *)@(i).stringValue;
        const void *value = (__bridge const void *)@(i).stringValue;
        CFDictionarySetValue(cf_hash, key, value);
    }
    NSTimeInterval begin, end, time;
    
    begin = CACurrentMediaTime();
    @autoreleasepool {
        
        for (int i=0; i<2000; i++) {
            const void *key = @(i).stringValue.UTF8String;
            FHHashGetValue(hash, key);
        }
    }
    end = CACurrentMediaTime();
    time = end - begin;
    printf("mine:   %8.2f\n", time * 1000);
    
    
    begin = CACurrentMediaTime();
    @autoreleasepool {
        
        for (int i=0; i<2000; i++) {
            const void *key =  (__bridge const void *)@(i).stringValue;
            CFDictionaryGetValue(cf_hash, key);
        }
    }
    end = CACurrentMediaTime();
    time = end - begin;
    printf("cf:   %8.2f\n", time * 1000);
    // Override point for customization after application launch.
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}


- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}


@end
