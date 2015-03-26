//
//  Parser.cpp
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "Parser.h"
#include "Log.h"

namespace ax {
namespace Compile {

void Parser::reset( ax_Obj< SourceFile > sourceFile ) {
	lexer.reset( sourceFile );
}


void Parser::skipTypeName	() {
	if( token.is_roundBracketOpen() ) {	//tuple
		nextToken();
		for(;;){
			skipTypeName();
			if( token.is_comma() ) 				{ nextToken(); continue;	}
			if( token.is_roundBracketClose() ) 	{ nextToken(); return;		}
			Log::Error( token, ax_txt("invalid type name") );
		}
	}

	for(;;) {
		if( ! token.is_identifier() ) Log::Error( token, ax_txt("invalid type name") );
		nextToken();
					
		if( token.is_less() ) {
			nextToken();

			for(;;) {
				skipTypeName();
				if( token.is_comma() ) 		{ nextToken(); continue;	}
				if( token.is_greater() ) 	{ nextToken(); break;		}
				Log::Error( token, ax_txt("invalid type name") );
			}
		}
		
		if( token.is_squareBracketOpen() ) {
			nextToken();
			
			for(;;) {
				if( token.is_comma() ) 				{ nextToken(); continue;	}
				if( token.is_squareBracketClose() ) { nextToken(); break;		}
				Log::Error( token, ax_txt("] is missing") );
			}
		}
		
		if( token.is_dot() ) { nextToken(); continue; }

		break;
	}
}

void Parser::nextToken() {
	for(;;){
		bool b = lexer.getToken( token );
		if( !b ) return;
		
		if( token.is_comment() ) continue;
		break;
	}
}

void Parser::skipNewLines() {
	while( token.is_newLine() ) {
		nextToken();
	}
}

void Parser::_skipBracket ( TokenType openToken, TokenType closeToken ) {
	int level = 1;
	const int maxLevel = 256;
	auto startPos = token.pos;
	
	while( level > 0 ) {
		if( token.type == openToken  ) level++;
		if( token.type == closeToken ) level--;

		if( token.is_EOF() ) {
			Log::Error( token, ax_txt("End of File: bracket mis-match\n  startPos={?}"), startPos );
		}
		
		if( level > maxLevel ) {
			Log::Error( token, ax_txt("reach max round bracket level {?}\n  startPos={?}"), maxLevel, startPos );
		}
		nextToken();
	}
}


void Parser::skipExpression () {
	for(;;) {
		if( token.is_newLine_or_semiColon() ) break;
		nextToken();
	}
}


void Parser::skipCurlyBracket () { _skipBracket( TokenType::t_curlyBracketOpen,  TokenType::t_curlyBracketClose ); }
void Parser::skipRoundBracket () { _skipBracket( TokenType::t_roundBracketOpen,  TokenType::t_roundBracketClose ); }
void Parser::skipSquareBracket() { _skipBracket( TokenType::t_squareBracketOpen, TokenType::t_squareBracketClose ); }
void Parser::skipAngleBracket () { _skipBracket( TokenType::t_less,  			  TokenType::t_greater ); }


DeclarationModifier Parser::parseDeclarationModifier() {
	DeclarationModifier	m;

	for(;;) {
		if( token.type == TokenType::t_mutating ) {
			if( m.b_mutating || m.b_nonmutating ) Log::Error( token, ax_txt("duplicate declaration specifier") );
			m.b_mutating = true;
			nextToken();
			continue;
		}

		if( token.type == TokenType::t_nonmutating ) {
			if( m.b_mutating || m.b_nonmutating ) Log::Error( token, ax_txt("duplicate declaration specifier") );
			m.b_nonmutating = true;
			nextToken();
			continue;
		}
		
		if( token.type == TokenType::t_static ) {
			if( m.b_static ) Log::Error( token, ax_txt("duplicate declaration specifier") );
			m.b_static = true;
			nextToken();
			continue;
		}

		if( token.type == TokenType::t_extern ) {
			if( m.b_extern ) Log::Error( token, ax_txt("duplicate declaration specifier") );
			m.b_extern = true;
			nextToken();
			continue;
		}

		if( token.type == TokenType::t_virtual ) {
			if( m.b_virtual ) Log::Error( token, ax_txt("duplicate declaration specifier") );
			m.b_virtual = true;
			nextToken();
			continue;
		}

		if( token.type == TokenType::t_override ) {
			if( m.b_override ) Log::Error( token, ax_txt("duplicate declaration specifier") );
			m.b_override = true;
			nextToken();
			continue;
		}

		if( token.type == TokenType::t_unowned ) {
			if( m.b_unowned || m.b_weak ) Log::Error( token, ax_txt("duplicate declaration specifier") );
			m.b_unowned = true;
			nextToken();
			continue;
		}
		
		if( token.type == TokenType::t_weak ) {
			if( m.b_unowned || m.b_weak ) Log::Error( token, ax_txt("duplicate declaration specifier") );
			m.b_weak = true;
			nextToken();
			continue;
		}
		
		break;
	}
	
	return m;
}




}} //namespace