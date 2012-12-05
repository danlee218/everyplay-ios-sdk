/*
 * Copyright 2012 Applifier
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "EveryplayCapture.h"
#import "EveryplayAccount.h"
#import "EveryplayRequest.h"

#pragma mark - Developer metadata
static NSString * const kEveryplayMetadataScoreInteger = @"score";
static NSString * const kEveryplayMetadataLevelInteger = @"level";
static NSString * const kEveryplayMetadataLevelNameString = @"level_name";

#pragma mark - View controller flow settings

typedef enum  {
    EveryplayFlowReturnsToGame         = 1 << 0,    // Default
    EveryplayFlowReturnsToVideoPlayer  = 1 << 1
} EveryplayFlowDefs;

#pragma mark - Error codes
static NSString * const kEveryplayErrorDomain = @"com.everyplay";

static const int kEveryplayLoginCanceledError = 100;
static const int kEveryplayMovieExportCanceledError = 101;
static const int kEveryplayFileUploadError = 102;

#pragma mark - Notifications

extern NSString * const EveryplayAccountDidChangeNotification;
extern NSString * const EveryplayDidFailToRequestAccessNotification;

#pragma mark - Handler

typedef void(^EveryplayAccessRequestCompletionHandler)(NSError *error);
typedef void(^EveryplayPreparedAuthorizationURLHandler)(NSURL *preparedURL);
typedef void(^EveryplayDataLoadingHandler)(NSError *error, id data);

#pragma mark -

@class EveryplayAccount;

@class EveryplayVideoPlayerViewController;

@protocol EveryplayVideoPlayerDelegate
@required
- (void)EveryplayVideoReachedEnd:(EveryplayVideoPlayerViewController *)videoPlayerViewController;
- (void)EveryplayVideoDoneButtonPressed:(EveryplayVideoPlayerViewController *)videoPlayerViewController;
@end

@protocol EveryplayDelegate <NSObject>
- (void)everyplayShown;
- (void)everyplayHidden;
@optional
- (void)everyplayRecordingStarted;
- (void)everyplayRecordingStopped;

- (void)everyplayThumbnailReadyAtFilePath:(NSString *)thumbnailFilePath;
- (void)everyplayThumbnailReadyAtURL:(NSURL *)thumbnailUrl;
@end

@interface Everyplay : NSObject <EveryplayVideoPlayerDelegate> {
}

#pragma mark - Properties
@property (nonatomic, strong) EveryplayCapture *capture;
@property (nonatomic, strong) UIViewController *parentViewController;
@property (nonatomic, strong) id <EveryplayDelegate> everyplayDelegate;
@property (nonatomic, assign) EveryplayFlowDefs flowControl;

#pragma mark - Singleton
+ (Everyplay *)sharedInstance;

+ (BOOL)isSupported;

+ (Everyplay *)initWithDelegate:(id <EveryplayDelegate>)everyplayDelegate;
+ (Everyplay *)initWithDelegate:(id <EveryplayDelegate>)everyplayDelegate andParentViewController:(UIViewController *)viewController;

#pragma mark - Public Methods
- (void)showEveryplay;
- (void)hideEveryplay;
- (void)playLastRecording;

- (void)mergeSessionDeveloperData:(NSDictionary *)dictionary;

#pragma mark - Manage Accounts
+ (EveryplayAccount *)account;

+ (void)requestAccessWithCompletionHandler:(EveryplayAccessRequestCompletionHandler)aCompletionHandler;
+ (void)requestAccessforScopes:(NSString *)scopes
         withCompletionHandler:(EveryplayAccessRequestCompletionHandler)aCompletionHandler;
+ (void)removeAccess;

#pragma mark - Configuration
+ (void)setClientId:(NSString *)client
       clientSecret:(NSString *)secret
        redirectURI:(NSString *)url;

#pragma mark - OAuth2 Flow
+ (BOOL)handleRedirectURL:(NSURL *)URL;

@end


#pragma mark - Macros

#define EVERYPLAY_CANCELED(error) ([error.domain isEqualToString:(NSString *)kEveryplayErrorDomain] && error.code == kEveryplayLoginCanceledError)
