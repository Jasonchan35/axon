//
//  ArrayEnumerator.h
//  ax_core
//
//  Created by Jason on 2015-03-20.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_ArrayEnumerator_h
#define ax_core_ArrayEnumerator_h



namespace ax {
namespace System {


template< typename ARRAY, typename T >
class	ArrayEnumerator {
public:
	class	Iterator {
	public:
		Iterator( T* p=nullptr ) : _p(p) {}
		operator const T*	()  { return  _p; }
		T &		operator*	()	{ return *_p; }
		void	operator++	()	{ ++_p; }
		bool	operator==	( const Iterator & rhs )	{ return _p == rhs._p; }
		bool	operator!=	( const Iterator & rhs )	{ return _p != rhs._p; }
	private:
		T*	_p;
	};

	Iterator	begin	()	const	{ return Iterator(_begin); }
	Iterator	end		()	const	{ return Iterator(_end);   }

	ArrayEnumerator( const ARRAY & arr, T* begin, T* end ) : _begin( begin ), _end( end ) {
		_arr = ax_const_cast( & arr );
	#if ax_DEBUG_Enumerating
		++_arr->_enumerating;
	#endif
	}
	
	~ArrayEnumerator() {
	#if ax_DEBUG_Enumerating
		--_arr->_enumerating;
	#endif
	}

private:
	T*		_begin;
	T*		_end;
	ARRAY* 	_arr;
};


template< typename ARRAY, typename T >
class	ArrayInverseEnumerator {
public:
	class	Iterator {
	public:
		Iterator( T* p=nullptr ) : _p(p) {}
		operator const T*	()  { return  _p; }
		T &		operator*	()	{ return *_p; }
		void	operator++	()	{ --_p; }
		bool	operator==	( const Iterator & rhs )	{ return _p == rhs._p; }
		bool	operator!=	( const Iterator & rhs )	{ return _p != rhs._p; }
	private:
		T*	_p;
	};

	Iterator	begin	()	const	{ return Iterator(_begin); }
	Iterator	end		()	const	{ return Iterator(_end);   }

	ArrayInverseEnumerator( const ARRAY & arr, T* begin, T* end ) {
		_arr = ax_const_cast( & arr );
		++_arr->_enumerating;
		
		if( begin && end ) {
			_begin = end   - 1;
			_end   = begin - 1;
		}else{
			_begin = nullptr;
			_end   = nullptr;
		}
	}
	
	~ArrayInverseEnumerator() {
		--_arr->_enumerating;
	}

private:
	T*		_begin;
	T*		_end;
	ARRAY* 	_arr;
};

}} //namespace

#endif
