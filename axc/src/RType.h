//
//  RType.h
//  axc
//
//  Created by Jason on 2015-03-29.
//  Copyright (c) 2015 Jason Chan. All rights reserved.
//

#ifndef __axc__RType__
#define __axc__RType__

#include "Token.h"

namespace ax {
namespace Compile {

class TypeNode;
class Func;

class RType {
public:
	static	RType	kNull;

	RType( ax_NullableObj< TypeNode > type_ = nullptr, bool isMutable_ = false )
	: type( type_ )
	, isMutable( isMutable_ )
	, isTypeName( false ) {
	}
	

	bool	is_null() const { return type.is_null(); }
	bool	canAssignFrom( const RType & rhs ) const;
	

	ax_NullableObj< Func >	getFunc					( const ax_string & name );
	ax_NullableObj< Func >	getOperatorFunc			( TokenType op );
	ax_NullableObj< Func >	getPrefixOperatorFunc	( TokenType op );

	void	OnStringReq( ax_ToStringReq & req ) const;

	ax_NullableObj< TypeNode >	type;
	bool	isMutable;
	bool	isTypeName;
};

}} //namespace

#endif /* defined(__axc__RType__) */
