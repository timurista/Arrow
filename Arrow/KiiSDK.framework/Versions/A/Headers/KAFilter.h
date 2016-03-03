//
//  KAFilter.h
//  KiiAnalytics
//
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

/** Use this class to set up one or more filters for your query
 */
@interface KAFilter : NSObject

/** Add a key/value pair to this filter
 
 Use filters to slice your data into useful result sets
 @param filterKey The key to filter by. Filter key must match with pattern ^[a-zA-Z][a-zA-Z0-9_]{0,63}$
 @param filterValue The value to filter against. Filter value must not be nil or empty.
 @exception NSException Named NSInvalidArgumentException is thrown if filter key or filter value is invalid.
*/
- (void) addFilter:(NSString*)filterKey withValue:(NSString*)filterValue;

@end
