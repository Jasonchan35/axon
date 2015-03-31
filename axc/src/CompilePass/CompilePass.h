//
//  CompilePass.h
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__CompilePass__
#define __axc__CompilePass__

#include "../Parser.h"
#include "../Compiler.h"
#include "../ExprAST.h"

namespace ax {
namespace Compile {

class CompilePass {
public:
	CompilePass() : parser( token, pos ) {}

	ax_NullableObj< ExprAST >		parseExpression();
	
	ax_NullableObj< ExprAST >		parseExpr_Primary();
	ax_NullableObj< ExprAST >		parseExpr_Identifier		();
	ax_NullableObj< ExprAST >		parseExpr_Number			();
	ax_NullableObj< ExprAST >		parseExpr_StringLiteral		();

//	ax_NullableObj< ExprAST >		parseExpr_Tuple				( ax_Obj<ExprAST> & first );
//	ax_NullableObj< ExprAST >		parseExpr_ArrayOrDictConst	();
//	
	ax_NullableObj< ExprAST >		parseExpr_BinaryOp			( ax_int exprPrec, ax_Obj<ExprAST> & lhs );

//	ax_NullableObj< ExprAST >		parseExpr_If				();
//	ax_NullableObj< ExprAST >		parseExpr_While				();
//	ax_NullableObj< ExprAST >		parseExpr_DoWhile			();
//	ax_NullableObj< ExprAST >		parseExpr_For				();
//
//	ax_NullableObj< ExprAST >		parseExpr_Return			();
//	ax_NullableObj< ExprAST >		parseExpr_Break				();
//	ax_NullableObj< ExprAST >		parseExpr_Continue			();
//
//	ax_NullableObj< ExprAST >		parseExpr_LocalVar			( DeclarationModifier & modifier );


	ax_NullableObj< TypedNode > 	parseTypename	();
	ax_NullableObj< MetaNode >		parseNode		();

	void	nextToken();

	Token		token;
	LexerPos	pos;
	Parser		parser;
};

}} //namespace


#endif /* defined(__axc__CompilePass__) */
