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

	type_object		= ax_new_obj(  ClassNode, root, LexerPos(), ax_txt("Object" ) );
	type_object->buildin = true;

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
	type_bool		= ax_new_obj(  PrimitiveType, root, ax_txt("bool" ) );
	
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
	
	
	ax_foreach( & t, type_all_number ) {
		addPrefixOperatorFunc( RType(t), t, TokenType::t_add );
		addPrefixOperatorFunc( RType(t), t, TokenType::t_sub );
		addPrefixOperatorFunc( RType(t), t, TokenType::t_add2 );
		addPrefixOperatorFunc( RType(t), t, TokenType::t_sub2 );

		addPostfixOperatorFunc( RType(t), t, TokenType::t_add2 );
		addPostfixOperatorFunc( RType(t), t, TokenType::t_sub2 );

		addOperatorFunc( RType(t), t, TokenType::t_assign );
	
		addOperatorFunc( RType(t), t, TokenType::t_add );
		addOperatorFunc( RType(t), t, TokenType::t_sub );
		addOperatorFunc( RType(t), t, TokenType::t_mul );
		addOperatorFunc( RType(t), t, TokenType::t_div );
		addOperatorFunc( RType(t), t, TokenType::t_mod );
		
		addOperatorFunc( RType(type_bool), t, TokenType::t_equal );
		addOperatorFunc( RType(type_bool), t, TokenType::t_notEqual );
		addOperatorFunc( RType(type_bool), t, TokenType::t_less );
		addOperatorFunc( RType(type_bool), t, TokenType::t_lessEqual );
		addOperatorFunc( RType(type_bool), t, TokenType::t_greater );
		addOperatorFunc( RType(type_bool), t, TokenType::t_greaterEqual );
	}
	
	addPrefixOperatorFunc( RType(type_bool), type_bool, TokenType::t_not );
	
	addOperatorFunc( RType(type_bool), type_bool, TokenType::t_or  );
	addOperatorFunc( RType(type_bool), type_bool, TokenType::t_and );
	
	addOperatorFunc( RType(type_bool), type_bool, TokenType::t_equal );
	addOperatorFunc( RType(type_bool), type_bool, TokenType::t_notEqual );

	addOperatorFunc( RType(type_string),type_string, TokenType::t_add );
	addOperatorFunc( RType(type_bool),	type_string, TokenType::t_equal );
	addOperatorFunc( RType(type_bool),	type_string, TokenType::t_notEqual );
	addOperatorFunc( RType(type_bool),	type_string, TokenType::t_less );
	addOperatorFunc( RType(type_bool), 	type_string, TokenType::t_lessEqual );
	addOperatorFunc( RType(type_bool), 	type_string, TokenType::t_greater );
	addOperatorFunc( RType(type_bool), 	type_string, TokenType::t_greaterEqual );
	
}


void Metadata::addOperatorFunc( RType returnType, ax_Obj< TypedNode > type, TokenType op ) {
	auto fn = type->getOrAddOperatorFunc( op );
	
	auto ov = ax_new_obj( FuncOverload, fn, LexerPos() );
	
	ov->buildin = true;
	ov->returnType = returnType;
	auto & pm = ov->params.addNew();
	pm.rtype = RType( type );
	pm.name = ax_txt("rhs");
	
	fn->addOverload( ov );
}

void Metadata::addPrefixOperatorFunc( RType returnType, ax_Obj< TypedNode > type, TokenType op ) {
	auto fn = type->getOrAddPrefixOperatorFunc( op );
	
	auto ov = ax_new_obj( FuncOverload, fn, LexerPos() );
	
	ov->buildin = true;
	ov->returnType = returnType;
	fn->addOverload( ov );
}

void Metadata::addPostfixOperatorFunc( RType returnType, ax_Obj< TypedNode > type, TokenType op ) {
	auto fn = type->getOrAddOperatorFunc( op );
	
	auto ov = ax_new_obj( FuncOverload, fn, LexerPos() );
	
	ov->buildin = true;
	ov->returnType = returnType;
	fn->addOverload( ov );
}

	
void Metadata::_OnStringReq_Node( ax_ToStringReq & req, ax_Obj< MetaNode > node ) const {
	req << node;
	
	req.newLine();
	req.indentLevel++;	
	ax_foreach( & c, *node->children ) {
		_OnStringReq_Node( req, c );
	}
	req.indentLevel--;
}

void Metadata::OnStringReq( ax_ToStringReq & req ) const {
	req << ax_txt("======= Metadata =======\n");
	_OnStringReq_Node( req, root );
	req << ax_txt("======= End Metadata =======\n");
}

}} //namespace