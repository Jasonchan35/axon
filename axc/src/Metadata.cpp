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

Metadata* g_metadata;

Metadata::Metadata() {
	g_metadata = this;

	namespace_ax	= ax_new_obj( Namespace, nullptr, ax_txt("ax"), Location() );
	root			= ax_new_obj( Namespace, namespace_ax, ax_txt("ax_build"), Location() );
	
	type_dict		= ax_new_obj(  Class, root, ax_txt("Dict"), Location() );
	type_dict->buildin	 = true;
	type_dict->setCppName( ax_txt("ax_Dict"), true );

	type_any		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("any"	), Location() );	type_any->setCppName	( ax_txt("any"		), true );
	
	type_void		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("void"	), Location() );	type_void->setCppName	( ax_txt("void"		), true );
	type_bool		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("bool"	), Location() );	type_bool->setCppName	( ax_txt("bool"		), true );
	type_string		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("string" ), Location() );	type_string->setCppName	( ax_txt("ax_string"), true );

	type_int8		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("int8"	), Location() );	type_int8->setCppName  	( ax_txt("ax_int8"	), true );
	type_int16		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("int16"	), Location() );	type_int16->setCppName 	( ax_txt("ax_int16"	), true );
	type_int32		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("int32"	), Location() );	type_int32->setCppName 	( ax_txt("ax_int32"	), true );
	type_int64		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("int64"	), Location() );	type_int64->setCppName 	( ax_txt("ax_int64"	), true );

	type_uint8		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("uint8"	), Location() );	type_uint8->setCppName 	( ax_txt("ax_uint8"	), true );
	type_uint16		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("uint16" ), Location() );	type_uint16->setCppName	( ax_txt("ax_uint16"), true );
	type_uint32		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("uint32" ), Location() );	type_uint32->setCppName	( ax_txt("ax_uint32"), true );
	type_uint64		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("uint64" ), Location() );	type_uint64->setCppName	( ax_txt("ax_uint64"), true );
	
	type_int		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("int"	), Location() );	type_int->setCppName 	( ax_txt("ax_int"	), true );
	type_uint		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("uint"	), Location() );	type_uint->setCppName	( ax_txt("ax_uint"	), true );

	type_float		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("float"	), Location() );	type_float->setCppName	( ax_txt("float"	), true );
	type_double		= ax_new_obj(  PrimitiveTypeSpec, root, ax_txt("double" ), Location() );	type_double->setCppName	( ax_txt("double"	), true );
	
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
		addPrefixOperatorFunc( t, t, TokenType::t_add );
		addPrefixOperatorFunc( t, t, TokenType::t_sub );
		addPrefixOperatorFunc( t, t, TokenType::t_add2 );
		addPrefixOperatorFunc( t, t, TokenType::t_sub2 );

		addPostfixOperatorFunc( t, t, TokenType::t_add2 );
		addPostfixOperatorFunc( t, t, TokenType::t_sub2 );

		addOperatorFunc( t, t, TokenType::t_assign );
	
		addOperatorFunc( t, t, TokenType::t_add );
		addOperatorFunc( t, t, TokenType::t_sub );
		addOperatorFunc( t, t, TokenType::t_mul );
		addOperatorFunc( t, t, TokenType::t_div );
		addOperatorFunc( t, t, TokenType::t_mod );
		
		addOperatorFunc( type_bool, t, TokenType::t_equal );
		addOperatorFunc( type_bool, t, TokenType::t_notEqual );
		addOperatorFunc( type_bool, t, TokenType::t_less );
		addOperatorFunc( type_bool, t, TokenType::t_lessEqual );
		addOperatorFunc( type_bool, t, TokenType::t_greater );
		addOperatorFunc( type_bool, t, TokenType::t_greaterEqual );
	}
	
	addPrefixOperatorFunc( type_bool, type_bool, TokenType::t_not );
	
	addOperatorFunc( type_bool,	type_bool, 	TokenType::t_or  );
	addOperatorFunc( type_bool,	type_bool, 	TokenType::t_and );
	
	addOperatorFunc( type_bool, type_bool, 	TokenType::t_equal );
	addOperatorFunc( type_bool, type_bool, 	TokenType::t_notEqual );

	addOperatorFunc( type_string,type_string, TokenType::t_add );
	addOperatorFunc( type_bool,	type_string, TokenType::t_equal );
	addOperatorFunc( type_bool,	type_string, TokenType::t_notEqual );
	addOperatorFunc( type_bool,	type_string, TokenType::t_less );
	addOperatorFunc( type_bool, type_string, TokenType::t_lessEqual );
	addOperatorFunc( type_bool, type_string, TokenType::t_greater );
	addOperatorFunc( type_bool, type_string, TokenType::t_greaterEqual );
	
	type_object		= ax_new_obj(  Class, root, ax_txt("Object" ), Location() );
	type_object->buildin = true;
	type_object->setCppName( ax_txt("ax_Object"), true );
	
	{
		type_array		= ax_new_obj(  Class, root, ax_txt("Array"), Location() );
		type_array->buildin	 = true;
		type_array->setCppName( ax_txt("ax_Array"), true );
		
		auto p1 = type_array->addTemplateParam( ax_txt("T"), Location() );
		
		{
			auto fn = type_array->addFunc( ax_txt("New") );
			auto fo = fn->addOverload( Location() );
			fo->buildin = true;
			fo->addParam( ax_txt("size"), Location(), Type::MakeValue( type_int, false ), Location() );
			fo->returnType = Type::MakeTypename( type_array );
		}
		
		{
			auto fn = type_array->addFunc( ax_txt("add") );
			auto fo = fn->addOverload( Location() );
			fo->buildin = true;
			fo->addParam( ax_txt("value"), Location(), Type::MakeValue( p1, false ), Location() );
			fo->returnType = Type::MakeTypename( type_void );
		}
	}
}

void Metadata::addOperatorFunc( ax_Obj< TypeSpec > returnType, ax_Obj< TypeSpec > type, TokenType op ) {
	auto fn = type->getOrAddOperatorFunc( op );
	auto fo = fn->addOverload( Location() );
	
	fo->buildin = true;
	fo->returnType = Type::MakeValue( returnType, false );
		
	fo->addParam( ax_txt("rhs"), Location(), Type::MakeValue( type, false ), Location() );
}

void Metadata::addPrefixOperatorFunc( ax_Obj< TypeSpec > returnType, ax_Obj< TypeSpec > type, TokenType op ) {
	auto fn = type->getOrAddPrefixOperatorFunc( op );
	auto fo = fn->addOverload( Location() );
	
	fo->buildin = true;
	fo->returnType = Type::MakeValue( returnType, false );
}

void Metadata::addPostfixOperatorFunc( ax_Obj< TypeSpec > returnType, ax_Obj< TypeSpec > type, TokenType op ) {
	auto fn = type->getOrAddOperatorFunc( op );
	auto fo = fn->addOverload( Location() );
	
	fo->buildin = true;
	fo->returnType = Type::MakeValue( returnType, false );
}

	
void Metadata::_OnStringReq_Node( ax_ToStringReq & req, ax_Obj< MetaNode > node ) const {
	req << node;
//	req << node->getTypeInfo().name() << ax_txt(" ");
//	req << node->name() << ax_txt(" ");
//	req << ax_txt("  [") << (const void*) node.ptr() << ax_txt("] ");
	
	req.newLine();
	req.indentLevel++;	
	ax_foreach( & c, node->children ) {
		req.indent();
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


