//
//  Locked.h
//  ax_core
//
//  Created by Jason Chan on 12/1/14.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_Locked_h
#define ax_core_Locked_h

namespace ax {
namespace System {

template< typename T0, typename T1 = void, typename T2 = void >
class Locked : public NonCopyable {
public:
	Locked( T0* p0 = nullptr, T1 *p1 = nullptr, T2 *p2 = nullptr ) : _p0(p0), _p1(p1), _p2(p2) {
		if( p0 < p1 ) {
			if( p2 < p0 ) {
			//	201
				if( p2 ) p2->lock();
				if( p0 ) p0->lock();
				if( p1 ) p1->lock();
			}else if( p1 < p2 ) {
			//	012
				if( p0 ) p0->lock();
				if( p1 ) p1->lock();
				if( p2 ) p2->lock();
			}else{
			//	021
				if( p0 ) p0->lock();
				if( p2 ) p2->lock();
				if( p1 ) p1->lock();
			}
		}else{ // p1 < p0
			if( p2 < p1 ) {
			//	210
				if( p2 ) p2->lock();
				if( p1 ) p1->lock();
				if( p0 ) p0->lock();
			}else if( p0 < p2 ) {
			//	102
				if( p1 ) p1->lock();
				if( p0 ) p0->lock();
				if( p2 ) p2->lock();
			}else{
			//	120
				if( p1 ) p1->lock();
				if( p2 ) p2->lock();
				if( p0 ) p0->lock();
			}
		}
	}
	
	Locked( Locked && rhs ) {
		_p0 = rhs._p0;
		_p1 = rhs._p1;
		_p2 = rhs._p2;
		rhs._p0 = nullptr;
		rhs._p1 = nullptr;
		rhs._p2 = nullptr;
	}
	
	~Locked() {
		if( _p0 ) { _p0->unlock(); _p0 = nullptr; }
		if( _p1 ) { _p1->unlock(); _p1 = nullptr; }
		if( _p2 ) { _p2->unlock(); _p2 = nullptr; }
	}
private:
	T0*	_p0;
	T1*	_p1;
	T1*	_p2;
};

template< typename T0, typename T1 >
class Locked<T0,T1> : public NonCopyable {
public:
	Locked( T0* p0 = nullptr, T1 *p1 = nullptr ) : _p0(p0), _p1(p1) {
		if( p0 < p1 ) {
			if( p0 ) p0->lock();
			if( p1 ) p1->lock();
		}else{
			if( p1 ) p1->lock();
			if( p0 ) p0->lock();
		}
	}
	
	Locked( Locked && rhs ) {
		_p0 = rhs._p0;
		_p1 = rhs._p1;
		rhs._p0 = nullptr;
		rhs._p1 = nullptr;
	}
	
	~Locked() {
		if( _p0 ) { _p0->unlock(); _p0 = nullptr; }
		if( _p1 ) { _p1->unlock(); _p1 = nullptr; }
	}
private:
	T0*	_p0;
	T1*	_p1;
};

template< typename T0 >
class Locked<T0> : public NonCopyable {
public:
	Locked( T0* p0 = nullptr ) : _p0(p0) {
		if( p0 ) p0->lock();
	}
	
	Locked( Locked && rhs ) {
		_p0 = rhs._p0;
		rhs._p0 = nullptr;
	}
	
	~Locked() {
		if( _p0 ) { _p0->unlock();	_p0 = nullptr; }
	}
private:
	T0*	_p0;
};

template< typename T0, typename T1, typename T2 > inline Locked<T0,T1,T2> ax_locked( T0 & lock0, T1 & lock1, T2 & lock2 )	{ return Locked<T0,T1,T2>( &lock0, &lock1, &lock2 ); }
template< typename T0, typename T1 > 			  inline Locked<T0,T1>	  ax_locked( T0 & lock0, T1 & lock1 ) 				{ return Locked<T0,T1>	 ( &lock0, &lock1 ); }
template< typename T0 > 						  inline Locked<T0> 	  ax_locked( T0 & lock0 ) 							{ return Locked<T0>		 ( &lock0 ); }

} //namespace System
} //namespace ax


#endif
