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
#include "UtfConverter.h"

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
	struct	ax_type_on_gc_trace : public std::true_type {};

	StringX() : _data(nullptr), _size(0) {}

	ax_ALWAYS_INLINE(	const T &	operator[]	( ax_int  i ) const	) { return at(i); }
	ax_ALWAYS_INLINE(	const T &	at			( ax_int  i ) const	) { _checkBound(i); 	  return _data[i]; }
	ax_ALWAYS_INLINE( 	const T &	unsafe_at	( ax_int  i ) const	) { _debug_checkBound(i); return _data[i]; }
	ax_ALWAYS_INLINE( 	const T &	last		() 		 	  const	) { return at( _size-1 ); }
	ax_ALWAYS_INLINE( 	const T &	last		( ax_int  i ) const	) { return at( _size-i-1 ); }
	
	ax_ALWAYS_INLINE(		bool	inBound		( ax_int  i ) const	) { return i >= 0 && i < _size; }

	bool	operator==( const StringX & rhs ) const { return equals(rhs); }
	
	bool	equals( const StringX & rhs ) const {
		if( _data == rhs._data && _size == rhs._size ) return true;
		return ax_str_equals( _data, rhs._data );
	}
	
	bool	equals( const StringX & rhs, bool ignoreCase ) const {
		if( _data == rhs._data && _size == rhs._size ) return true;
		if( ignoreCase ) {
			return ax_str_case_equals( _data, rhs._data );
		}else{
			return ax_str_equals( _data, rhs._data );
		}
	}

	StringX	operator+( const StringX<T> & rhs ) const {
		if( rhs.size() == 0 ) return *this;
		
		auto len = _size + rhs.size();
		auto p = Memory::Alloc<T>( len + 1 );
		ArrayUtility::Copy( p, _data, _size );
		ArrayUtility::Copy( p+_size, rhs.c_str(), rhs.size() );
		p[len] = 0;
		return StringX::MakeExternal( p, len );
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

	static	T*	AllocBuffer( ax_int size ) {
		if( size == 0 ) return nullptr;
		
		auto buf = Memory::Alloc<T>( size + 1 );
		buf[size] = 0;
		return buf;
	}

	template< typename R >
	static	StringX	CloneUtf_c_str ( const R* sz ) 	{
		return CloneUtf( sz, ax_strlen( sz ) );
	}

	template< typename R >
	static	StringX	CloneUtf ( const R* sz, ax_int size ) 	{
		auto req_len = UtfConverter::GetConvertedCount<R,T>( sz, size );
		auto buf = AllocBuffer( req_len );				
		UtfConverter::Convert( buf, req_len, sz, size );
		return StringX( buf, req_len );
	}
	
	static	StringX	Clone_c_str ( const T* sz ) 	{ return Clone( sz, ax_strlen(sz) ); }
	static	StringX	Clone ( const T* sz, ax_int len ) {
		auto buf = AllocBuffer( len );
		ArrayUtility::Copy( buf, sz, len );
		return StringX( buf, len );
	}
	
	static	StringX	MakeExternal_c_str( const T* sz ) { return MakeExternal(sz, ax_strlen(sz)); }
	static	StringX	MakeExternal( const T* sz, ax_int len ) {
		return StringX(sz,len);
	}
	
	ax_int	GetHash() const { return ax_c_str_hash(_data); }
	
protected:

	const T*	_data;
	ax_int		_size;

	StringX( const T* sz, ax_int size ) : _data(sz),_size(size) {
		if( size < 0 ) throw Err_Undefined();
		if( size > 0 && sz[size] != 0 ) throw Err_Undefined();
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
