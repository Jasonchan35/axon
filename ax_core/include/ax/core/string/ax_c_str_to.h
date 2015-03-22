//
//  ax_c_str_to.h
//  ax_core
//
//  Created by Jason on 2014-11-28.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_ax_str_to_h
#define ax_core_ax_str_to_h

#include "../platform/platform.h"

namespace ax {
namespace System {

#define ax_TYPE_LIST_ITEM( NAME, T ) \
	bool ax_c_str_to( const T* sz, bool		&v ); \
	bool ax_c_str_to( const T* sz, uint8_t    &v ); \
	bool ax_c_str_to( const T* sz, uint16_t   &v ); \
	bool ax_c_str_to( const T* sz, uint32_t   &v ); \
	bool ax_c_str_to( const T* sz, uint64_t   &v ); \
	bool ax_c_str_to( const T* sz, int8_t    	&v ); \
	bool ax_c_str_to( const T* sz, int16_t    &v ); \
	bool ax_c_str_to( const T* sz, int32_t   	&v ); \
	bool ax_c_str_to( const T* sz, int64_t   	&v ); \
	bool ax_c_str_to( const T* sz, float  	&v ); \
	bool ax_c_str_to( const T* sz, double 	&v ); \
//==============================
	ax_TYPE_LIST_all_char
#undef ax_TYPE_LIST_ITEM

} //namespace System
} //namespace ax


#endif
