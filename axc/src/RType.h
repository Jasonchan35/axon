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

class TypedNode;
class FuncNode;

class RType {
public:

	RType( ax_NullableObj< TypedNode > type_ = nullptr, bool isMutable_ = false )
	: type( type_ )
	, isMutable( isMutable_ ) {
	}

	ax_NullableObj< FuncNode >	getFunc					( const ax_string & name );
	ax_NullableObj< FuncNode >	getOperatorFunc			( TokenType op );
	ax_NullableObj< FuncNode >	getPrefixOperatorFunc	( TokenType op );


	void	OnStringReq( ax_ToStringReq & req ) const;

	ax_NullableObj< TypedNode >	type;
	bool	isMutable;
};

}} //namespace

#endif /* defined(__axc__RType__) */
