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

#define	ax_obj_new( T, ... )	( ax::System::Obj<T>::_NewObject( ax_new(T,__VA_ARGS__) ) )

namespace ax {
namespace System {

class Object : public NonCopyable {
public:
	virtual	~Object() {
	}
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
					T*	ptr()		{ return _p; }
			const	T*	ptr() const	{ return _p; }

					T&	operator* 	() 			{ return *_p; }
			const 	T&	operator*	() const	{ return *_p; }

					T*	operator->	() 			{ return  _p; }
			const 	T*	operator->	() const	{ return  _p; }

	static	Obj		_NewObject( T* p ) { Object_RegisterFinalizer(p); return Obj(p); }

//	template< typename R >
//	void	to_string( ToStringRequest_<R> & req ) const;
private:

	Obj( T* p ) : _p(p) {}

	T*	_p;
};

}} //namespace

#endif
