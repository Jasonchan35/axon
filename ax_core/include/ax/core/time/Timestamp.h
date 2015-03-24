//
//  Timestamp.h
//  ax_core
//
//  Created by Jason on 2014-11-27.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_Timestamp_h
#define ax_core_Timestamp_h

#include "../platform/platform.h"

namespace ax {
namespace System {
namespace Time {

//! timestamp - UTC second based on Unix epoch ( 1970-01-01 )

class Timestamp {
public:
	Timestamp() : _sec(0) {}

	void	setToNow();
	
	void	setSeconds( double sec ) { _sec = sec; }
	
	double	seconds() const		{ return _sec; }


#if ax_OS_Win
	void		set_FILETIME	( FILETIME &ft );
	FILETIME	to_FILETIME		() const;
#endif

#if ax_OS_Unix
	void		set_timespec	( timespec & ts );
	timespec	to_timespec		() const;

	void		set_timeval		( timeval & ts );
	timeval		to_timeval		() const;
#endif

#if __OBJC__
	void		set_NSDate( NSDate *d );
#endif // __OBJC__
	
private:
	double	_sec;
};


} //namespace Time
} //namespace System
} //namespace ax

#endif
