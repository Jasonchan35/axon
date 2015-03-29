//
//  ax_c_str_to.cpp
//  ax_core
//
//  Created by Jason on 2014-11-28.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include <ax/core/string/ax_c_str_to.h>

//==================  ax_c_str_to char ============================
bool ax_c_str_to( const char* sz, bool &v ) {
	if( ax_strcasecmp( sz, "true" ) == 0 ) {
		v = true;
		return true;
	}else if( ax_strcasecmp( sz, "false" ) == 0 ) {
		v = false;
		return true;
	}
	return false;
}

bool ax_c_str_to( const char* sz, uint8_t    &v ) {
	uint32_t tmp;
	if( 1 != sscanf( sz, "%u", &tmp ) )
		return false;
	return ax_safe_assign( v, tmp );
}

bool ax_c_str_to( const char* sz, uint16_t    &v ) {
	uint32_t tmp;
	if( 1 != sscanf( sz, "%u", &tmp ) )
		return false;
	return ax_safe_assign( v, tmp );
}

bool ax_c_str_to( const char* sz, uint32_t   &v ) {
	if( 1 != sscanf( sz, "%u", &v ) )
		return false;
	return true;
}

bool ax_c_str_to( const char* sz, uint64_t   &v ) {
	if( 1 != sscanf( sz, "%llu", (long long unsigned*)&v ) )
		return false;
	return true;
}

bool ax_c_str_to( const char* sz, int8_t    &v ) {
	int32_t tmp;
	if( 1 != sscanf( sz, "%d", &tmp ) )
		return false;
	return ax_safe_assign( v, tmp );
}

bool ax_c_str_to( const char* sz, int16_t    &v ) {
	int32_t tmp;
	if( 1 != sscanf( sz, "%d", &tmp ) )
		return false;
	return ax_safe_assign( v, tmp );
}

bool ax_c_str_to( const char* sz, int32_t   &v ) {
	if( 1 != sscanf( sz, "%d", &v ) )
		return false;
	return true;
}

bool ax_c_str_to( const char* sz, int64_t   &v ) {
	if( 1 != sscanf( sz, "%lld", (long long int *)&v ) )
		return false;
	return true;
}

bool ax_c_str_to( const char* sz, float  &v ) {
	if( 1 != sscanf( sz, "%f", &v ) )
		return false;
	return true;
}

bool ax_c_str_to( const char* sz, double &v ) {
	if( 1 != sscanf( sz, "%lf", &v ) )
		return false;
	return true;
}

//==================  ax_c_str_to wchar ============================
bool ax_c_str_to( const wchar_t* sz, bool &v ) {
	if( ax_strcasecmp( sz, L"true" ) == 0 ) {
		v = true;
		return true;
	}else if( ax_strcasecmp( sz, L"false" ) == 0 ) {
		v = false;
		return true;
	}
	return false;
}

bool ax_c_str_to( const wchar_t* sz, uint8_t    &v ) {
	uint32_t tmp;
	if( 1 != swscanf( sz, L"%u", &tmp ) )
		return false;
	return ax_safe_assign( v, tmp );
}

bool ax_c_str_to( const wchar_t* sz, uint16_t    &v ) {
	uint32_t tmp;
	if( 1 != swscanf( sz, L"%u", &tmp ) )
		return false;
	return ax_safe_assign( v, tmp );
}

bool ax_c_str_to( const wchar_t* sz, uint32_t   &v ) {
	if( 1 != swscanf( sz, L"%u", &v ) )
		return false;
	return true;
}

bool ax_c_str_to( const wchar_t* sz, uint64_t   &v ) {
	if( 1 != swscanf( sz, L"%llu", &v ) )
		return false;
	return true;
}

bool ax_c_str_to( const wchar_t* sz, int8_t    &v ) {
	int32_t tmp;
	if( 1 != swscanf( sz, L"%d", &tmp ) )
		return false;
	return ax_safe_assign( v, tmp );
}

bool ax_c_str_to( const wchar_t* sz, int16_t    &v ) {
	int32_t tmp;
	if( 1 != swscanf( sz, L"%d", &tmp ) )
		return false;
	return ax_safe_assign( v, tmp );
}

bool ax_c_str_to( const wchar_t* sz, int32_t   &v ) {
	if( 1 != swscanf( sz, L"%d", &v ) )
		return false;
	return true;
}

bool ax_c_str_to( const wchar_t* sz, int64_t   &v ) {
	if( 1 != swscanf( sz, L"%lld", &v ) )
		return false;
	return true;
}

bool ax_c_str_to( const wchar_t* sz, float  &v ) {
	if( 1 != swscanf( sz, L"%f", &v ) )
		return false;
	return true;
}

bool ax_c_str_to( const wchar_t* sz, double &v ) {
	if( 1 != swscanf( sz, L"%lf", &v ) )
		return false;
	return true;
}

//==================  ax_c_str_to char16_t ============================
#define ax_TYPE_LIST_ITEM( NAME, T ) \
	bool ax_c_str_to( const char16_t* sz, T &v ) { \
		wchar_t	tmp[100+1]; \
		auto dst = tmp; \
		for( int i=0; i<100; i++ ) { \
			*dst = *sz; \
			if( *dst ) break; \
		} \
		tmp[100] = 0; \
		return ax_c_str_to( tmp, v ); \
	} \
	\
	bool ax_c_str_to( const char32_t* sz, T &v ) { \
		wchar_t	tmp[100+1]; \
		auto dst = tmp; \
		for( int i=0; i<100; i++ ) { \
			*dst = *sz; \
			if( *dst ) break; \
		} \
		tmp[100] = 0; \
		return ax_c_str_to( tmp, v ); \
	} \
//-----
	ax_TYPE_LIST_all_int
	ax_TYPE_LIST_all_uint
	ax_TYPE_LIST_all_float
	ax_TYPE_LIST_all_bool
#undef ax_TYPE_LIST_ITEM

