//
//  SpinLock.h
//  ax_core
//
//  Created by Jason Chan on 12/1/14.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_SpinLock_h
#define ax_core_SpinLock_h

#include "Locked.h"

namespace ax {
namespace System {
namespace Thread {

class SpinLock : public NonCopyable {
public:
	SpinLock() : _p( ATOMIC_FLAG_INIT ) {
	}
	
	void	lock	() {
		while ( _p.test_and_set(std::memory_order_acquire) )  // acquire lock
			; // spin
	}
	void	unlock	() {
		_p.clear(std::memory_order_release);
	}

private:
	std::atomic_flag	_p;
};


template< typename T0, typename T1, typename T2 > class Locked_SpinLockData;

template< typename T >
class SpinLockData : public NonCopyable {
public:
	typedef	SpinLock	Lock;
	typedef	T			Value;
	
//	void	signal();
	
template< typename T0, typename T1, typename T2 > friend class Locked_SpinLockData;
protected:
	Lock		_lock;
	T			_value;
};


}}} //namespace

#endif
