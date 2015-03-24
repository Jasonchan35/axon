//
//  Timestamp.cpp
//  ax_core
//
//  Created by Jason on 2014-11-27.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include <ax/core/time/Timestamp.h>


namespace ax {
namespace System {
namespace Time {


#if ax_OS_Win


void Timestamp::setToNow() {
	FILETIME ft;
	GetSystemTimeAsFileTime( &ft );
	setFILETIME( ft );
}


void Timestamp::setFILETIME( FILETIME &ft ) {
	uint64_t u = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime; // based on 1601-01-01
	u -= 116444736000000000; // based on 1970-01-01
	second_ = (double)( (long double) u / 10000000 );  // 100-nanosecond to second
}


FILETIME Timestamp::toFILETIME() const {
	uint64_t u = (uint64_t) ((long double)second_ * 10000000);  // second to 100-nanosecond 
	u += 116444736000000000; // based on 1970-01-01
	
	FILETIME	ft;
	ft.dwHighDateTime = u >> 32;
	ft.dwLowDateTime  = (DWORD)u;
	return ft;
}

#endif //ax_OS_Win

#if ax_OS_Unix

#if ax_OS_iOS || ax_OS_MacOSX
	// check .mm file
#else
	void Timestamp::setToNow() {
		timespec	t;
		clock_gettime( CLOCK_REALTIME, &t );
		set_timespec( t );
	}
#endif

void	Timestamp::set_timespec( timespec & t ) {
	_sec = (double)t.tv_sec + ( (double)t.tv_nsec / 1000000000 );
}

timespec Timestamp::to_timespec() const {
	timespec o;
	double r,i;
	r = ax_modf( _sec, &i );
	o.tv_sec  = (time_t)i;
	o.tv_nsec = (long)(r * 1000000000);
	return o;
}


void	Timestamp::set_timeval( timeval & t ) {
	_sec = (double)t.tv_sec + ( (double)t.tv_usec / 1000000 );
}


timeval Timestamp::to_timeval() const {
	timeval	o;
	double r,i;
	r = ax_modf( _sec, &i );
	o.tv_sec  = (time_t)i;
	o.tv_usec = (useconds_t)(r * 1000000);
	return o;
}

#endif //ax_OS_Unix


} //namespace Time
} //namespace System
} //namespace ax
