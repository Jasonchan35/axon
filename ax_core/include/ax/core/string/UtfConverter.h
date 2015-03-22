//
//  UtfConverter.h
//  ax_core
//
//  Created by Jason Chan on 3/16/15.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_UtfConverter_h
#define ax_core_UtfConverter_h

#include "../array/ArrayUtility.h"

namespace ax {
namespace System {

struct UtfConverter : public StaticClass {
	template< typename SRC, typename DST >
	static ax_int	GetConvertedCount ( const SRC* src, ax_int src_len ) {
		ax_int out_len = 0;
		auto e = src + src_len;
		for( ;src<e; src++ ) {
			out_len += _UtfCount<SRC>( _DecodeUtf( src, e ) );
		}
		return out_len;
	}

	template< typename SRC, typename DST >
	static void	Convert	( DST* dst, ax_int dst_len, const SRC* src, ax_int src_len  ) {
		auto e  = src + src_len;
		auto de = dst + dst_len;	
		for( ;src<e; src++, dst++ ) {
			_EncodeUtf( dst, de, _DecodeUtf( src, e ) );
		}
	}

private:
	template< typename SRC > ax_ALWAYS_INLINE( static ax_int	_UtfCount 	( ax_unichar v ) );
	template< typename SRC > ax_ALWAYS_INLINE( static uint32_t 	_DecodeUtf	( const SRC* & src, const SRC* e ) );
	template< typename DST > ax_ALWAYS_INLINE( static void 		_EncodeUtf	(       DST* & dst, const DST* e, ax_unichar v ) );

};

template<> inline
ax_unichar UtfConverter::_DecodeUtf<wchar_t>( const wchar_t* & src, const wchar_t* e ) {
	return *src;
}

template<> inline
ax_unichar UtfConverter::_DecodeUtf<char32_t>( const char32_t* & src, const char32_t* e ) {
	return *src;
}

template<> inline
ax_unichar UtfConverter::_DecodeUtf<char16_t>( const char16_t* & src, const char16_t* e ) {
	ax_unichar a = *src;
	
	if( a >= 0xD800 && a <= 0xDBFF ) {
		src++;
		if( src >= e ) throw Err_StringUTF();
				
		ax_unichar b = *src;
		
		if( b >= 0xDC00 && b <= 0xDFFF ) {
			return ( a << 10 ) + b - 0x35FDC00;
		}else{
			src--; // push back
//			throw Err_StringUTF();
//			Not UTF16 ?
		}
	}
	
	return a;
}

template<> inline
ax_unichar UtfConverter::_DecodeUtf<char>( const char* & src, const char* e ) {
	ax_unichar a = *src;
	
	if( ( a & 0x80 ) == 0 ) return a;
	
	if( ( a & 0xE0 ) == 0xC0 ) {
		if( src+2 > e ) throw Err_StringUTF();
		UInt32 b = *src; src++;
		
		return ( ( a & 0x1F ) << 6 ) | ( b & 0x3F );
	}
	
	if( ( a & 0xF0 ) == 0xE0 ) {
		if( src+3 > e ) throw Err_StringUTF();
		UInt32 b = *src; src++;
		UInt32 c = *src; src++;
	
		return ( (a & 0x0F) << 12 ) | ( (b & 0x3F) << 6 ) | ( c & 0x3F );
	}
	
	if( ( a & 0xF8 ) == 0xF0 ) {
		if( src+4 > e ) throw Err_StringUTF();
		UInt32 b = *src; src++;
		UInt32 c = *src; src++;
		UInt32 d = *src; src++;
	
		return ( (a & 0x07) << 18 ) | ( (b & 0x3F) << 12 ) | ( (c & 0x3F) << 6 ) | ( d & 0x3F );
	}
	
	
	if( ( a & 0xFC ) == 0xF8 ) {
		if( src+5 > e ) throw Err_StringUTF();
		UInt32 b = *src; src++;
		UInt32 c = *src; src++;
		UInt32 d = *src; src++;
		UInt32 e = *src; src++;
	
		return ( (a & 0x03) << 24 ) | ( (b & 0x3F) << 18 ) | ( (c & 0x3F) << 12 ) | ( (d & 0x3F) << 6 ) | ( e & 0x3F );
		
	}
	
	if( ( a & 0xFE ) == 0xFC ) {
		if( src+6 > e ) throw Err_StringUTF();
		UInt32 b = *src; src++;
		UInt32 c = *src; src++;
		UInt32 d = *src; src++;
		UInt32 e = *src; src++;
		UInt32 f = *src; src++;

		return ( (a & 0x01) << 30 ) | ( (b & 0x3F) << 24 ) | ( (c & 0x3F) << 18 ) | ( (d & 0x3F) << 12 ) | ( (e & 0x3F) << 6 ) | ( f & 0x3F );
	}

	throw Err_StringUTF();
	
//	it's not UTF8 ?
	return a;
}

template<> inline
ax_int UtfConverter::_UtfCount< wchar_t >( ax_unichar v ) {
	return 1;
}

template<> inline
ax_int UtfConverter::_UtfCount< char32_t >( ax_unichar v ) {
	return 1;
}

template<> inline
ax_int UtfConverter::_UtfCount< char16_t >( ax_unichar v ) {
	if( v <  0x10000 ) return 1;
	if( v < 0x110000 ) return 2;
	
	throw Err_StringUTF();
}

template<> inline
ax_int UtfConverter::_UtfCount< char >( ax_unichar v ) {
	if( v <       0x80 ) return 1;
	if( v <    0x00800 ) return 2;
	if( v <    0x10000 ) return 3;
	if( v <   0x200000 ) return 4;
// The patterns below are not part of UTF-8, but were part of the first specification.
	if( v <  0x4000000 ) return 5;
	if( v < 0x80000000 ) return 6;
	
	throw Err_StringUTF();
}

//template<> inline Int UtfConverter::_UtfCount< wchar_t >( UInt32 u ) { return _UtfCount< WCharAsCharXX >(u); }

template<> inline
void UtfConverter::_EncodeUtf< wchar_t >( wchar_t* & dst, const wchar_t* e, ax_unichar v ) {
	*dst = v;
}

template<> inline
void UtfConverter::_EncodeUtf< char32_t >( char32_t* & dst, const char32_t* e, ax_unichar v ) {
	*dst = v;
}

template<> inline
void UtfConverter::_EncodeUtf< char16_t >( char16_t* & dst, const char16_t* e, ax_unichar v ) {
	if( v <  0x10000 ) {
		*dst = v;
		return;
	}
	
	if( v < 0x110000 ) {
		*dst = ( v >> 10   ) + 0xD7C0; dst++;
		*dst = ( v & 0x3FF ) + 0xDC00;
		return;
	}
	
	throw Err_StringUTF();
}

template<> inline
void UtfConverter::_EncodeUtf< char >( char* & dst, const char* e, ax_unichar v ) {
	if( v <       0x80 ) {
		*dst = v;
		return;
	}
	
	if( v <    0x00800 ) {
		if( dst + 2 > e ) throw Err_StringUTF();
		*dst = ( v >> 6 ) | 0xC0; dst++;
		*dst = ( v & 0x3F ) | 0x80;
		return;
	}
		
	if( v <    0x10000 ) {
		if( dst + 3 > e ) throw Err_StringUTF();
		*dst = ( (v >> 12)        ) | 0xC0; dst++;
		*dst = ( (v >> 6 ) & 0x3F ) | 0x80; dst++;
		*dst = (  v        & 0x3F ) | 0x80;
		return;
	}
	
	if( v <   0x200000 ) {
		if( dst + 4 > e ) throw Err_StringUTF();
		*dst = ( (v >> 18)        ) | 0xC0; dst++;
		*dst = ( (v >> 12) & 0x3F ) | 0x80; dst++;
		*dst = ( (v >> 6 ) & 0x3F ) | 0x80; dst++;
		*dst = (  v        & 0x3F ) | 0x80;
		return;
	}
	
// The patterns below are not part of UTF-8, but were part of the first specification.
	if( v <  0x4000000 ) {
		if( dst + 5 > e ) throw Err_StringUTF();
		*dst = ( (v >> 24)        ) | 0xC0; dst++;
		*dst = ( (v >> 18) & 0x3F ) | 0x80; dst++;
		*dst = ( (v >> 12) & 0x3F ) | 0x80; dst++;
		*dst = ( (v >> 6 ) & 0x3F ) | 0x80; dst++;
		*dst = (  v        & 0x3F ) | 0x80;
		return;
	}
	
	if( v < 0x80000000 ) {
		if( dst + 6 > e ) throw Err_StringUTF();
		*dst = ( (v >> 30)        ) | 0xC0; dst++;
		*dst = ( (v >> 24) & 0x3F ) | 0x80; dst++;
		*dst = ( (v >> 18) & 0x3F ) | 0x80; dst++;
		*dst = ( (v >> 12) & 0x3F ) | 0x80; dst++;
		*dst = ( (v >> 6 ) & 0x3F ) | 0x80; dst++;
		*dst = (  v        & 0x3F ) | 0x80;
		return;
	}
	
	throw Err_StringUTF();
}

#define ax_TYPE_LIST_ITEM( NAME, T ) \
	template<> inline \
	ax_int	UtfConverter::GetConvertedCount< T,T > ( const T* src, ax_int src_len ) { return src_len; } \
	\
	template<> inline \
	void UtfConverter::Convert<T,T> ( T* dst, ax_int dst_len, const T* src, ax_int src_len  ) { ArrayUtility::Copy( dst, src, src_len ); } \
//-----
	ax_TYPE_LIST_all_char
#undef ax_TYPE_LIST_ITEM

}} //namespace



#endif
