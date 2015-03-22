//
//  LockedData.h
//  ax_core
//
//  Created by Jason Chan on 12/1/14.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_LockedData_h
#define ax_core_LockedData_h

namespace ax {
namespace System {
namespace Thread {

template< typename D0, typename D1, typename D2 >
class LockedData : public NonCopyable {
public:
	LockedData( D0 & p0, D1 & p1, D2 & p2 )
	: _locked( p0._lock, p1._lock, p2._lock )
	, d0( &p0 )
	, d1( &p1 )
	, d2( &p2 )
	{}
	
	Locked_Data( Locked_Data && rhs ) {
		_locked  = ax_move( rhs._locked );
		d0 = rhs.d0;
		d1 = rhs.d1;
		d2 = rhs.d2;
		
		rhs.d0 = nullptr;
		rhs.d1 = nullptr;
		rhs.d2 = nullptr;
	}

	typename D0::Value &	value0() { return d0->_value; }
	typename D1::Value &	value1() { return d1->_value; }
	typename D2::Value &	value2() { return d2->_value; }

			D0 &			data0() { return *d0; }
			D1 &			data1() { return *d1; }
			D2 &			data2() { return *d2; }

private:
	Locked< typename D0::Lock, typename D1::Lock, typename D2::Lock >	_locked;

	D0*	d0;
	D1*	d1;
	D2*	d2;
};

template< typename D0, typename D1, typename D2 > inline
LockedData<D0,D1,D2>	ax_lock_data( D0 & p0, D1 & p1, D2 & p2 ) { return LockedData<D0,D1,D2>( p0, p1, p2 ); }

}} //namespace

#endif
