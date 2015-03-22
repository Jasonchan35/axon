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
#include "../base/Obj.h"
#include "../array/ArrayUtility.h"

namespace ax {
namespace System {


#define ax_str8( sz )	( ax::System::StringX<char>    ::MakeExternal( u8##sz, sizeof( u8##sz ) / sizeof(char)     -1 ) )
#define ax_str16( sz )	( ax::System::StringX<char16_t>::MakeExternal(  u##sz, sizeof(  u##sz ) / sizeof(char16_t) -1 ) )
#define ax_str32( sz )	( ax::System::StringX<char32_t>::MakeExternal(  U##sz, sizeof(  U##sz ) / sizeof(char32_t) -1 ) )
#define ax_strW( sz )	( ax::System::StringX<wchar_t> ::MakeExternal(  L##sz, sizeof(  L##sz ) / sizeof(wchar_t)  -1 ) )

//! Immutable string	Literal / GC Memory
template< typename T >
class StringX /* copyable */{
public:

	ax_ALWAYS_INLINE(	const T &	operator[]	( ax_int  i ) const	) { return at(i); }
	ax_ALWAYS_INLINE(	const T &	at			( ax_int  i ) const	) { _checkBound(i); 	  return _data[i]; }
	ax_ALWAYS_INLINE( 	const T &	unsafe_at	( ax_int  i ) const	) { _debug_checkBound(i); return _data[i]; }
	ax_ALWAYS_INLINE( 	const T &	last		() 		 	  const	) { return at( _size-1 ); }
	ax_ALWAYS_INLINE( 	const T &	last		( ax_int  i ) const	) { return at( _size-i-1 ); }
	
	ax_ALWAYS_INLINE(		bool	inBound		( ax_int  i ) const	) { return i >= 0 && i < _size; }


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
	ax_ALWAYS_INLINE(	void	to_string( ToStringReq_<R> & req ) const );

	
	static	StringX	Make_c_str ( const T* sz ) 	{ return Make( sz, ax_strlen(sz) ); }
	static	StringX	Make ( const T* sz, ax_int len ) {
		if( ! sz || len == 0 ) return StringX();
		
		auto p = Memory::Alloc<T>( len + 1 );
		ArrayUtility::Copy( p, sz, len );
		p[len] = 0;
		
		return StringX(p,len);
	}
	
	static	StringX	MakeExternal_c_str( const T* sz ) { return MakeExternal(sz, ax_strlen(sz)); }
	static	StringX	MakeExternal( const T* sz, ax_int len ) {
		return StringX(sz,len);
	}
	

protected:

	const T*	_data;
	ax_int		_size;

	StringX() : _data(nullptr), _size(0) {}
	StringX( const T* sz, ax_int size ) : _data(sz),_size(size) {}

	ax_ALWAYS_INLINE( void 	_checkBound			( ax_int i ) const ) { if( ! inBound(i) ) throw Err_Array_OutOfRange(); }
	ax_ALWAYS_INLINE( void	_debug_checkBound	( ax_int i ) const ) {
		#if _DEBUG
			_checkBound(i);
		#endif
	}

};

typedef	StringX< ax_char >	String;

template< typename T > inline
std::ostream& operator<< ( std::ostream& o, const StringX<T>& v ) {
	o << v.c_str();
	return o;
}



}} //namespace

#endif
