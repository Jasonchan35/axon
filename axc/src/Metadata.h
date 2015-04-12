//
//  Metadata.h
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__Metadata__
#define __axc__Metadata__

#include "MetaNode.h"

namespace ax {
namespace Compile {

class Metadata {
public:
	Metadata();

	ax_Obj< Namespace > namespace_ax;
	ax_Obj< Namespace > root;
	
	
	ax_Obj< Func >		func_new_array;
	
	ax_Array_< ax_Obj<CompositeType> >		structList;
	ax_Array_< ax_Obj<Prop> >			propList;
	ax_Array_< ax_Obj<FuncOverload> >	funcOverloadList;

	ax_Obj< TypeNode >				type_any;

	ax_Obj< PrimitiveType >			type_void;
	ax_Obj< PrimitiveType >			type_bool;
	ax_Obj< PrimitiveType >			type_string;
	
	ax_Obj< PrimitiveType >			type_int;
	ax_Obj< PrimitiveType >			type_int8;
	ax_Obj< PrimitiveType >			type_int16;
	ax_Obj< PrimitiveType >			type_int32;
	ax_Obj< PrimitiveType >			type_int64;
	
	ax_Obj< PrimitiveType >			type_uint;
	ax_Obj< PrimitiveType >			type_uint8;
	ax_Obj< PrimitiveType >			type_uint16;
	ax_Obj< PrimitiveType >			type_uint32;
	ax_Obj< PrimitiveType >			type_uint64;

	ax_Obj< PrimitiveType >			type_float;
	ax_Obj< PrimitiveType >			type_double;

	ax_Obj< Class >					type_object;
	ax_Obj< Class >					type_array;
	ax_Obj< Class >					type_dict;
	
	ax_NullableObj< Class >			type_app;
	
	ax_Array_< ax_Obj< PrimitiveType > >	type_all_number;
	ax_Array_< ax_Obj< PrimitiveType > >	type_all_int;
	ax_Array_< ax_Obj< PrimitiveType > >	type_all_uint;
	ax_Array_< ax_Obj< PrimitiveType > >	type_all_float;
	
	
	void	addOperatorFunc			( ax_Obj< TypeNode > returnType, ax_Obj< TypeNode > type, TokenType op );
	
	void	addPrefixOperatorFunc	( ax_Obj< TypeNode > returnType, ax_Obj< TypeNode > type, TokenType op );
	void	addPostfixOperatorFunc	( ax_Obj< TypeNode > returnType, ax_Obj< TypeNode > type, TokenType op );
	
	
	void 	OnStringReq( ax_ToStringReq & req ) const;
	
private:
	void _OnStringReq_Node( ax_ToStringReq & req, ax_Obj< MetaNode > node ) const;
};

extern Metadata*	g_metadata;

}} //namespace

#endif /* defined(__axc__Metadata__) */
