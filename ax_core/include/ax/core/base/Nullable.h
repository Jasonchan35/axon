//
//  Nullable.h
//  axon_core
//
//  Created by Jason on 2014-12-06.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef axon_core_Nullable_h
#define axon_core_Nullable_h

#include "../platform/platform.h"
#include "../array/ArrayUtility.h"

namespace ax {
namespace System {

template< typename T >
class Nullable : public NonCopyable {
public:
	Nullable() {}
	Nullable( std::nullptr_t t ) {}

	Nullable( const Nullable &  rhs ) 	{ operator=(rhs); }
	
	template< typename R >
	Nullable( const R & v ) 			{ operator=( v ); }

			T		get			( T defaultValue ) const 	{ return _p ? *_p : defaultValue; }
			
			bool	tryGet		( T& v ) const				{ if( !_p ) return false;  v = *_p; return true; }
			
			T		unsafeGet	() 							{ _checkNull(); return *_p; }
		 	T		unsafeGet	() const 					{ _checkNull(); return *_p; }

			bool	is_null		() const { return _p == nullptr; }
			bool	not_null	() const { return _p != nullptr; }

			bool 	operator==	( std::nullptr_t t ) const { return _p == nullptr; }
			bool 	operator!=	( std::nullptr_t t ) const { return _p != nullptr; }
			
			bool	operator==	( const T & rhs ) const {
				if( _p == nullptr ) return false;
				return *_p == rhs;
			}
			
			bool	operator==	( const Nullable & rhs ) const {
				if( _p == rhs._p ) return true;
				if( _p && rhs._p ) return *_p == *rhs._p;
				return false;
			}
			
			bool	operator!=	( const T		 & rhs ) const { return ! operator==(rhs); }
			bool	operator!=	( const Nullable & rhs ) const { return ! operator==(rhs); }

	void operator=( const Nullable & rhs ) {
		setNull();
		if( rhs._p ) {
			operator=( *rhs._p );
		}
	}

	void operator=( std::nullptr_t p ) { setNull(); }	
	void setNull() {
		if( _p ) {
			ax_call_destructor( T, _p );
			ArrayUtility::SetAllZeroForGC( _buf, sizeof(T) );
		}
		_p = nullptr;
	}
	
	template< typename R >
	void operator=( const R & v ) {
		if( _p ) {
			*_p = v;
		}else{
			_p = reinterpret_cast<T*>( _buf );
			ax_call_constructor( T, _p, v );
		}
	}
	
	template< typename R >
	void	OnStringReq( ToStringReq_<R> & req ) const {
		if( _p == nullptr ) {
			req << ax_txt("null");
		}else{
			req << *_p;
		}
	}
	
	
private:
	void _checkNull() const { if( ! _p ) throw Err_Undefined(); }

	Ptr<T>	_p;
	char	_buf[ sizeof(T) ];
};

}} //namespace

template< typename T > inline
ax_Nullable<T>	ax_make_nullable( const T & v ) { return ax_Nullable<T>( v ); }

#endif
