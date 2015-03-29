//
//  ExprAST.h
//  axc
//
//  Created by Jason on 2015-03-29.
//  Copyright (c) 2015 Jason Chan. All rights reserved.
//

#ifndef __axc__ExprAST__
#define __axc__ExprAST__

#include "RType.h"

namespace ax {
namespace Compile {

class ExprAST : public System::Object {
	ax_DefObject( ExprAST, System::Object );
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	ExprAST	( const LexerPos & pos_ ) : pos(pos_) {}
	LexerPos	pos;

	RType		returnType;
};


class NumberAST : public ExprAST {
	ax_DefObject( NumberAST, ExprAST )
public:
/*
	0x11		hex
	0o11		oct
	0b11		bin
	11			int
	11.0f		float
	11.0		double
	1.1e2		double
	
	0011		still dec, not oct
	1_000_000	int
*/
	ax_string	srcStr;
	
	enum NumberType{
		t_none,
		
		t_int,
		t_uint,
		
		t_int32,
		t_int64,
		
		t_uint32,
		t_uint64,
		
		t_float,
		t_double,
	};
	
	NumberType	numberType;
	
	union NumberValue {
		ax_int		v_int;
		ax_uint		v_uint;
	
		int32_t		v_int32;
		int64_t		v_int64;
		
		uint32_t	v_uint32;
		uint64_t	v_uint64;
		
		float		v_float;
		double		v_double;
	};
	
	NumberValue	numberValue;
	char	numberPrefix;

	NumberAST( const LexerPos &_pos, const ax_string & _srcStr );
};

class StringLiteralAST : public ExprAST {
	ax_DefObject( StringLiteralAST, ExprAST )
public:
	ax_string	value;
	StringLiteralAST( const LexerPos &pos_, const ax_string & value_ )
	: base(pos_)
	, value(value_) {
	}
};


class IdentifierAST : public ExprAST {
	ax_DefObject( IdentifierAST, ExprAST )
public:
	ax_Obj< MetaNode >		node;

	bool			dot;
	RType		dotType;
	
	IdentifierAST( LexerPos &pos_, ax_Obj< MetaNode > node_ )
	: base(pos_)
	, node(node_) {
	}
};


class PrefixAST : public ExprAST {
	ax_DefObject( PrefixAST, ExprAST )
public:
	TokenType			op;
	ax_Obj<ExprAST>		expr;
	
	PrefixAST( const LexerPos &pos_, TokenType op_ ) : base(pos_), op(op_) {}
	
};

class PostfixAST : public ExprAST {
	ax_DefObject( PostfixAST, ExprAST )
public:
	TokenType			op;
	ax_Obj<ExprAST>		expr;
	
	PostfixAST( const LexerPos &pos_, TokenType op_, ax_Obj< ExprAST > expr_ ) : base(pos_), op(op_), expr(expr_) {}
	
};


class BinaryAST : public ExprAST {
	ax_DefObject( BinaryAST, ExprAST )
public:
	TokenType			op;
	
	ax_Obj< ExprAST >	lhs, rhs;
	
	BinaryAST( const LexerPos & pos_, TokenType op_, ax_Obj<ExprAST> lhs_, ax_Obj<ExprAST> rhs_, bool parenthesis_ )
	: base(pos_)
	, op(op_)
	, lhs( lhs_ )
	, rhs( rhs_ )
	, parenthesis( parenthesis_ )
	{
	}
	
	bool	parenthesis : 1;
};


class FuncArgAST : public ExprAST {
	ax_DefObject( FuncArgAST, ExprAST )
public:
	ax_Array_< ax_Obj< ExprAST >, 8 >	args;
	
	FuncArgAST( const LexerPos &_pos ) : base(_pos) {}

};



}} //namespace

#endif /* defined(__axc__ExprAST__) */