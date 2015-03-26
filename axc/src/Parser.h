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

namespace ax {
namespace Compile {

class Parser {
public:
	Parser( Token & token, LexerPos & pos ) : token(token), pos(pos), lexer( token, pos ) {}

	void	reset 	( ax_Obj< SourceFile > sourceFile );

	void	nextToken			();
	
	void	skipNewLines		();

	void	skipExpression		();
	void	skipRoundBracket	();
	void	skipCurlyBracket	();
	void	skipSquareBracket	();
	void	skipAngleBracket	();

	void	_skipBracket		( TokenType openToken, TokenType closeToken );
	
	void	skipTypeName		();
	void	_skipTypeName		();
	
	DeclarationModifier	parseDeclarationModifier();
	
	
	Token &		token;
	LexerPos &	pos;
	Lexer		lexer;
		
};

}} //namespace 

#endif /* defined(__axc__Parser__) */
