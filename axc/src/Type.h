//
//  Type.h
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

class TypeSpec;
class Func;

class Type {
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	Type()
	: type( nullptr )
	, isMutable( false )
	, isInstance( false )
	, isTypename( false ) {
	}
	
	static Type	MakeLiteral( ax_Obj< TypeSpec > type_ ) {
		Type o;
		o.type = type_;
		o.isInstance = true;
		return o;
	}
	
	static Type	MakeValue( ax_Obj< TypeSpec > type_, bool isMutable ) {
		Type o;
		o.type = type_;
		o.isMutable  = isMutable;
		o.isInstance = true;
		return o;
	}
		
	static Type	MakeTypename( ax_Obj< TypeSpec > type_ ) {
		Type o;
		o.type = type_;
		o.isTypename = true;
		return o;
	}
	
	bool	operator==( const Type & rhs ) const;
	bool	operator!=( const Type & rhs ) const { return ! operator==(rhs); }

	bool	is_null() const { return type.is_null(); }
	bool	canAssignFrom( const Type & rhs ) const;
	
	ax_NullableObj< Func >	getFunc					( const ax_string & name );
	
	ax_NullableObj< Func >	getOperatorFunc			( TokenType op, const Location & pos );
	ax_NullableObj< Func >	getPrefixOperatorFunc	( TokenType op, const Location & pos );

	void	OnStringReq( ax_ToStringReq & req ) const;

	void	appendFullname	( ax_MutString & fullname, const ax_string & seperator ) const;

	ax_NullableObj< TypeSpec >	type;
	bool	isMutable	:1;
	bool	isInstance	:1;
	bool	isTypename	:1;
};

class RValue {
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	Type		type;
	ax_string	name;
	Location	pos;
	
	void	OnStringReq( ax_ToStringReq & req ) const;
	
};

}} //namespace

#endif /* defined(__axc__RType__) */
