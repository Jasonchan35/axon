//
//  Ptr.h
//  ax_core
//
//  Created by Jason on 2015-03-15.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_Ptr_h
#define ax_core_Ptr_h

namespace ax {
namespace System {

template< typename T >
class Ptr {
public:
	struct	ax_type_on_gc_trace : public std::true_type {};
			

							Ptr( T* p = nullptr ) : _p( p ) {}
	template< typename R >	Ptr( R* p ) : _p( p ) {}

	template< typename R >	Ptr( Ptr<R> & p ) : _p( p.ptr() ) {}

	void move( T & rhs ) {
		_p = rhs._p;
		rhs._p = nullptr;
	}

	template< typename R > void operator=( R* p ) { _p = p; }


	operator 		T* 	() 			{ return _p; }
	operator const 	T*	() const	{ return _p; }
	
			void		setPtr( T* p )	{ _p = p; }
	
					T*	ptr()		{ return _p; }
			const	T*	ptr() const	{ return _p; }

					T&	deref()			{ return *_p; }
			const	T&	deref() const	{ return *_p; }

					T&	operator* 	() 			{ return *_p; }
			const 	T&	operator*	() const	{ return *_p; }

					T*	operator->	() 			{ return  _p; }
			const 	T*	operator->	() const	{ return  _p; }

//	template< typename R >
//	void	to_string( ToStringRequest_<R> & req ) const;
private:
	T*	_p;
};

template< typename T > inline	Ptr<T>  Ptr_Make ( T* p ) { return Ptr<T> ( p ); }

}} //namespace

#endif
