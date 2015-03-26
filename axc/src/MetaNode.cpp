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
ax_ImplObject( PropNode );

ax_ImplObject( TypedNode );
ax_ImplObject( StructureNode );
ax_ImplObject( class_node );
ax_ImplObject( struct_node );
ax_ImplObject( interface_node );


MetaNode::MetaNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name ) {
	this->parent = parent;
	this->name	 = name;
	this->pos	 = pos;
	
	this->children = ax_new_obj( ChildrenDict );
	
	ax_if_let( p, parent ) {
		p->children->add( name, ax_ThisObj );
	}
}

void MetaNode::OnStringReq( ax_ToStringReq & req ) const {
	req.indent() << ax_txt("name=") << name << ax_txt("\t\t") << this->getTypeInfo().name();
	
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

namespace_node::namespace_node( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

ax_Obj< namespace_node >	namespace_node::getOrAddNamespace	( const ax_string & name, LexerPos & pos ) {

	ax_if_not_let( p, children->tryGetValue( name ) ) {
		return ax_new_obj( namespace_node, ax_ThisObj, pos, name );
	}

	if( ! p->ax_is< namespace_node >() ) {
		Log::Error( nullptr, &pos, ax_txt("identifier already exists") );
	}

	return p->ax_cast< namespace_node >();
}

TypedNode::TypedNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

PropNode::PropNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
	is_let = true;
}

StructureNode::StructureNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

interface_node::interface_node( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

struct_node::struct_node( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

class_node::class_node( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}



}} //namespace
