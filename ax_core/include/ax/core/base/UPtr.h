//
//  UPtr.h
//  ax_core
//
//  Created by Jason on 2015-03-20.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_UPtr_h
#define ax_core_UPtr_h

#define ax_uptr_new( T, ... )   ( ax::System::UPtr<T>::_Make( ax_new_uncollect(T,__VA_ARGS__) ) )

namespace ax {
namespace System {
/*
//! unique pointer
template< typename T >
class UPtr : public NonCopyable {
public:
	static	UPtr	_Make( T* p ) { return UPtr(p); }

	UPtr() : _p(nullptr) {}
	~UPtr() { release(); }
	
	UPtr( UPtr && rhs ) :_p(nullptr) 	{ move( rhs ); }
		
					T*		operator->	() 				{ return _p; }
			const 	T*		operator->	() const		{ return _p; }
	
					T*		ptr			()				{ return _p; }
			const	T*		ptr			() const		{ return _p; }

					T*		giveup		() 				{ auto p = _p; _p = nullptr; return p; }
					void	release		() 				{ if( _p ) { ax_delete_uncollect(_p); _p = nullptr; } }

					void 	operator=	( UPtr && rhs ) { move(rhs); }
					void	move		( UPtr &  rhs ) { release(); _p = rhs._p; rhs._p = nullptr; }
	
private:
	UPtr( T* p ) : _p(p) {}
	T*	_p;
};
*/

}} //namespace

#endif
