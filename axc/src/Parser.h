//
//  Parser.h
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__Parser__
#define __axc__Parser__

#include "Lexer.h"
#include "DeclarationModifier.h"
#include "ExprAST.h"

namespace ax {
namespace Compile {

class Parser {
public:
	Parser() : lexer( pos ) {}

	void	reset 	( ax_Obj< SourceFile > sourceFile );
	void	setPos	( const LexerPos & pos );

	void	nextToken			();
	
	void	skipSemicolonOrNewlines();
	
	void	skipUntilEndOfLine	();
	void	skipUntil( TokenType t );

	void	skipExpression		();
	void	skipRoundBracket	();
	void	skipCurlyBracket	();
	void	skipSquareBracket	();
	void	skipAngleBracket	();

	void	_skipBracket		( TokenType openToken, TokenType closeToken );
	
	void	skipTypeName		();
	void	_skipTypeName		();
	
	DeclarationModifier	parseDeclarationModifier();
	
	ax_NullableObj< ExprAST >		parseExpression();
	
	ax_NullableObj< ExprAST >		parseExpr_Primary();
	ax_NullableObj< ExprAST >		parseExpr_Identifier		();
	ax_NullableObj< ExprAST >		parseExpr_NumberLiteral		();
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


	ax_NullableObj< TypeNode > 		parseType	();
	ax_NullableObj< MetaNode >		parseNode	();
	
	Token 		token;
	LexerPos	pos;
	Lexer		lexer;
	
};

}} //namespace 

#endif /* defined(__axc__Parser__) */
