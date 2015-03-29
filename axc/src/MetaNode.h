//
//  MetaNode.h
//  axc
//
//  Created by Jason on 2014-12-07.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__MetaNode__
#define __axc__MetaNode__

#include "Log.h"
#include "DeclarationModifier.h"

namespace ax {
namespace Compile {

class FuncNode;

class LexerPos;
class MetaNode : public System::Object {
	ax_DefObject( MetaNode, System::Object )
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	MetaNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	template< typename T >
	ax_NullableObj<T>	getUpperByType	() {
//		ax_dump( name );
		ax_assert( this );
		
		ax_if_let( p, this->ax_as<T>() ) {
			return p;
		}
		
		ax_if_let( o, parent ) {
			return o->getUpperByType<T>();
		}else{
			return nullptr;
		}
	}


	ax_string			name;
	LexerPos			pos;

	ax_NullableObj< MetaNode >	parent;
	
	TokenType			nodeType;
	bool		is_let		() { return nodeType == TokenType::t_let; }
	bool		is_var		() { return nodeType == TokenType::t_var; }
	bool		is_class	() { return nodeType == TokenType::t_class; }
	bool		is_struct	() { return nodeType == TokenType::t_struct; }
	bool		is_interface() { return nodeType == TokenType::t_interface; }
	
	ax_NullableObj< FuncNode >	getFunc		( const ax_string & name );
	ax_Obj< FuncNode >			getOrAddFunc( const ax_string & name );
		
				
	typedef ax_Dict< ax_string, ax_Obj< MetaNode > >	ChildrenDict;
	
	ax_Obj<	ChildrenDict >	children;
	
	virtual void OnStringReq( ax_ToStringReq & req ) const;
};

class namespace_node : public MetaNode {
	ax_DefObject( namespace_node, MetaNode )
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	namespace_node( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	ax_Obj< namespace_node > getOrAddNamespace	( const ax_string & name, LexerPos & pos );
};

class TypedNode : public MetaNode {
	ax_DefObject( TypedNode, MetaNode );
public:
	TypedNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	DeclarationModifier	modifier;
};

class StructNode : public TypedNode {
	ax_DefObject( StructNode, TypedNode )
public:
	StructNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	ax_Array_< LexerPos >	baseOrInterfacePos;
	LexerPos				bodyPos;
};

class PropNode : public TypedNode {
	ax_DefObject( PropNode, TypedNode );
public:
	PropNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	LexerPos	dataTypePos;
	LexerPos	initExprPos;
};

class FuncOverload : public TypedNode {
	ax_DefObject( FuncOverload, TypedNode )
public:
	FuncOverload( ax_Obj< FuncNode > parent, const LexerPos & pos );

	LexerPos		returnTypePos;
	LexerPos		paramPos;
	LexerPos		bodyPos;
};

class FuncNode : public TypedNode {
	ax_DefObject( FuncNode, TypedNode );
public:
	FuncNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );
	
	ax_Array_< ax_Obj< FuncOverload > >		overloads;
};

}} //namespace

#endif /* defined(__axc__MetaNode__) */
