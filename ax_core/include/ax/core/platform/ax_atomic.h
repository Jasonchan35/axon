//
//  ax_atomic.h
//  ax_core
//
//  Created by Jason on 2015-03-20.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_ax_atomic_h
#define ax_core_ax_atomic_h


namespace ax {
namespace System {

#if 1 // thread-safe
template< typename T >
class AtomicNumber : private std::atomic<T>, public NonCopyable {
	typedef	std::atomic<T> base;
public:
	AtomicNumber( T v = 0 ) : base(v) {}
	
	ax_ALWAYS_INLINE(	T		get 		() const		)	{ return base::load(); }
	ax_ALWAYS_INLINE(			operator T	() const		)	{ return get(); }
	ax_ALWAYS_INLINE(	T		operator++	() 				)	{ return ++getBase(); }
	ax_ALWAYS_INLINE(	T		operator--	()				)	{ return --getBase(); }
	
	ax_ALWAYS_INLINE(	void	operator=	( const T& v )	)	{ base::store(v); }
	
private:
	base& getBase() { return *this; }
};

#else

template< typename T >
class AtomicNumber : public NonCopyable {
public:
	AtomicNumber( T v = 0 ) : _v(0) {}

	ax_ALWAYS_INLINE(			operator T 	() const		)	{ return _v; }
	ax_ALWAYS_INLINE(	T 		operator++	() 				)	{ return ++_v; }
	ax_ALWAYS_INLINE(	T		operator--	()				)	{ return --_v; }
	ax_ALWAYS_INLINE(	void	operator=	( const T& v )	)	{ _v = v; }

private:
	T	_v;
};

#endif

typedef	AtomicNumber< ax_int >	RefCount;

//typedef AtomicNumber< ax_int >	DebugEnumeratingCounter;
typedef ax_int	DebugEnumeratingCounter;

}} //namespace

#endif
