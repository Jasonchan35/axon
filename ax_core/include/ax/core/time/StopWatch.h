//
//  StopWatch.h
//  ax_core
//
//  Created by Jason on 2014-11-27.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_StopWatch_h
#define ax_core_StopWatch_h

#include "Timestamp.h"

namespace ax {
namespace System {
namespace Time {

class StopWatch {
public:
	StopWatch() {
		_start.setToNow();
		_last = _start;
	}

	void	reset() {
		_start.setToNow();
	}
	
	void	resetToLastGet() {
		_start = _last;
	}
	
	double	lastGet() {
		return _last.seconds() - _start.seconds();
	}
	
	double	get() {
		_last.setToNow();
		return lastGet();
	}


private:
	Timestamp	_start;
	Timestamp	_last;
};

} //namespace Time
} //namespace System
} //namespace ax


#endif
