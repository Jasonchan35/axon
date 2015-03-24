//
//  Type.cpp
//  ax_core
//
//  Created by Jason Chan on Mar 18, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#include <ax/core/base/Type.h>

#define ax_TYPE_LIST_ITEM( NAME, T ) \
	ax_TypeInfo		ax_TypeInfo_##NAME( ax_txt(#NAME), ax_TypeCode::k_##NAME, nullptr ); \
//----
	ax_TYPE_LIST_all_int
	ax_TYPE_LIST_all_uint
	ax_TYPE_LIST_all_char
	ax_TYPE_LIST_all_bool
#undef ax_TYPE_LIST_ITEM


namespace ax {
namespace System {

TypeInfo::TypeInfo( const String & name, TypeCode typeCode, const TypeInfo* baseType )
: _name( name )
, _typeCode( typeCode )
, _baseType( baseType )
{
}

}} //namespace

