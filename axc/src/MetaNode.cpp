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

void MetaNode::init( MetaNode* parent, LexerPos & pos, const ax_string & name ) {
	this->parent = parent;
	this->name	 = name;
	if( parent ) {
		parent->children->add( name, ax_as_obj( this ) );
	}
}

void MetaNode::OnStringReq( ax_ToStringReq & req ) const {
	auto type = ax_typeof( *this );

	req.indent() << ax_txt("name=") << name << ax_txt(" ") << type;
	
//	if( parent ) {
//		req << ax_txt(" paraent=") << parent->name;
//	}
	
	req.newLine();
	
	req.indentLevel++;
	
	ax_foreach( auto & c, *children ) {
		req << c;
	}
	req.indentLevel--;
}

Node_namespace::Node_namespace(  MetaNode* parent, LexerPos & pos, const ax_string & name ) {
	base::init( parent, pos, name );
}

ax_Obj< Node_namespace >	Node_namespace::getOrAddNamespace	( const ax_string & name, LexerPos & pos ) {

	ax_if_not_let( p, children->tryGetValue( name ) ) {
		return ax_new_obj( Node_namespace, this, pos, name );
	}

	if( ! p->ax_is< Node_namespace >() ) {
		Log::Error( nullptr, &pos, "identifier already exists");
	}

	return p->ax_cast< Node_namespace >();
}


}} //namespace
