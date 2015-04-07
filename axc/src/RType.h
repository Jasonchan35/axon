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
	struct	ax_type_on_gc_trace : public std::true_type {};

	RType()
	: type( nullptr )
	, isMutable( false )
	, isInstance( false )
	, isTypename( false ) {
	}
	
	static RType	MakeLiteral( ax_Obj< TypeNode > type_ ) {
		RType o;
		o.type = type_;
		o.isInstance = true;
		return o;
	}
	
	static RType	MakePrimitiveValue( ax_Obj< TypeNode > type_ ) {
		RType o;
		o.type = type_;
		o.isInstance = true;
		return o;
	}
	
	static RType	MakeTypename( ax_Obj< TypeNode > type_ ) {
		RType o;
		o.type = type_;
		o.isTypename = true;
		return o;
	}

	bool	is_null() const { return type.is_null(); }
	bool	canAssignFrom( const RType & rhs ) const;
	

	ax_NullableObj< Func >	getFunc					( const ax_string & name );
	ax_NullableObj< Func >	getOperatorFunc			( TokenType op );
	ax_NullableObj< Func >	getPrefixOperatorFunc	( TokenType op );

	void	OnStringReq( ax_ToStringReq & req ) const;

	void	appendFullname	( ax_MutString & fullname, const ax_string & seperator ) const;

	ax_NullableObj< TypeNode >	type;
	bool	isMutable	:1;
	bool	isInstance	:1;
	bool	isTypename	:1;
};

}} //namespace

#endif /* defined(__axc__RType__) */
