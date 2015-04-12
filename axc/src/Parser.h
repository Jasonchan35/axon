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
#include "Modifier.h"
#include "AST.h"

namespace ax {
namespace Compile {

class Parser {
public:
	Parser() : lexer( pos ) {}

	void	reset 	( ax_Obj< SourceFile > sourceFile );
	void	setPos	( const Location & pos );

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
	
	Modifier	parseDeclarationModifier();
	
	ax_NullableObj< AST >		parseExpression();
	
	ax_NullableObj< AST >		parseExpr_Primary();
	ax_NullableObj< AST >		parseExpr_Identifier		();
	ax_NullableObj< AST >		parseExpr_NumberLiteral		();
	ax_NullableObj< AST >		parseExpr_StringLiteral		();

//	ax_NullableObj< AST >		parseExpr_Tuple				( ax_Obj<AST> & first );
//	ax_NullableObj< AST >		parseExpr_ArrayOrDictConst	();
//	
	ax_NullableObj< AST >		parseExpr_BinaryOp			( ax_int exprPrec, ax_Obj<AST> & lhs );

//	ax_NullableObj< AST >		parseExpr_If				();
//	ax_NullableObj< AST >		parseExpr_While				();
//	ax_NullableObj< AST >		parseExpr_DoWhile			();
//	ax_NullableObj< AST >		parseExpr_For				();
//
//	ax_NullableObj< AST >		parseExpr_Return			();
//	ax_NullableObj< AST >		parseExpr_Break				();
//	ax_NullableObj< AST >		parseExpr_Continue			();
//
//	ax_NullableObj< AST >		parseExpr_LocalVar			( Modifier & modifier );


						Type 		parseTypename	();
	ax_NullableObj< MetaNode >		parseNode	();
	
	Token 		token;
	Location	pos;
	Lexer		lexer;
	
};

}} //namespace 

#endif /* defined(__axc__Parser__) */
