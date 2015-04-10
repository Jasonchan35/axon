//
//  ToStringReq.cpp
//  ax_core
//
//  Created by Jason Chan on Mar 18, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#include <ax/core/string/ToStringReq.h>

#ifdef ax_COMPILER_VC
	#define	ax_snprintf	_snprintf
#else
	#define	ax_snprintf	snprintf
#endif

template< typename T > inline
void _ax_to_string_req_bool( ax::System::ToStringReq_<T> & req, const bool & v ) {
	auto s = v ? ax_txt("true") : ax_txt("false");
	req.outbuf().append_static_cast( s );
}


template< typename T > inline
void _ax_to_string_req_int32( ax::System::ToStringReq_<T> & req, const int32_t & v ) {
	char tmp[128];
	int ret = ax_snprintf( tmp, 128, "%d", v );
	if( ret < 0 || ret >= 128 ) throw ax::System::Err_Undefined();
	req.outbuf().append_static_cast( tmp, ret );
}

template< typename T > inline
void _ax_to_string_req_int64( ax::System::ToStringReq_<T> & req, const int64_t & v ) {
	char tmp[128];
	int ret = ax_snprintf( tmp, 128, "%lld", v );
	if( ret < 0 || ret >= 128 ) throw ax::System::Err_Undefined();
	req.outbuf().append_static_cast( tmp, ret );
}

template< typename T > inline
void _ax_to_string_req_uint32( ax::System::ToStringReq_<T> & req, const uint32_t & v ) {
	char tmp[128];
	int ret = ax_snprintf( tmp, 128, "%u", v );
	if( ret < 0 || ret >= 128 ) throw ax::System::Err_Undefined();
	req.outbuf().append_static_cast( tmp, ret );
}

template< typename T > inline
void _ax_to_string_req_uint64( ax::System::ToStringReq_<T> & req, const uint64_t & v ) {
	char tmp[128];
	int ret = ax_snprintf( tmp, 128, "%llu", v );
	if( ret < 0 || ret >= 128 ) throw ax::System::Err_Undefined();
	req.outbuf().append_static_cast( tmp, ret );
}

template< typename T > inline
void _ax_to_string_req_float( ax::System::ToStringReq_<T> & req, const float & v ) {
	char tmp[128];
	int ret = ax_snprintf( tmp, 128, "%f", v );
	if( ret < 0 || ret >= 128 ) throw ax::System::Err_Undefined();
	req.outbuf().append_static_cast( tmp, ret );
}

template< typename T > inline
void _ax_to_string_req_double( ax::System::ToStringReq_<T> & req, const double & v ) {
	char tmp[128];
	int ret = ax_snprintf( tmp, 128, "%f", v );
	if( ret < 0 || ret >= 128 ) throw ax::System::Err_Undefined();
	req.outbuf().append_static_cast( tmp, ret );
}

template< typename T > inline
void _ax_to_string_req_pointer( ax::System::ToStringReq_<T> & req, const void* & v ) {
	char tmp[128];
	int ret = ax_snprintf( tmp, 128, "%p", v );
	if( ret < 0 || ret >= 128 ) throw ax::System::Err_Undefined();
	req.outbuf().append_static_cast( tmp, ret );
}


#define ax_TYPE_LIST_ITEM( NAME, T ) \
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const void*      v ) { _ax_to_string_req_pointer ( req, v ); } \
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const void*    & v ) { _ax_to_string_req_pointer ( req, v ); } \
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const bool   	& v ) { _ax_to_string_req_bool	  ( req, v ); } \
	\
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const int8_t   & v ) { _ax_to_string_req_int32 ( req, v ); } \
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const int16_t  & v ) { _ax_to_string_req_int32 ( req, v ); } \
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const int32_t  & v ) { _ax_to_string_req_int32 ( req, v ); } \
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const int64_t  & v ) { _ax_to_string_req_int64 ( req, v ); } \
	\
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const uint8_t  & v ) { _ax_to_string_req_uint32( req, v ); } \
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const uint16_t & v ) { _ax_to_string_req_uint32( req, v ); } \
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const uint32_t & v ) { _ax_to_string_req_uint32( req, v ); } \
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const uint64_t & v ) { _ax_to_string_req_uint64( req, v ); } \
	\
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const float    & v ) { _ax_to_string_req_float ( req, v ); } \
	template<> void ax_to_string_req( ax::System::ToStringReq_<T> & req, const double   & v ) { _ax_to_string_req_double( req, v ); } \
	\
//--------
	ax_TYPE_LIST_all_char
#undef ax_TYPE_LIST_ITEM

