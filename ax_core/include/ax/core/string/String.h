//
//  String.h
//  ax_core
//
//  Created by Jason on 2015-03-17.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_String_h
#define ax_core_String_h

#include "ax_c_str.h"
#include "ax_c_str_to.h"
#include "../base/Exception.h"
#include "../array/ArrayUtility.h"

namespace ax {
namespace System {

template< typename T > class MutStringX;

#define ax_txt8( sz )	( ax::System::StringX<char>    ::MakeExternal( u8##sz, sizeof( u8##sz ) / sizeof(char)     -1 ) )
#define ax_txt16( sz )	( ax::System::StringX<char16_t>::MakeExternal(  u##sz, sizeof(  u##sz ) / sizeof(char16_t) -1 ) )
#define ax_txt32( sz )	( ax::System::StringX<char32_t>::MakeExternal(  U##sz, sizeof(  U##sz ) / sizeof(char32_t) -1 ) )
#define ax_txtW( sz )	( ax::System::StringX<wchar_t> ::MakeExternal(  L##sz, sizeof(  L##sz ) / sizeof(wchar_t)  -1 ) )

//! Immutable string	Literal / GC Memory
template< typename T >
class StringX /* copyable */{
public:
	StringX() : _data(nullptr), _size(0) {}
	StringX( const MutStringX<T> & rhs ); // function body in MutString.h

	ax_ALWAYS_INLINE(	const T &	operator[]	( ax_int  i ) const	) { return at(i); }
	ax_ALWAYS_INLINE(	const T &	at			( ax_int  i ) const	) { _checkBound(i); 	  return _data[i]; }
	ax_ALWAYS_INLINE( 	const T &	unsafe_at	( ax_int  i ) const	) { _debug_checkBound(i); return _data[i]; }
	ax_ALWAYS_INLINE( 	const T &	last		() 		 	  const	) { return at( _size-1 ); }
	ax_ALWAYS_INLINE( 	const T &	last		( ax_int  i ) const	) { return at( _size-i-1 ); }
	
	ax_ALWAYS_INLINE(		bool	inBound		( ax_int  i ) const	) { return i >= 0 && i < _size; }

	bool	operator==( const StringX & rhs ) const {
		if( _data == rhs._data && _size == rhs._size ) return true;
		return ax_str_equals( _data, rhs._data );
	}

	ax_int		size	() const				{ return _size; }
		
	const T*	c_str	() const 				{ return _data ? _data : ax_empty_c_str<T>(); }
	const T*	c_str	( ax_int offset ) const {
		if( !_data ) {
			if( offset != 0 ) throw  Err_Array_OutOfRange();
			return ax_empty_c_str<T>();
		}
		return & at(offset);
	}

	template< typename R >
	ax_ALWAYS_INLINE(	void	OnStringReq( ToStringReq_<R> & req ) const );

	
	static	StringX	Make_c_str ( const T* sz ) 	{ return Make( sz, ax_strlen(sz) ); }
	static	StringX	Make ( const T* sz, ax_int len ) {
		StringX	o;
		o._dup( sz, len );
		return o;
	}
	
	static	StringX	MakeExternal_c_str( const T* sz ) { return MakeExternal(sz, ax_strlen(sz)); }
	static	StringX	MakeExternal( const T* sz, ax_int len ) {
		return StringX(sz,len);
	}
	
	ax_int	GetHash() const { return ax_c_str_hash(_data); }
	
protected:

	const T*	_data;
	ax_int		_size;

	StringX( const T* sz, ax_int size ) : _data(sz),_size(size) {}
	
	void	_dup( const T* sz, ax_int len ) {
		if( ! sz || len == 0 ) {
			_data = nullptr;
			_size = 0;
		}else{
			auto p = Memory::Alloc<T>( len + 1 );
			ArrayUtility::Copy( p, sz, len );
			p[len] = 0;
			_data = p;
			_size = len;
		}
	}

	ax_ALWAYS_INLINE( void 	_checkBound			( ax_int i ) const ) { if( ! inBound(i) ) throw Err_Array_OutOfRange(); }
	ax_ALWAYS_INLINE( void	_debug_checkBound	( ax_int i ) const ) {
		#if _DEBUG
			_checkBound(i);
		#endif
	}

};

typedef	StringX< ax_char >	String;
typedef StringX< char >		StringA;

template< typename T > inline
std::ostream& operator<< ( std::ostream& o, const StringX<T>& v ) {
	o << v.c_str();
	return o;
}



}} //namespace

#endif
