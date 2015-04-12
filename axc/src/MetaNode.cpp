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
ax_ImplObject( TypeSpec );
ax_ImplObject( PrimitiveTypeSpec );
ax_ImplObject( TypenameSpec );

ax_ImplObject( TemplateParam );

ax_ImplObject( CompositeTypeSpec );
ax_ImplObject( Interface );
ax_ImplObject( Struct );
ax_ImplObject( Class );

ax_ImplObject( Prop );
ax_ImplObject( Func );
ax_ImplObject( FuncOverload );
ax_ImplObject( FuncType );

ax_string	k_ctor_name = ax_txt("ctor");

#if 0
#pragma mark =========================
#endif


MetaNode::MetaNode( ax_NullableObj< MetaNode > parent_, const ax_string & name_, const Location & pos_ )
: parent( parent_ )
, _name( name_ )
, macro_cppName( false )
, pos( pos_ )
, buildin( false )
, isTemplateInstance( false ) {

//	ax_log( ax_txt("new {?} parent={?}"), getFullname( ax_txt(".") ), ax_get( parent, name(), ax_txt("") ) );
	
	ax_if_let( p, parent ) {
		p->children.add( name_, ax_ThisObj );
	}
}

void MetaNode::onInit() {
}

void	MetaNode::onCopy( ax_Obj< MetaNode > src ) {
	pos					= src->pos;
	buildin				= src->buildin;
	isTemplateInstance	= src->isTemplateInstance;
	
	ax_foreach( &c, src->children ) {
		c->clone( ax_ThisObj );
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
	req << getTypeInfo().name() << ax_txt(" ");
	req << name() << ax_txt(" ");
}

ax_NullableObj< Func >	MetaNode::getFunc	( const ax_string & name ) {
	ax_if_not_let( p, children.tryGetValue( name ) ) {
		return nullptr;
	}else{
		return p->ax_as< Func >();
	}
}

ax_Obj< Func >	MetaNode::getOrAddFunc	( const ax_string & name ) {
	ax_if_let( p, getFunc( name ) ) {
		return p;
	}
	return addFunc( name );
}

ax_Obj< Func >	MetaNode::addFunc	( const ax_string & name ) {
	return ax_new_obj( Func, ax_ThisObj, name, Location() );
}

ax_string MetaNode::getTemplateInstanceName( const ax_Array< Type > & elementTypes ) {
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

#if 0
#pragma mark =========================
#endif

void TemplateParam::onInit() {
}

void TemplateParam::onCopy( ax_Obj< TemplateParam > p ) {
	type = p->type;
}

void TemplateParam::OnStringReq( ax_ToStringReq & req ) const {
	base::OnStringReq( req );

	req << type;
}

TemplateReplaceReq &	TemplateReplaceReq::operator<<( ax_Obj< TemplateParam > rhs ) {
	Type	new_value;
	if( dict.tryGetValue( rhs, new_value ) ) {
		rhs->type = new_value;
	}
	return *this;
}

TemplateReplaceReq &	TemplateReplaceReq::operator<<( Type & rhs ) {
	ax_if_let( t, rhs.type ) {
		if( dict.tryGetValue( t, rhs ) ) {
			ax_log( ax_txt("template replace {?} -> {?}"), t, rhs );
		}
	}

	return *this;
}

#if 0
#pragma mark =========================
#endif

void TypeSpec::onInit() {
}

void TypeSpec::onCopy( ax_Obj< TypeSpec > src ) {
	modifier 		= src->modifier;
	_templateParams	= src->_templateParams;
}

ax_Obj< TemplateParam > TypeSpec::addTemplateParam( const ax_string & name, const Location & pos ) {
	auto p = ax_new_obj( TemplateParam, ax_ThisObj, name, pos );

	_templateParams.add( p );
	
	return p;
}

void TypeSpec::onVisit( TemplateReplaceReq & req ) {
//	ax_foreach( &p, _templateParams ) {
//		req << p;
//	}
}

ax_Obj< TypeSpec > TypeSpec::getOrAddTemplateInstance( const ax_Array< Type > & params, const Location & pos ) {
	if( _templateParams.size() != params.size() ) {
		Log::Error( pos, ax_txt("invalid number of template parameters") );
	}

	auto new_name = getTemplateInstanceName( params );
	
	ax_if_let( p, templateInstance.tryGetValue( new_name ) ) {
		return p;
	}
	
	auto new_inst = this->clone( new_name )->ax_cast<TypeSpec>();
		
	templateInstance.add( new_name, new_inst );

	TemplateReplaceReq	req;
	
	ax_int n = params.size();
	for( ax_int i=0; i<n; i++ ) {
		req.dict.add( _templateParams[i], params[i] );
	}
	
	new_inst->onDeepVisit( req );

	g_compiler->dumpMetadata();
	
	return new_inst;
}

void TypeSpec::OnStringReq( ax_ToStringReq & req ) const {
	base::OnStringReq( req );
		
	if( _templateParams.size() > 0 ) {
		req << ax_txt("<");
		
		ax_int c = 0;
		ax_foreach( &p, _templateParams ) {
			if( c > 0 ) req << ax_txt(", ");
			req << p;
			c++;
		}
		
		req << ax_txt(">");
	}
}

#if 0
#pragma mark =========================
#endif


void Namespace::onInit() {
}

void Namespace::onCopy( ax_Obj< Namespace > p ) {

}

ax_Obj< Namespace >	Namespace::getOrAddNamespace	( const ax_string & name, Location & pos ) {

	ax_if_not_let( p, children.tryGetValue( name ) ) {
		auto new_node = ax_new_obj( Namespace, ax_ThisObj, name, pos );
		return new_node;
	}

	if( ! p->ax_is< Namespace >() ) {
		Log::Error( nullptr, &pos, ax_txt("identifier already exists") );
	}

	return p->ax_cast< Namespace >();
}

#if 0
#pragma mark =========================
#endif

void CompositeTypeSpec::onInit() {
	isNestedType = false;
	
	g_metadata->structList.add( ax_ThisObj );
}

void CompositeTypeSpec::onCopy( ax_Obj< CompositeTypeSpec > p ) {
	isNestedType = p->isNestedType;
}

#if 0
#pragma mark =========================
#endif

void PrimitiveTypeSpec::onInit() {
	buildin=true;
}

void PrimitiveTypeSpec::onCopy( ax_Obj< PrimitiveTypeSpec > p ) {

}

#if 0
#pragma mark =========================
#endif

void TypenameSpec::onInit() {
}

void TypenameSpec::onCopy( ax_Obj< TypenameSpec > p ) {

}

#if 0
#pragma mark =========================
#endif

void Struct::onInit() {
}

void Struct::onCopy( ax_Obj< Struct > p ) {
}


#if 0
#pragma mark =========================
#endif

void Class::onInit() {
}

void Class::onCopy( ax_Obj< Class > p ) {
}



#if 0
#pragma mark =========================
#endif

void Interface::onInit() {
}

void Interface::onCopy( ax_Obj< Interface > p ) {
}

#if 0
#pragma mark =========================
#endif

void Prop::onInit() {
	g_metadata->propList.add( ax_ThisObj );
}

void Prop::onCopy( ax_Obj< Prop > p ) {
}

#if 0
#pragma mark =========================
#endif

void Func::onInit() {
}

void Func::onCopy( ax_Obj<Func> p ) {
	ax_foreach( &c, children ) {
		ax_if_let( fo, c->ax_as<FuncOverload>() ) {
			overloads.add( fo );
		}
	}
}

ax_Obj< FuncOverload > Func::addOverload( const Location & pos ) {
	auto fo_name = ax_format( ax_txt("fo{?}"), overloads.size() );

	auto fo = ax_new_obj( FuncOverload, ax_ThisObj, fo_name, pos );
	
	overloads.add( fo );
	
	return fo;
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

#if 0
#pragma mark =========================
#endif

FuncParam &	FuncOverload::addParam( const ax_string & name, const Location & namePos, const Type & type, const Location & typePos ) {
	auto & o = params.addNew();
	
	o.name		= name;
	o.namePos	= namePos;
	o.type		= type;
	o.typePos	= typePos;
	
	return o;
}

void	FuncOverload::onVisit( TemplateReplaceReq & req ) {
	ax_dump( this->fullname() );

	ax_foreach( &p, params ) {
		req << p.type;
	}
	
	req << returnType;
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
	
	req << ax_txt(" : ") << returnType;
}

void FuncOverload::onInit() {
	g_metadata->funcOverloadList.add( ax_ThisObj );
	ax_if_not_let( p, parent ) {
		Log::Error( pos, ax_txt("func overload parent must be func") );
	}
	func = p->ax_cast<Func>();
}

void FuncOverload::onCopy( ax_Obj< FuncOverload > src ) {
	params			= src->params;

	paramPos		= src->paramPos;
	
	returnType		= src->returnType;
	returnTypePos	= src->returnTypePos;
	
	bodyPos			= src->bodyPos;
}

}} //namespace
