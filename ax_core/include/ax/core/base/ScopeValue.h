//
//  ScopeValue.h
//  ax_core
//
//  Created by Jason Chan on Mar 20, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_ScopeValue_h
#define ax_core_ScopeValue_h


namespace ax {
namespace System {

template< typename T >
class ScopeValue : public NonCopyable {
public:
	~ScopeValue	() 				{ unbind(); }

	ScopeValue	( T &p )						{ _bind( p ); }
	ScopeValue	( T &p, const T& new_value )	{ _bind( p ); operator=( new_value ); }
	ScopeValue	( ScopeValue && rhs ) 			{ move( rhs ); }
	
	void	operator=( const T& new_value ) { *_p = new_value; }
	
	const T&	value	()	{ return *_p; }
	const T&	oldValue()	{ return _old_value; }
	
	void	bind		( T& p )	{ unbind(); _bind(p); }
	void	unbind		()			{ if(_p) { *_p = _old_value; _p = nullptr; } }
	
	void	revert		()			{ if(_p) *_p  = _old_value; }
	void	save		()			{ if(_p) _old_value = *_p;  }

	void	move		( ScopeValue & rhs ) {
		_p = rhs._p;
		_old_value = rhs._old_value;
		
		rhs._p = nullptr;
		_old_value = T();
	}

private:
	void _bind( T& p )						{ _p = &p; _old_value = p; }

	T*	_p;
	T	_old_value;
};

template< typename T > inline
ScopeValue<T> ax_scope_value( T & v ) {
	return ScopeValue<T>(v);
}

template< typename T, typename R > inline
ScopeValue<T> ax_scope_value( T & v, const R & new_value ) {
	return ScopeValue<T>(v,new_value);
}


}} //namespace


#endif
