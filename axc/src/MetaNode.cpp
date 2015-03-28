//
//  MetaNode.cpp
//  axc
//
//  Created by Jason on 2014-12-07.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "MetaNode.h"
#include "Lexer.h"

namespace ax {
namespace Compile {

ax_ImplObject( MetaNode );

ax_ImplObject( namespace_node );
ax_ImplObject( TypedNode );
ax_ImplObject( StructNode );
ax_ImplObject( PropNode );
ax_ImplObject( FuncNode );
ax_ImplObject( FuncOverload );


MetaNode::MetaNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name ) {
	nodeType = TokenType::t_unknown;
	
	this->parent = parent;
	this->name	 = name;
	this->pos	 = pos;
	
	this->children = ax_new_obj( ChildrenDict );
	
	ax_if_let( p, parent ) {
		p->children->add( name, ax_ThisObj );
	}
}

void MetaNode::OnStringReq( ax_ToStringReq & req ) const {
	req.indent() << ax_txt("name=") << name << ax_txt("\t\t") << nodeType;
	
//	if( parent ) {
//		req << ax_txt(" paraent=") << parent->name;
//	}
	
	req.newLine();
	
	req.indentLevel++;
	
	ax_foreach( & c, *children ) {
		req << c;
	}
	req.indentLevel--;
}

ax_NullableObj< FuncNode >	MetaNode::getFunc	( const ax_string & name ) {
	ax_if_not_let( p, children->tryGetValue( name ) ) {
		return nullptr;
	}else{
		return p->ax_as< FuncNode >();
	}
}

ax_Obj< FuncNode >			MetaNode::getOrAddFunc	( const ax_string & name ) {
	ax_if_let( p, getFunc( name ) ) {
		return p;
	}
	
	auto fn = ax_new_obj( FuncNode, ax_ThisObj, LexerPos(), name );
	return fn;
}


namespace_node::namespace_node( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

ax_Obj< namespace_node >	namespace_node::getOrAddNamespace	( const ax_string & name, LexerPos & pos ) {

	ax_if_not_let( p, children->tryGetValue( name ) ) {
		auto new_node = ax_new_obj( namespace_node, ax_ThisObj, pos, name );
		new_node->nodeType = TokenType::t_namespace;
		return new_node;
	}

	if( ! p->ax_is< namespace_node >() ) {
		Log::Error( nullptr, &pos, ax_txt("identifier already exists") );
	}

	return p->ax_cast< namespace_node >();
}

TypedNode::TypedNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

StructNode::StructNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

PropNode::PropNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

FuncNode::FuncNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
	nodeType = TokenType::t_fn;
}

FuncOverload::FuncOverload( ax_Obj< FuncNode > fn, const LexerPos & pos )
: base( nullptr, pos, fn->name ) {
	fn->overloads.append( ax_ThisObj );
}



}} //namespace
