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

class PropNode : public TypedNode {
	ax_DefObject( PropNode, TypedNode );
public:
	PropNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	bool		is_let;

	LexerPos	dataTypePos;
	LexerPos	initExprPos;
};


//-----------
class StructureNode : public TypedNode {
	ax_DefObject( StructureNode, TypedNode )
public:
	StructureNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );
	
	ax_Array_< LexerPos >	 baseOrInterfacePos;
	
};

class interface_node : public StructureNode {
	ax_DefObject( interface_node, StructureNode )
public:
	interface_node( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

};

class struct_node : public StructureNode {
	ax_DefObject( struct_node, StructureNode )
public:
	struct_node( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

};

class class_node : public StructureNode {
	ax_DefObject( class_node, StructureNode )
public:
	class_node( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

};


}} //namespace

#endif /* defined(__axc__MetaNode__) */
