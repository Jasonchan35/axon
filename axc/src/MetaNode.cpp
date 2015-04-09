//
//  MetaNode.cpp
//  axc
//
//  Created by Jason on 2014-12-07.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "MetaNode.h"
#include "Lexer.h"
#include "Compiler.h"

namespace ax {
namespace Compile {

ax_ImplObject( MetaNode );

ax_ImplObject( Namespace );
ax_ImplObject( TypeNode );
ax_ImplObject( TupleType );
ax_ImplObject( PrimitiveType );
ax_ImplObject( Typename );

ax_ImplObject( StructType );
ax_ImplObject( Interface );
ax_ImplObject( Struct );
ax_ImplObject( Class );

ax_ImplObject( Prop );
ax_ImplObject( Func );
ax_ImplObject( FuncOverload );
ax_ImplObject( FuncType );

ax_string	k_ctor_name = ax_txt("ctor");

MetaNode::MetaNode( ax_NullableObj< MetaNode > parent_, const LexerPos & pos_, const ax_string & name_ )
: parent( parent_ )
, _name( name_ )
, macro_cppName( false )
, pos( pos_ )
, buildin( false )
, isTemplateInstance( false ) {
	
	ax_if_let( p, parent ) {
		p->children.add( name_, ax_ThisObj );
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
	fullname.append( name() );
}


ax_NullableObj< Func >	MetaNode::getOperatorFunc ( TokenType op ) {
	return getFunc( ax_txt( "operator " ) + ax_to_string(op) );
}

ax_Obj< Func >	MetaNode::getOrAddOperatorFunc	( TokenType op ) {
	return getOrAddFunc( ax_txt( "operator " ) + ax_to_string(op) );
}

ax_NullableObj< Func >	MetaNode::getPrefixOperatorFunc		( TokenType op ) {
	return getFunc( ax_txt( "prefix operator " ) + ax_to_string(op) );
}

ax_Obj< Func >	MetaNode::getOrAddPrefixOperatorFunc	( TokenType op ) {
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
	ax_if_let( t, children.tryGetValue( name ) ) {
		return t;
	}
	return onGetMember( name );
}

ax_NullableObj<MetaNode>	MetaNode::onGetMember	( const ax_string & name ) {
	return nullptr;
}

void MetaNode::OnStringReq( ax_ToStringReq & req ) const {
	req << this->getTypeInfo().name();
	req << ax_txt(" \"") << name() << ax_txt("\"");
//	if( buildin ) req << ax_txt(" buildin");
}

ax_NullableObj< Func >	MetaNode::getFunc	( const ax_string & name ) {
	ax_if_not_let( p, children.tryGetValue( name ) ) {
		return nullptr;
	}else{
		return p->ax_as< Func >();
	}
}

ax_Obj< Func >			MetaNode::getOrAddFunc	( const ax_string & name ) {
	ax_if_let( p, getFunc( name ) ) {
		return p;
	}
	return addFunc( name );
}

ax_Obj< Func >			MetaNode::addFunc	( const ax_string & name ) {
	return ax_new_obj( Func, ax_ThisObj, LexerPos(), name );
}


ax_string MetaNode::getTemplateInstanceName( const ax_Array< RType > & elementTypes ) {
	ax_TempString	o;

	o.append( this->name() );
	o.append( ax_txt("<") );
	
	int c = 0;
	ax_foreach( & e, elementTypes ) {
		if( c > 0 ) o.append( ax_txt(",") );
		e.appendFullname( o, ax_txt(".") );
		c++;
	}
	o.append( ax_txt(">") );
	
	return o.to_string();
}

ax_Obj< TypeNode > TypeNode::getOrAddTemplateInstance( const ax_Array< RType > & templateParams ) {
	auto new_name = getTemplateInstanceName( templateParams );
	
	ax_if_let( p, templateInstance.tryGetValue( new_name ) ) {
		return p;
	}

	auto new_inst = ax_new_obj( Func, this->parent, this->pos, new_name );
	new_inst->buildin = buildin;
	
	templateInstance.add( new_name, new_inst );
	
	ax_foreach( & c, children ) {
		c->onParentCreateTemplateInstance( new_inst, templateParams );
	}
	
	return new_inst;
}

Namespace::Namespace( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

ax_Obj< Namespace >	Namespace::getOrAddNamespace	( const ax_string & name, LexerPos & pos ) {

	ax_if_not_let( p, children.tryGetValue( name ) ) {
		auto new_node = ax_new_obj( Namespace, ax_ThisObj, pos, name );
		return new_node;
	}

	if( ! p->ax_is< Namespace >() ) {
		Log::Error( nullptr, &pos, ax_txt("identifier already exists") );
	}

	return p->ax_cast< Namespace >();
}

TypeNode::TypeNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
	buildin = false;
}

void TypeNode::OnStringReq( ax_ToStringReq & req ) const {
	base::OnStringReq( req );
		
	if( templateParams.size() > 0 ) {
		req << ax_txt("<");
		
		ax_int c = 0;
		ax_foreach( &p, templateParams ) {
			if( c > 0 ) req << ax_txt(", ");
			req << p;
			c++;
		}
		
		req << ax_txt(">");
	}
}


TupleType::TupleType( const LexerPos & pos, const ax_string & name, const ax_Array< RType > & elementTypes_ )
: base( nullptr, pos, name ) {
	this->elementTypes.assign( elementTypes_ );
}

Typename::Typename( ax_NullableObj< MetaNode> parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

ax_Obj< TupleType >	TupleTypeTable::getOrAddTuple	( const LexerPos & pos, const ax_Array< RType > & elementTypes ) {
	auto name = getTupleName( elementTypes );
	
	ax_if_let( p, tuples.tryGetValue( name ) ) {
		return p;
	}

	auto new_tuple = ax_new_obj( TupleType, pos, name, elementTypes );
	tuples.add( name, new_tuple );
	
	return new_tuple;
}

ax_string TupleTypeTable::getTupleName( const ax_Array< RType > & elementTypes ) {
	ax_TempString	name;

	name.append( ax_txt("(") );
	
	int c = 0;
	ax_foreach( & e, elementTypes ) {
		if( c > 0 ) name.append( ax_txt(",") );
		e.appendFullname( name, ax_txt(".") );
		c++;
	}
	name.append( ax_txt(")") );
	
	return name.to_string();
}


StructType::StructType( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
	isNestedType = false;				
	g_metadata->structList.add( ax_ThisObj );
}

Prop::Prop( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name, bool is_let )
: base( parent, pos, name )
, is_let( is_let ) {
	g_metadata->propList.add( ax_ThisObj );
}

Func::Func( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name )
: base( parent, pos, name ) {
}

ax_NullableObj< FuncOverload > Func::getOverload( ax_Array< ax_Obj< FuncOverload > > & candidate, const ax_Array<FuncParam> & params ) {
	candidate.resize(0);
	
	ax_foreach( & fo, overloads ) {
		if( fo->isMatch( params ) ) {
			candidate.add( fo );
		}
	}
	
	if( candidate.size() == 1 ) return candidate[0];
	return nullptr;
}

void Func::OnStringReq( ax_ToStringReq & req ) const {
	base::OnStringReq( req );
	req.indentLevel++;
	
	ax_foreach( &fo, overloads ) {
		req.newLine();
		req.indent() << fo;
	}
	
	req.indentLevel--;
}

FuncParam &	FuncOverload::addParam( const ax_string & name, const LexerPos & namePos, const RType & type, const LexerPos & typePos ) {
	auto & o = params.addNew();
	
	o.name		= name;
	o.namePos	= namePos;
	o.type		= type;
	o.typePos	= typePos;
	
	return o;
}

bool	FuncOverload::isMatch ( const ax_Array< FuncParam > & callParams ) {
	ax_int i = 0;
	ax_foreach( & c, callParams ) {
		if( i >= params.size() ) return false;
		auto & p = params[i];
		
		if( c.type != p.type ) return false;

		i++;
	}
	
//	auto fullname = func->getFullname( ax_txt(".") );
	
	if( i != params.size() ) {
		return false;
	}
	
	return true;
}

void FuncOverload::OnStringReq( ax_ToStringReq & req ) const {
	base::OnStringReq( req );
	
	ax_int c = 0;
	req << ax_txt(" (");
	
	if( params.size() > 0 ) {
		req << ax_txt(" ");
		ax_foreach( &p, params ) {
			if( c > 0 ) req << ax_txt(", ");
			req << p.name;
			req << ax_txt(":") << p.type;
			c++;
		}
		req << ax_txt(" ");
	}
	req << ax_txt(")");
}

FuncOverload::FuncOverload( ax_Obj< Func > fn, const LexerPos & pos )
: base( nullptr, pos, fn->name() ) {
	func = fn;
	g_metadata->funcOverloadList.add( ax_ThisObj );
	fn->overloads.add( ax_ThisObj );
}



}} //namespace
