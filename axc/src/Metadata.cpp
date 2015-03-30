//
//  Metadata.cpp
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "Metadata.h"

namespace ax {
namespace Compile {

Metadata::Metadata() {
	root = ax_new_obj( NamespaceNode, ax_NullableObj< MetaNode >(nullptr), LexerPos(), ax_txt("ax_build") );
	root->nodeType = TokenType::t_namespace;

	type_int8		= ax_new_obj(  PrimitiveType, root, ax_txt("int8" ) );
	type_int16		= ax_new_obj(  PrimitiveType, root, ax_txt("int16" ) );
	type_int32		= ax_new_obj(  PrimitiveType, root, ax_txt("int32" ) );
	type_int64		= ax_new_obj(  PrimitiveType, root, ax_txt("int64" ) );

	type_uint8		= ax_new_obj(  PrimitiveType, root, ax_txt("uint8" ) );
	type_uint16		= ax_new_obj(  PrimitiveType, root, ax_txt("uint16" ) );
	type_uint32		= ax_new_obj(  PrimitiveType, root, ax_txt("uint32" ) );
	type_uint64		= ax_new_obj(  PrimitiveType, root, ax_txt("uint64" ) );
	
	type_int		= ax_new_obj(  PrimitiveType, root, ax_txt("int" ) );
	type_uint		= ax_new_obj(  PrimitiveType, root, ax_txt("uint" ) );

	type_float		= ax_new_obj(  PrimitiveType, root, ax_txt("float" ) );
	type_double		= ax_new_obj(  PrimitiveType, root, ax_txt("double" ) );

	type_string		= ax_new_obj(  PrimitiveType, root, ax_txt("string" ) );
	
	type_all_int.add( type_int );
	type_all_int.add( type_int8 );
	type_all_int.add( type_int16 );
	type_all_int.add( type_int32 );
	type_all_int.add( type_int64 );

	type_all_uint.add( type_uint );
	type_all_uint.add( type_uint8 );
	type_all_uint.add( type_uint16 );
	type_all_uint.add( type_uint32 );
	type_all_uint.add( type_uint64 );

	type_all_float.add( type_float );
	type_all_float.add( type_double );

	ax_foreach( & t, type_all_int   ) { type_all_number.add( t ); }
	ax_foreach( & t, type_all_uint  ) { type_all_number.add( t ); }
	ax_foreach( & t, type_all_float ) { type_all_number.add( t ); }
	
}

void Metadata::OnStringReq( ax_ToStringReq & req ) const {
	req << ax_txt("======= Metadata =======\n");
	req << root;	
	req << ax_txt("======= End Metadata =======\n");
}

}} //namespace