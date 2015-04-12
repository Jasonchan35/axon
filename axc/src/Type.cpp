//
//  Type.cpp
//  axc
//
//  Created by Jason on 2015-03-29.
//  Copyright (c) 2015 Jason Chan. All rights reserved.
//

#include "Type.h"
#include "MetaData.h"

namespace ax {
namespace Compile {

void	Type::appendFullname	( ax_MutString & fullname, const ax_string & seperator ) const {
	ax_if_not_let( t, type ) {
		fullname << ax_txt("<NULL>");
		return;
	}
	
	t->appendFullname( fullname, seperator );
}

bool Type::operator==( const Type & rhs ) const {
	ax_if_not_let( t, type 		) { return false; }
	ax_if_not_let( r, rhs.type 	) { return false; }
	return t == r;
}

bool Type::canAssignFrom( const Type & rhs ) const {
	ax_if_not_let( t, type 		) { return false; }
	ax_if_not_let( r, rhs.type 	) { return false; }
	
	return t->canAssignFrom( r );
}

void	Type::OnStringReq( ax_ToStringReq & req ) const {
	ax_if_let( t, type ) {
		req << t->name();
	}else{
		req << ax_txt("Type<NULL>");
	}
}

ax_NullableObj< Func >	Type::getFunc ( const ax_string & name ) {
	ax_if_not_let( t, type ) return nullptr;
	return t->getFunc(name);
}

ax_NullableObj< Func >	Type::getOperatorFunc	( TokenType op, const Location & pos ) {
	ax_if_not_let( t, type ) return nullptr;

	if( isTypename ) {
		if( op == TokenType::t_op_call ) {
			return t->getFunc( k_ctor_name );
		}
		
		if( op == TokenType::t_op_subscript ) {
			auto a = g_metadata->type_array;
			
			ax_Array_< Type, 32 > templateParam;
			templateParam.add( *this );
			
			auto ai = a->getOrAddTemplateInstance( templateParam, pos );
			return ai->getFunc( ax_txt("New") );
		}
	
		return nullptr;
		
	}else{
		if( op == TokenType::t_op_call ) {
			ax_if_let( ft, t->ax_as< FuncType >() ) {
				return ft->func;
			}
		}
		
		return t->getOperatorFunc(op);
	}
}

ax_NullableObj< Func >	Type::getPrefixOperatorFunc	( TokenType op, const Location & pos ) {
	ax_if_not_let( t, type ) return nullptr;
	return t->getPrefixOperatorFunc(op);
}

void RValue::OnStringReq( ax_ToStringReq & req ) const {
	req << name << ax_txt(":") << type;
}

}} //namespace

