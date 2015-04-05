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
	Lexer( LexerPos & pos ) : c(pos) {}

	void	setPos	( const LexerPos & pos );

	bool	getToken		( Token & token );
	void	_getToken		( Token & token );
	void	_getToken_number( Token & token, bool dot );
	
	void	_setToken( Token & token, TokenType type, const ax_string & str );
	
	class	Cursor {
	public:
		Cursor( LexerPos & pos_ );
		
		void setPos( const LexerPos & pos );
		
		void operator++		(int) 	{ next(); }
		void operator++		() 		{ next(); }
		void next			();
		
		void trimSeperators	();
		
		operator	ax_char	() const	{ return ch; }
		ax_char		getChar	() const 	{ return ch; }
		
		bool	isSeperator();
		bool	isAlpha();
		bool	isDigit();
		bool	isHex();
		
		LexerPos & pos;
		
	private:
		const ax_char*	p;
		const ax_char*	end;
		ax_char			ch;
	};
	
	Cursor	c;	
};

}} //namespace

#endif /* defined(__axc__Lexer__) */
