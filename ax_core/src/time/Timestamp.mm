//
//  Timestamp.mm
//  ax_core
//
//  Created by Jason on 2014-11-27.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include <ax/core/time/Timestamp.h>

namespace ax {
namespace System {
namespace Time {

#if ax_OS_MacOSX || ax_OS_iOS

void Timestamp::setToNow() {
	@autoreleasepool {
		set_NSDate( [NSDate date] );
	}
}

void Timestamp::set_NSDate( NSDate *d ) {
	_sec = [d timeIntervalSince1970 ];
}

#endif

} //namespace Time
} //namespace System
} //namespace ax
