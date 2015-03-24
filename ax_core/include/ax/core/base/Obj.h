//
//  Obj.h
//  ax_core
//
//  Created by Jason Chan on Mar 17, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_Obj_h
#define ax_core_Obj_h 

#include "Memory.h"
#include "Nullable.h"
#include "../string/String.h"

#define	ax_new_obj( T, ... )	( ax::System::Obj<T>::_NewObject( ax_new(T,__VA_ARGS__) ) )
#define ax_ThisObj				( ax::System::Obj_FromPtr(this) )

#define ax_DefObject( T, BASE ) \
	private:\
		typedef BASE	base; \
	public: \
		typedef	T		THIS_CLASS; \
	private: \
//-------------


namespace ax {
namespace System {

template< typename T > class Obj;

class Object : public NonCopyable {
public:
	virtual	~Object() {
	}
	
	template< typename R >	Nullable< Obj<R> >	ax_as	();
	template< typename R >	Obj<R>				ax_cast	();
	template< typename R >	bool				ax_is	();
};

inline void Object_OnFinalize( void* obj, void* clientData ) {
	static_cast< Object* >( obj )->~Object();
}

inline
void Object_RegisterFinalizer( Object* p ) {
	Memory::GC_RegisterFinalizer( p, &Object_OnFinalize, nullptr );
}

template< typename T >
class Obj {
public:
	struct	ax_type_on_gc_trace : public std::true_type {};
	
	Obj	() : _p(nullptr) {}
	Obj	( const Obj<T> & rhs ) { _p = ax_const_cast( rhs.ptr() ); }
	
	template< typename R >
	Obj	( const Obj<R> & rhs ) { _p = ax_const_cast( rhs.ptr() ); }
	
	template< typename R >
	void operator=	( const Obj<R> & rhs ) 	{ _p = ax_const_cast( rhs.ptr() ); }
	
	
					T*	ptr()		{ return _p; }
			const	T*	ptr() const	{ return _p; }

					T&	operator*	() 			{ return *_p; }
			const 	T&	operator*	() const	{ return *_p; }

					T*	operator->	() 			{ return  _p; }
			const 	T*	operator->	() const	{ return  _p; }

	static	Obj		_NewObject( T* p ) { Object_RegisterFinalizer(p); return Obj(p); }
	static	Obj		_FromPtr( T* p )   { return Obj(p); }
	
	template< typename R >
	void	OnStringReq( ToStringReq_<R> & req ) const {
		if( _p == nullptr ) {
			req << ax_txt("null");
		}else{
			_p->OnStringReq(req);
		}
	}
	
private:
	Obj( T* p ) : _p(p) { _checkIsObject(p); }

	static void _checkIsObject( Object* p ) {}

	T*	_p;
};

template< typename T > inline
Obj<T> Obj_FromPtr( T* p ) { return Obj<T>::_FromPtr(p); }


template< typename T > using NullableObj = Nullable< Obj<T> >;

}} //namespace

#endif
