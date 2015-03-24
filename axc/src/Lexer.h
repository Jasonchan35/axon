//
//  Lexer.h
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__Lexer__
#define __axc__Lexer__

#include "define.h"
#include "LexerPos.h"
#include "Token.h"
#include "Log.h"
#include "SourceFile.h"

namespace ax {
namespace Compile {

class Lexer {
public:
	Lexer	( Token & token, LexerPos & pos ) : token( token ), c(pos) {}

	void	reset 	( ax_Obj< SourceFile > sourceFile );

	bool	getToken	( Token & token );
	void	_getToken	( Token & token );
	void	_getToken_number	( Token & token, bool dot );
	
	void	_setToken( Token & token, TokenType type, const ax_string & str );
	
	void	nextToken	();

	Token &	token;
	
	
	class	Cursor {
	public:
		Cursor( LexerPos & pos );
		
		void reset( ax_Obj< SourceFile > sourceFile );

		void setPos( LexerPos & pos );
		
		void operator++		(int) 	{ next(); }
		void operator++		() 		{ next(); }
		void next			();
		
		void trimSeperators	();
		
		operator	char	() const	{ return ch; }
		char		getChar	() const 	{ return ch; }
		
		bool	isSeperator();
		bool	isAlpha();
		bool	isDigit();
		bool	isHex();
		
		LexerPos &	pos;
		
	private:
		const ax_char*	p;
		ax_char			ch;
	};
	
	Cursor	c;	
};

}} //namespace

#endif /* defined(__axc__Lexer__) */
