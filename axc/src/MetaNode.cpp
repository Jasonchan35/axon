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

ax_ImplObject( NamespaceNode );
ax_ImplObject( TypedNode );
ax_ImplObject( PrimitiveType );
ax_ImplObject( StructNode );
ax_ImplObject( PropNode );
ax_ImplObject( FuncNode );
ax_ImplObject( FuncOverload );
ax_ImplObject( FuncType );



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


ax_string MetaNode::getFullname( const ax_string & seperator ) const {
	ax_TempString s;
	appendFullname( s, seperator );
	return s.to_string();
}

void MetaNode::appendFullname( ax_MutString & fullname, const ax_string & seperator ) const {
	ax_if_let( p, parent ) {
		p->appendFullname( fullname, seperator );
		fullname.append( seperator );
	}
	fullname.append( name );
}


ax_NullableObj< FuncNode >	MetaNode::getOperatorFunc ( TokenType op ) {
	return getFunc( ax_txt( "operator " ) + ax_to_string(op) );
}

ax_Obj< FuncNode >	MetaNode::getOrAddOperatorFunc	( TokenType op ) {
	return getOrAddFunc( ax_txt( "operator " ) + ax_to_string(op) );
}

ax_NullableObj< FuncNode >	MetaNode::getPrefixOperatorFunc		( TokenType op ) {
	return getFunc( ax_txt( "prefix operator " ) + ax_to_string(op) );
}

ax_Obj< FuncNode >	MetaNode::getOrAddPrefixOperatorFunc	( TokenType op ) {
	return getOrAddFunc( ax_txt( "prefix operator " ) + ax_to_string(op) );
}




ax_NullableObj<MetaNode>	MetaNode::getNode( const ax_string & name ) {
	ax_if_let( m, getMember( name ) ) return m;
	ax_if_let( t, onGetNode( name ) ) return t;
	
	ax_if_let( p, parent ) {
		return p->getNode( name );
	}
	return nullptr;
}

ax_NullableObj<MetaNode>	MetaNode::getMember ( const ax_string & name ) {
	ax_if_let( t, children->tryGetValue( name ) ) {
		return t;
	}
	return onGetMember( name );
}

ax_NullableObj<MetaNode>	MetaNode::onGetMember	( const ax_string & name ) {
	return nullptr;
}


void MetaNode::OnStringReq( ax_ToStringReq & req ) const {
	req.indent() << ax_txt("name=") << name << ax_txt("\t\t") << this->getTypeInfo().name() <<  ax_txt("\t") << nodeType;
	
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


NamespaceNode::NamespaceNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

ax_Obj< NamespaceNode >	NamespaceNode::getOrAddNamespace	( const ax_string & name, LexerPos & pos ) {

	ax_if_not_let( p, children->tryGetValue( name ) ) {
		auto new_node = ax_new_obj( NamespaceNode, ax_ThisObj, pos, name );
		new_node->nodeType = TokenType::t_namespace;
		return new_node;
	}

	if( ! p->ax_is< NamespaceNode >() ) {
		Log::Error( nullptr, &pos, ax_txt("identifier already exists") );
	}

	return p->ax_cast< NamespaceNode >();
}

TypedNode::TypedNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
	buildin = false;
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

ax_NullableObj< FuncOverload > FuncNode::getOverload( ax_Array< ax_Obj< FuncOverload > > & candidate, const ax_Array<FuncParam> & params ) {
	candidate.resize(0);
	
	ax_foreach( & fo, overloads ) {
		if( fo->isMatch( params ) ) {
			candidate.add( fo );
		}
	}
	
	if( candidate.size() == 1 ) return candidate[0];
	return nullptr;
}

bool	FuncOverload::isMatch ( const ax_Array< FuncParam > & callParams ) {
	ax_int i = 0;
	ax_foreach( & c, callParams ) {
		if( i >= params.size() ) return false;
		auto & p = params[i];
		
		if( c.rtype.type != p.rtype.type ) return false;

		i++;
	}
	
//	auto fullname = func->getFullname( ax_txt(".") );
	
	if( i != params.size() ) {
		return false;
	}
	
	return true;
}

FuncOverload::FuncOverload( ax_Obj< FuncNode > fn, const LexerPos & pos )
: base( nullptr, pos, fn->name ) {
	fn->overloads.add( ax_ThisObj );
	func = fn;
}



}} //namespace
