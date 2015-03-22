//
//  ax_c_str.h
//  ax_core
//
//  Created by Jason Chan on 3/16/15.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_ax_c_str_h
#define ax_core_ax_c_str_h

#include "../platform/platform.h"


template< typename T > const T* 	ax_empty_c_str();
template<> inline const char* 		ax_empty_c_str() { return  ""; }
template<> inline const char16_t*	ax_empty_c_str() { return u""; }
template<> inline const char32_t*	ax_empty_c_str() { return U""; }
template<> inline const wchar_t*	ax_empty_c_str() { return L""; }

template< typename T > const T* 	ax_HEX_chars();
template< typename T > const T* 	ax_hex_chars();

template<> inline const char*		ax_HEX_chars<char>() 		{ return  "0123456789ABCDEF"; }
template<> inline const char*		ax_hex_chars<char>() 		{ return  "0123456789abcdef"; }

template<> inline const char16_t*	ax_HEX_chars<char16_t>() 	{ return u"0123456789ABCDEF"; }
template<> inline const char16_t*	ax_hex_chars<char16_t>() 	{ return u"0123456789abcdef"; }

template<> inline const char32_t*	ax_HEX_chars<char32_t>() 	{ return U"0123456789ABCDEF"; }
template<> inline const char32_t*	ax_hex_chars<char32_t>() 	{ return U"0123456789abcdef"; }

template<> inline const wchar_t*	ax_HEX_chars<wchar_t>() 	{ return L"0123456789ABCDEF"; }
template<> inline const wchar_t*	ax_hex_chars<wchar_t>() 	{ return L"0123456789abcdef"; }

template<class T> inline
bool ax_ishex( T ch ) {
	if( ch >= '0' && ch <='9' ) return true;
	if( ch >= 'A' && ch <='F' ) return true;
	if( ch >= 'a' && ch <='f' ) return true;
	return false;
}

template<class T> inline
T ax_char_escape( T c ) {
	switch( c ) {
		case 'n': return '\n';
		case 'r': return '\r';
		case 'b': return '\b';
		case 't': return '\t';
	}
	return c;
}

template< typename T > inline bool	ax_isupper( T  ch ) { return ( ch>='A' && ch <= 'Z' ); }
template< typename T > inline bool	ax_islower( T  ch ) { return ( ch>='a' && ch <= 'z' ); }

template< typename T > inline bool	ax_isalpha( T  ch ) { return ax_isupper(ch) || ax_islower(ch); }
template< typename T > inline bool	ax_isdigit( T  ch ) { return ( ch >= '0' && ch <='9' ); }

template< typename T > inline T		ax_toupper( T  ch ) { return ax_islower(ch) ? (ch-('a'-'A')) : ch; }
template< typename T > inline T		ax_tolower( T  ch ) { return ax_isupper(ch) ? (ch+('a'-'A')) : ch; }


#define ax_TYPE_LIST_ITEM( NAME, T ) \
/* standard C string function */ \
			ax_int	ax_strlen			( const T*  sz ); \
			ax_int	ax_strnlen			( const T*  sz, ax_int len ); \
	\
			T*		ax_strcasestr		(       T*  big, const T*   little ); \
	const	T*		ax_strcasestr		( const T*  big, const T*   little ); \
	\
			ax_int	ax_strcasecmp 		( const T*	s1,  const T*	s2 ); \
			ax_int	ax_strncasecmp		( const T*	s1,  const T*	s2, ax_int n ); \
			ax_int	ax_strcmp			( const T*  s1,  const T*   s2 ); \
			ax_int	ax_strncmp			( const T*  s1,  const T*   s2, ax_int n ); \
	\
			T*		ax_strcpy			( 		T*  dst, const T*   src ); \
			T*		ax_strncpy			( 		T*  dst, const T*   src, ax_int n ); \
			T*		ax_strstr			(       T*  a,   const T*   b ); \
	const	T*		ax_strstr			( const T*  a,   const T*   b ); \
			T*		ax_strrstr			(       T*  a,   const T*   b ); \
	const 	T*		ax_strrstr			( const T*  a,   const T*   b ); \
	\
			T*		ax_strcasechr		(       T*  sz, T   ch ); \
	const	T* 		ax_strcasechr		( const T*	sz, T	ch ); \
			T*		ax_strchr			(       T*  sz, T   ch ); \
	const	T*		ax_strchr			( const T*  sz, T   ch ); \
			T*		ax_strrchr			(       T*  sz, T   ch ); \
	const	T*		ax_strrchr			( const T*  sz, T   ch ); \
	\
/* our additional C String function */	\
			ax_int	ax_c_str_hash		( const T *sz ); \
	\
	inline	bool	ax_str_equals		( const T*  s1,  const T*   s2 )		{ return 0 == ax_strcmp(s1,s2); } \
	inline	bool	ax_str_case_equals	( const T*  s1,  const T*   s2 )		{ return 0 == ax_strcasecmp(s1,s2); } \
			bool	ax_str_has_prefix	( const T*  sz,  const T*   prefix ); \
			bool	ax_str_has_suffix	( const T*  sz,  const T*   suffix ); \
	\
			T*		ax_strchr_list		(       T*	sz,  const T*	chrs ); \
	const	T*		ax_strchr_list		( const T*  sz,  const T*	chrs ); \
			T*		ax_strrchr_list		(       T*	sz,  const T*	chrs ); \
	const 	T*  	ax_strrchr_list		( const T*	sz,  const T*	chrs ); \
	\
//--------
	ax_TYPE_LIST_all_char
#undef	ax_TYPE_LIST_ITEM


template< typename T > inline
bool	_ax_str_has_prefix( const T* full, const T* prefix ) {
	if( ! full || ! prefix ) return false;

	ax_int	prefix_len = ax_strlen( prefix );
	return 0 == ax_strncmp( prefix, full, prefix_len );
}

template< typename T > inline
bool	_ax_str_has_suffix( const T* full, const T* suffix ) {
	if( ! full || ! suffix ) return false;

	ax_int	suffix_len = ax_strlen( suffix );
	ax_int	full_len   = ax_strlen( full );

	if( full_len < suffix_len ) return false;
	ax_int n = full_len - suffix_len;
	const T* p = full + n;
	return 0 == ax_strncmp( suffix, p, suffix_len );
}

template< typename T > inline
ax_int _ax_strnlen( const T* s, ax_int len ) {
	if( !s ) return 0;
	
	ax_int l=0;
	for(;;) {
		if( len <= l ) return len;
		if( *s == 0 ) return l;
		l++;
		s++;
	}
	return len;
}

//! search char(s) in string
template< typename T > inline
T* _ax_strchr_list( T* sz, T* chrs ) {
	if( !sz || ! chrs ) return nullptr;

	T* p = sz;
	for( ; *p;	p++ ) {
		for( T* c=chrs; *c; c++ ) {
			if( *p == *c ) return p;
		}
	}
	return nullptr;
}

//! search char(s) in string from right side
template< typename T > inline
T* _ax_strrchr_list( T* sz, T* chrs ) {
	if( !*sz || ! chrs ) return nullptr;

	ax_int len = ax_strlen( sz );
	T* p = sz + len - 1;
	for( ; p >= sz; p-- ) {
		for( T* c=chrs; *c; c++ ) {
			if( *p == *c ) return p;
		}
	}
	return nullptr;
}

template< typename T > inline
ax_int	 _ax_strcasecmp ( const T* s1, const T* s2 ) {
	if( s1 == s2 ) return 0;
	if( ! s1 ) return -1;
	if( ! s2 ) return  1;

	for( ;; s1++, s2++ ) {
		T c1 = ax_toupper(*s1);
		T c2 = ax_toupper(*s2);
		if( c1 != c2 ) return (c1-c2);
		if( c1 == 0 || c2 == 0 ) break;
	}
	return 0;
}

template< typename T > inline
ax_int	 _ax_strncasecmp( const T* s1, const T* s2, ax_int n ) {
	if( s1 == s2 ) return 0;
	if( ! s1 ) return -1;
	if( ! s2 ) return  1;
	
	ax_int i;
	for( i=0; i<n; s1++, s2++, i++ ) {
		T c1 = ax_toupper(*s1);
		T c2 = ax_toupper(*s2);
		if( c1 != c2 ) return (c1-c2);
		if( c1 == 0 || c2 == 0 ) break;
	}
	return 0;
}

template< typename T > inline
T* _ax_strcasechr( T* sz, T ch ) {
	if( ! sz ) return nullptr;
	
	T c = ax_toupper(ch);
	for(;*sz;sz++) {
		if( ax_toupper(*sz) == c ) return sz;
	}
	return nullptr;
}

template< typename T > inline
T* _ax_strcasestr( T* big, T* little ) {
	if( ! big || ! little ) return nullptr;

	ax_int big_len    = ax_strlen( big );
	ax_int little_len = ax_strlen( little );
	
	if( little_len > big_len ) return nullptr;
	
	ax_int n = big_len - little_len;
	ax_int i;
	for( i=0; i<=n; i++ ) {
		if( ax_strncasecmp( big+i, little, little_len ) == 0 )
			return big+i;
	}
	return nullptr;
}

template< typename T > inline
T*	_ax_strchr( T* sz, T ch ) {
	if( ! sz ) return nullptr;

	for( ; *sz; sz++ ) {
		if( *sz == ch ) return sz;
	}
	return nullptr;
}

template< typename T > inline
ax_int	 _ax_strcmp ( const T* s1, const T* s2 ) {
	if( s1 == s2 ) return 0;
	if( !s1 ) return -1;
	if( !s2 ) return  1;
	
	for( ;; s1++, s2++ ) {
		if( *s1 != *s2 ) return (*s1-*s2);
		if( *s1 == 0 || *s2 == 0 ) break;
	}
	return 0;
}

template< typename T > inline
ax_int	 _ax_strncmp( const T* s1, const T* s2, ax_int n ) {
	if( s1 == s2 ) return 0;
	if( !s1 ) return -1;
	if( !s2 ) return  1;
	
	ax_int i;
	for( i=0; i<n; s1++, s2++, i++ ) {
		if( *s1 != *s2 ) return (*s1-*s2);
		if( *s1 == 0 || *s2 == 0 ) break;
	}
	return 0;
}

template< typename T > inline
T* _ax_strcpy( T* dst, const T* src ) {
	if( !dst || !src ) return nullptr;

	T *ret = dst;
	while ((*dst++ = *src++));
	return ret ;
}

template< typename T > inline
T* _ax_strncpy( T* dst, const T* src, ax_int n ) {
	if( !dst || !src ) return nullptr;
	
	ax_int i;
	T* ret = dst;
	for( i=0; i<n; i++ ) {
		if( *src == 0 ) break;
		*dst = *src;
		dst++; src++;
	}	
	*dst = 0;
	return ret;
}

template< typename T > inline
T* _ax_strrchr( T* sz, T ch ) {
	if( !sz ) return nullptr;

	ax_int len = ax_strlen( sz );
	if( len == 0 ) return nullptr;
	T *s = &sz[ len-1 ];
	for( ; s > sz ; s-- ) {
		if( *s == ch ) return s;
	}
	return nullptr;
}

template< typename T > inline
T* _ax_strstr( T* a, T* b ) {
	if( !a || !b ) return nullptr;

	T* sa = a;
	T* sb = b;
	if( *b == 0 ) return nullptr;
	for( ; *a ; a++ ) {
		sa = a;
		sb = b;
		for(;;) {
			if( *sb == 0   ) return a; //found
			if( *sb != *sa ) break;
			sa++; sb++;
		}
	}
	return nullptr;
}

template< typename T > inline
T* _ax_strrstr( T* a, T* b ) {
	if( !a || !b ) return nullptr;

	ax_int alen, blen;

	T *p;

	alen = ax_strlen( a );
	blen = ax_strlen( b );
	
	if ( blen > alen ) return nullptr;

	for ( p = a + alen - blen; p >= a; p-- ) {
		if ( ax_strncmp( p, b, blen ) == 0 ) {
			return p;
		}
	}

	return nullptr;
}

template< typename T > inline
ax_int _ax_c_str_hash( const T *sz ) {
	if( ! sz ) return ax_int(877);
	/*
	"djb2" string hash
	this algorithm (k=33) was first reported by dan bernstein many years ago
	in comp.lang.c. another version of this algorithm (now favored by bernstein)
	uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33
	(why it works better than many other constants, prime or not) has never
	been adequately explained.
	*/
    ax_int h( 5381 );
	for( ; *sz; sz++) {
        h = ( ( h << 5 ) + h ) + (ax_int)(*sz); // h * 33 + c
	}
	return h;
}

template< typename T > inline
ax_int _ax_strlen( const T* s ) {
	if( !s ) return 0;
	
	ax_int len = 0;
	for( ; *s; s++, len++ ) {
	}
	return len;
}


#define ax_TYPE_LIST_ITEM( NAME, T ) \
	inline			bool	ax_str_has_prefix	( const T*    sz, const T*    prefix )		{ return _ax_str_has_prefix(sz,prefix); } \
	inline			bool	ax_str_has_suffix	( const T*    sz, const T*    suffix )		{ return _ax_str_has_suffix(sz,suffix); } \
	inline			ax_int	ax_strnlen			( const T*    s, ax_int len )				{ return _ax_strnlen(s,len); } \
	inline			ax_int	ax_strlen			( const T*    s ) 							{ return _ax_strlen(s); }  \
	inline			T*		ax_strchr_list		(       T*    sz, const T*    chrs )		{ return _ax_strchr_list(sz, ax_const_cast(chrs) ); }	\
	inline	const	T*		ax_strchr_list		( const T*    sz, const T*    chrs )		{ return _ax_strchr_list(sz,chrs); } \
	inline			T*		ax_strrchr_list		(       T*    sz, const T*    chrs )		{ return _ax_strrchr_list(sz, ax_const_cast(chrs) ); } \
	inline	const	T*		ax_strrchr_list		( const T*    sz, const T*    chrs )		{ return _ax_strrchr_list(sz,chrs); } \
	inline			ax_int	ax_strcasecmp		( const T*    s1, const T*    s2 )			{ return _ax_strcasecmp(s1,s2); } \
	inline			ax_int	ax_strncasecmp		( const T*    s1, const T*    s2, ax_int n )	{ return _ax_strncasecmp(s1,s2,n); } \
	inline			T*		ax_strcasechr		(       T*    sz, T    ch )					{ return _ax_strcasechr(sz,ch); } \
	inline	const	T*		ax_strcasechr		( const T*    sz, T    ch )					{ return _ax_strcasechr( ax_const_cast(sz),ch); } \
	inline			T*		ax_strcasestr		(       T*    big, const T*    little )		{ return _ax_strcasestr(big, ax_const_cast(little) ); } \
	inline	const	T*		ax_strcasestr		( const T*    big, const T*    little )		{ return _ax_strcasestr(big,little); } \
	inline			T*		ax_strchr			(       T*    sz, T    ch )					{ return _ax_strchr(sz,ch); } \
	inline	const	T*		ax_strchr			( const T*    sz, T    ch )					{ return _ax_strchr<const T>(sz,ch); } \
	inline			ax_int	ax_strcmp			( const T*    s1, const T*    s2 )			{ return _ax_strcmp(s1,s2); } \
	inline			ax_int	ax_strncmp			( const T*    s1, const T*    s2, ax_int n )	{ return _ax_strncmp(s1,s2,n); } \
	inline			T*		ax_strcpy			( 		T*    dst, const T*    src )		{ return _ax_strcpy(dst,src); } \
	inline			T*		ax_strncpy			( 		T*    dst, const T*    src, ax_int n ) { return _ax_strncpy(dst,src,n); } \
	inline			T*		ax_strrchr			(       T*    sz, T    ch )					{ return _ax_strrchr(sz,ch); } \
	inline	const	T*		ax_strrchr			( const T*    sz, T    ch )					{ return _ax_strrchr<const T>(sz,ch); } \
	inline			T*		ax_strstr			(       T*    a, const T*    b )			{ return _ax_strstr(a, ax_const_cast(b) ); } \
	inline	const	T*		ax_strstr			( const T*    a, const T*    b )			{ return _ax_strstr(a,b); } \
	inline			T*		ax_strrstr			(       T*    a, const T*    b )			{ return _ax_strrstr(a, ax_const_cast(b) ); } \
	inline	const	T*		ax_strrstr			( const T*    a, const T*    b )			{ return _ax_strrstr(a,b); } \
	inline			ax_int	ax_c_str_hash		( const T*    sz )							{ return _ax_c_str_hash(sz); } \
//--------------
	ax_TYPE_LIST_all_char
#undef ax_TYPE_LIST_ITEM

template< class T > inline
const T* ax_c_str_file_basename( const T* sz ) {
	auto p = ax_strrchr_list(sz,"\\/");
	return p ? p+1 : sz;
}



#endif
