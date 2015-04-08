//
//  RType.cpp
//  axc
//
//  Created by Jason on 2015-03-29.
//  Copyright (c) 2015 Jason Chan. All rights reserved.
//

#include "RType.h"
#include "MetaData.h"

namespace ax {
namespace Compile {

void	RType::appendFullname	( ax_MutString & fullname, const ax_string & seperator ) const {
	ax_if_let( t, type ) {
		fullname << ax_txt("<NULL>");
		return;
	}
	
	t->appendFullname( fullname, seperator );
}

bool RType::canAssignFrom( const RType & rhs ) const {
	ax_if_not_let( t, type 		) { return false; }
	ax_if_not_let( r, rhs.type 	) { return false; }
	
	return t->canAssignFrom( r );
}

void	RType::OnStringReq( ax_ToStringReq & req ) const {
	req << ax_txt("RType[") << type << ax_txt("]");
}

ax_NullableObj< Func >	RType::getFunc ( const ax_string & name ) {
	ax_if_not_let( t, type ) return nullptr;
	return t->getFunc(name);
}

ax_NullableObj< Func >	RType::getOperatorFunc		( TokenType op ) {
	ax_if_not_let( t, type ) return nullptr;

	if( isTypename ) {
		if( op == TokenType::t_op_call ) {
			return t->getFunc( k_ctor_name );
		}
		
		if( op == TokenType::t_op_subscript ) {
			return g_metadata->type_array->getFunc( k_ctor_name );
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

ax_NullableObj< Func >	RType::getPrefixOperatorFunc	( TokenType op ) {
	ax_if_not_let( t, type ) return nullptr;
	return t->getPrefixOperatorFunc(op);
}


}} //namespace

