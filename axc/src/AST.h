//
//  AST.h
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

class TypeAST;

class PropAST;
class NumberLiteralAST;
class StringLiteralAST;

class PrefixAST;
class PostfixAST;
class BinaryAST;

class FuncParamAST;

class FuncOverload;
class Prop;
class TypeNode;

class IASTDispatcher {
public:
	virtual	void	onAST( TypeAST		 	& p ) = 0;
	virtual	void	onAST( PropAST 			& p ) = 0;

	virtual	void	onAST( NumberLiteralAST & p ) = 0;
	virtual	void	onAST( StringLiteralAST & p ) = 0;
//	virtual	void	onAST( ArrayConstAST 	& p ) = 0;
//	virtual	void	onAST( DictConstAST 	& p ) = 0;

	virtual	void	onAST( PrefixAST 		& p ) = 0;
	virtual	void	onAST( PostfixAST 		& p ) = 0;
	virtual	void	onAST( BinaryAST 		& p ) = 0;

//	virtual	void	onAST( TupleAST 		& p ) = 0;
	virtual	void	onAST( FuncParamAST 		& p ) = 0;
//	virtual	void	onAST( SubscriptArgAST 	& p ) = 0;

//	virtual	void	onAST( StatementsAST	& p ) = 0;
//	virtual	void	onAST( IfAST 			& p ) = 0;
//	virtual	void	onAST( WhileAST 		& p ) = 0;
//	virtual	void	onAST( DoWhileAST 		& p ) = 0;
//	virtual	void	onAST( ForAST 			& p ) = 0;
	
//	virtual	void	onAST( ReturnAST 		& p ) = 0;
//	virtual	void	onAST( BreakAST 		& p ) = 0;
//	virtual	void	onAST( ContinueAST 		& p ) = 0;
//
//	virtual	void	onAST( LocalVarAST 		& p ) = 0;
};

class AST : public ax_Object {
	ax_DefObject( AST, ax_Object );
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	AST	( const Location & pos_, const RType & returnType_ );
	
			void	dispatch		( IASTDispatcher & p ) { onDispatch(p); }
	virtual	void	onDispatch		( IASTDispatcher & p ) = 0;
	
	Location	pos;
	RType		returnType;
};


class NumberLiteralAST : public AST {
	ax_DefObject( NumberLiteralAST, AST )
public:

	virtual	void	onDispatch		( IASTDispatcher & p ) { p.onAST(*this); }

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

	NumberLiteralAST( const Location &_pos, const ax_string & _srcStr );
};

class StringLiteralAST : public AST {
	ax_DefObject( StringLiteralAST, AST )
public:
	ax_string	value;
	StringLiteralAST( const Location &pos_, const ax_string & value_ );

	virtual	void	onDispatch		( IASTDispatcher & p ) { p.onAST(*this); }
};


class TypeAST : public AST {
	ax_DefObject( TypeAST, AST )
public:
	ax_NullableObj< TypeNode >		node;

	TypeAST( Location &pos_, ax_Obj< TypeNode > node_ );

	virtual	void	onDispatch		( IASTDispatcher & p ) { p.onAST(*this); }
};

class PropAST : public AST {
	ax_DefObject( PropAST, AST )
public:

	PropAST( Location &pos_, ax_Obj< Prop > node_ );
	virtual	void	onDispatch		( IASTDispatcher & p ) { p.onAST(*this); }
	
	ax_NullableObj< Prop >		node;
};


class PrefixAST : public AST {
	ax_DefObject( PrefixAST, AST )
public:

	ax_Obj< FuncOverload >	funcOverload;
	ax_Obj<AST>			expr;
	
	PrefixAST( const Location &pos_,  ax_Obj< FuncOverload > fo_, ax_Obj< AST > expr_  );
	
	virtual	void	onDispatch		( IASTDispatcher & p ) { p.onAST(*this); }
};

class PostfixAST : public AST {
	ax_DefObject( PostfixAST, AST )
public:
	ax_Obj< FuncOverload >	funcOverload;
	ax_Obj<AST>			expr;
	
	PostfixAST( const Location &pos_, ax_Obj< FuncOverload > fo_, ax_Obj< AST > expr_ );
	
	virtual	void	onDispatch		( IASTDispatcher & p ) { p.onAST(*this); }
};


class BinaryAST : public AST {
	ax_DefObject( BinaryAST, AST )
public:
	ax_Obj< FuncOverload >	funcOverload;
	ax_Obj< AST >		lhs, rhs;
	
	BinaryAST( const Location & pos_, ax_Obj< FuncOverload > fo_, ax_Obj<AST> lhs_, ax_Obj<AST> rhs_, bool parenthesis_ );
	
	virtual	void	onDispatch		( IASTDispatcher & p ) { p.onAST(*this); }
	
	bool	parenthesis : 1;
};


class FuncParamAST : public AST {
	ax_DefObject( FuncParamAST, AST )
public:
	ax_Array_< ax_Obj< AST >, 8 >	args;
	
	FuncParamAST( const Location &_pos );

	virtual	void	onDispatch		( IASTDispatcher & p ) { p.onAST(*this); }

};


}} //namespace

#endif /* defined(__axc__ExprAST__) */
