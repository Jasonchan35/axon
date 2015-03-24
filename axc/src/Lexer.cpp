//
//  Lexer.cpp
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "Lexer.h"
#include "Log.h"

namespace ax {
namespace Compile {

Lexer::Cursor::Cursor( LexerPos & pos ) : pos(pos) {
	p = nullptr;
	ch = 0;
}

void Lexer::Cursor::init( ax_Obj< SourceFile > sourceFile ) {
	pos.valid = true;
	pos.file = sourceFile;
	pos.filePos = 0;
	pos.line = 1;
	pos.col  = 1;

	p = sourceFile->source.c_str();
	if( p ) {
		ch = *p;
	}
}

void Lexer::Cursor::setPos( LexerPos & pos ) {
	this->pos = pos;
	
	p = nullptr;
	
	ax_if_let( sourceFile, pos.file ) {
		auto & source = sourceFile->source;
		if( pos.filePos >= source.size() ) return Log::Error( nullptr, &pos, "Error setCursor file position out of range" );
		p = source.c_str() + pos.filePos;
	}

	if( p ) {
		ch = *p;
	}
}


void Lexer::Cursor::next() {
	if( !p ) return;

	pos.filePos++;
	
	if( *p == '\n' ) {
		pos.line++;
		pos.col=1;
	}else{
		pos.col++;
	}
	
	p++;
	ch = *p;
}

void Lexer::Cursor::trimSeperators() {
	for( ;; next() ) {
		if( ! isSeperator() ) break;
	}
}

bool Lexer::Cursor::isSeperator() {
	return ch == ' ' || ch == '\t' || ch == '\r'; // || ch == '\n';
}

bool Lexer::Cursor::isAlpha() {
	return ax_isalpha(ch);
}

bool Lexer::Cursor::isDigit() {
	return ax_isdigit(ch);
}

bool Lexer::Cursor::isHex() {
	return ax_ishex(ch);
}


//===================================


void	Lexer::reset ( ax_Obj< SourceFile > sourceFile ) {
	c.reset( sourceFile );
}



bool	Lexer::getToken		( Token & token ) {
	_getToken(token);
	
	#if 0 // debug token
		ax_log( "--- token {?}", token );
	#endif

	switch( token.type ) {
		case TokenType::t_EOF: return false;
		case TokenType::t_curlyBracketOpen:  c.pos.blockLevel++; break;
		case TokenType::t_curlyBracketClose: c.pos.blockLevel--; break;
		default: break;
	}
	
	return true;
}

void	Lexer::_setToken( Token & token, TokenType type, const ax_string & str ) {
	token.type = type;
	token.str = str;
}


void	Lexer::_getToken( Token & token ) {
	token.type = TokenType::t_unknown;
	token.str.clear();

	c.trimSeperators();
	token.pos = c.pos;
	
	switch( c.getChar() ) {
		case 0: {
			return _setToken( token, TokenType::t_EOF, ax_txt("") );
		}break;
		
		case '\n': {
			c++;
			return _setToken( token, TokenType::t_newLine, ax_txt("<newLine>") );
		}break;
		
		case '=': {
			c++;
			if( c == '=' ) {
				c++;
				if( c == '=' ) {
					c++;
					return _setToken( token, TokenType::t_equal3,	ax_txt("===") );
				}				
				return _setToken( token, TokenType::t_equal,  		ax_txt("==") );
			}
			return _setToken( token, TokenType::t_assign, ax_txt("=") );
		}break;

		case '!': {
			c++;
			if( c == '.' ) { c++; return _setToken( token, TokenType::t_exclamationDot, ax_txt("!.") ); }
			if( c == '=' ) {
				c++;
				if( c == '=' ) {
					c++;
					return _setToken( token, TokenType::t_notEqual3,	ax_txt("!==") );
				}
				return _setToken( token, TokenType::t_notEqual, 		ax_txt("!=") );
			}
			return _setToken( token, TokenType::t_exclamation, ax_txt("!") );
		}break;
		
		case '<': {
			c++;
			if( c == '<' ) { c++; return _setToken( token, TokenType::t_less2, 	 			ax_txt("<<") ); }
			if( c == '>' ) { c++; return _setToken( token, TokenType::t_lessGreater, 		ax_txt("<>") ); }
			if( c == '=' ) { c++; return _setToken( token, TokenType::t_lessEqual,  		ax_txt("<=") ); }
			return _setToken( token, TokenType::t_less, ax_txt("<") );
		}break;

		case '>': {
			c++;
			if( c == '>' ) { c++; return _setToken( token, TokenType::t_greater2, 	 		ax_txt(">>") ); }
			if( c == '=' ) { c++; return _setToken( token, TokenType::t_greaterEqual,  		ax_txt(">=") ); }
			return _setToken( token, TokenType::t_greater, ax_txt(">") );
		}break;

		case '+': {
			c++;
			if( c == '+' ) { c++; return _setToken( token, TokenType::t_add2, 		 		ax_txt("++") ); }
			if( c == '=' ) { c++; return _setToken( token, TokenType::t_addAssign, 			ax_txt("+=") ); }
			return _setToken( token, TokenType::t_add, ax_txt("+") );
		}break;
		
		case '-': {
			c++;
			if( c == '-' ) { c++; return _setToken( token, TokenType::t_sub2, 		 		ax_txt("--") ); }
			if( c == '=' ) { c++; return _setToken( token, TokenType::t_subAssign, 			ax_txt("-=") ); }
			return _setToken( token, TokenType::t_sub, ax_txt("-") );
		}break;
		
		case '*': {
			c++;
			if( c == '*' ) { c++; return _setToken( token, TokenType::t_mul2, 				ax_txt("**") ); }
			if( c == '=' ) { c++; return _setToken( token, TokenType::t_mulAssign, 			ax_txt("*=") ); }
			return _setToken( token, TokenType::t_mul, ax_txt("*") );
		}break;

		case '%': {
			c++;
			if( c == '=' ) { c++; return _setToken( token, TokenType::t_modAssign, 			ax_txt("%=") ); }
			return _setToken( token, TokenType::t_mod, ax_txt("%") );
		}break;
		
		case ':': {
			c++;
			if( c == ':' ) { c++; return _setToken( token, TokenType::t_colon2, 			ax_txt("::") ); }
			if( c == '=' ) { c++; return _setToken( token, TokenType::t_colonAssign, 		ax_txt(":=") ); }
			return _setToken( token, TokenType::t_colon, ax_txt(":") );
		}break;
		
		
		case '/': {
			ax_TempString	tmp;
			c++;
			if( c == '/' ) { //skip comment line
				c++;
				tmp.append( ax_txt("//") );
			
				token.type = TokenType::t_comment;
				for( ;c; c++) {
					if( c == '\n' ) break;
					tmp.append(c);
				}
				c++;
				return;
			}
			
			if( c == '*' ) { //skip comment block
				c++;
				tmp.append( ax_txt("/*") );
				
				int level = 1;
				for( ;c; c++) {
					if( c == '*' ) {
						c++;
						if( c == '/' ) {
							level--;
							tmp.append( ax_txt("*/") );
							
							if( level == 0 ) break;
							continue;
						}
					}
					
					if( c == '/' ) {
						c++;
						if( c == '*' ) {
							level++;
							tmp.append( ax_txt("/*") );
							
							if( level > 256 ) Log::Error( nullptr, &c.pos, "excess comment block level limit" );
							continue;
						}
					}
					
					tmp.append(c);
				}
				c++;
				
				return _setToken( token, TokenType::t_comment, tmp );
			}
			
			if( c == '=' ) { c++; return _setToken( token, TokenType::t_divAssign, ax_txt("/=") ); }
			return _setToken( token, TokenType::t_div, ax_txt("/") );
		}break;
	
		case '?': {
			c++;
			if( c == '.' ) { c++; return _setToken( token, TokenType::t_questionDot, 	ax_txt("?.") ); }
			return _setToken( token, TokenType::t_question, 	ax_txt("?") );
		}break;
	
		case '[': { c++; return _setToken( token, TokenType::t_squareBracketOpen,  ax_txt("[") ); }	break;
		case ']': { c++; return _setToken( token, TokenType::t_squareBracketClose, ax_txt("]") ); }	break;
		
		case '(': { c++; return _setToken( token, TokenType::t_roundBracketOpen,  ax_txt("(") ); }	break;
		case ')': { c++; return _setToken( token, TokenType::t_roundBracketClose, ax_txt(")") ); }	break;
		
		case '{': { c++; return _setToken( token, TokenType::t_curlyBracketOpen,  ax_txt("{") ); }	break;
		case '}': { c++; return _setToken( token, TokenType::t_curlyBracketClose, ax_txt("}") ); }	break;

		case ';': { c++; return _setToken( token, TokenType::t_semiColon, 	ax_txt(";") ); }	break;

		case ',': { c++; return _setToken( token, TokenType::t_comma,      	ax_txt(",") ); }	break;
		case '`': { c++; return _setToken( token, TokenType::t_backMark, 	ax_txt("`") ); }	break;
		case '$': { c++; return _setToken( token, TokenType::t_dollar, 		ax_txt("$") ); }	break;
		case '#': { c++; return _setToken( token, TokenType::t_hash, 		ax_txt("#") ); }	break;
		case '@': { c++; return _setToken( token, TokenType::t_at,		 	ax_txt("@") ); }	break;
		case '\\':{ c++; return _setToken( token, TokenType::t_backSlash,	ax_txt("\\") ); }	break;
		

		//String
		case '\"': {
			ax_TempString	tmp;
			c++;
			for( ; c; c++ ) {
				if( c == '\\' ) {
					c++;
					if( !c ) {
						Log::Error( nullptr, &c.pos, "Unexpected end of string" );
					}
					
						 if( c == '\0') { tmp.append('\0'); }
					else if( c == '"' ) { tmp.append('\"'); }
					else if( c == '\'') { tmp.append('\''); }
					else if( c == '\\') { tmp.append('\\'); }
					
					else if( c == 'n' ) { tmp.append('\n'); }	// newline
					else if( c == 't' ) { tmp.append('\t'); }	// tab
					else if( c == 'r' ) { tmp.append('\r'); }	// carriage return
					
					else if( c == 'a' ) { tmp.append('\a'); }	// audible bell
					else if( c == 'b' ) { tmp.append('\b'); }	// backspace
					else if( c == 'f' ) { tmp.append('\f'); }	// form feed - new page
					else if( c == 'u' ) {
						uint32_t wc = 0;
						for( int i=0; i<4; i++ ) {
							c++;
							auto ch = c.getChar();
							if( ch >= '0' && ch <= '9' ) {
								wc <<= 4;	wc |= ch -'0';
							}else if( ch >= 'A' && ch <= 'F' ) {
								wc <<= 4;	wc |= ch -'A' + 10;
//							}else if( ch >= 'a' && ch <= 'f' ) {
//								wc <<= 4;	wc |= ch -'a';
							}else{
								Log::Error( nullptr, &c.pos, "unicode hex expected [0-9,A-F]" );
							}
						}
						tmp.append( (wchar_t)wc );
					
					}else {
						Log::Error( nullptr, &c.pos, "unknown string literal escape sequence [{?}]", c.getChar() );
					}
					continue;
				}
				
				if( c == '\"' ) break;
				tmp.append(c);
			}
			c++;
			
			return _setToken( token, TokenType::t_stringLiteral, tmp );
		}break;
		
		// char
		case '\'': {
			ax_string	tmp;
			c++;
			for( ; c; c++ ) {
				if( c == '\\' ) {
					tmp.append(c);	
					c++;
					if( !c ) {
						Log::Error( nullptr, &c.pos, "Unexpected end of character quote");
					}
					tmp.append(c);
					continue;
				}
				
				if( c == '\'' ) break;
				tmp.append(c);		
			}
			c++;
			return _setToken( token, TokenType::t_charLiteral, tmp );
		}break;
		
		case '.': {
			c++;
			if( c.isDigit() ) return _getToken_number( token, true );
			
			return _setToken( token, TokenType::t_dot, ax_txt(".") );
		}break;
	}
		
	if( c.isDigit() ) return _getToken_number( token, false );

	if( c.isAlpha() || c == '_' ) {
		ax_string	tmp;
		for( ; c; c++ ) {
			if( c.isAlpha() || c.isDigit() || c == '_' ) {
				tmp.append(c);
			}else{
				break;
			}
		}
		
		_setToken( token, TokenType::t_identifier, tmp );
		token.checkKeyword();
		return;
	}
	
	Log::Error( nullptr, &c.pos, "unknown character [{?}]", c.getChar() );
}


void Lexer::_getToken_number( Token & token, bool dot ) {
	ax_TempString	tmp;
	char prefix = 0;
	
	if( dot ) {
		tmp.append('.');
		
	}else{
		if( c == '0' ) {
			tmp.append(c); c++;
			switch( c.getChar() ) {
				case 'b':{
					prefix = c;
					tmp.append(c); c++;
					for( ; c; c++ ) {
						if( c != '_' ) {
							if( c < '0' || c > '1' ) break;
						}
						tmp.append(c);
					}
				}break;
				
				case 'o':{
					prefix = c;
					tmp.append(c); c++;
					for( ; c; c++ ) {
						if( c != '_' ) {
							if( c < '0'|| c > '7' ) break;
						}
						tmp.append(c);
					}
				}break;

				case 'x':{
					prefix = c;
					tmp.append(c); c++;
					for( ; c; c++ ) {
						if( ! c.isHex() && c != '_' ) break;
						tmp.append(c);
					}
				}break;
			}
		}
	
		if( ! prefix ) {
			for( ; c; c++ ) {
				if( ! c.isDigit() && c != '_' ) break;
				tmp.append(c);
			}
			
			if( c == '.' ) {
				tmp.append(c);	c++;
				dot = true;
			}
		}
	}
	
	if( ! prefix ) {
	// after dot
		for( ; c; c++ ) {
			if( ! c.isDigit() && c != '_' ) break;
			tmp.append(c);
		}
	
		if( c == 'e' || c == 'E' ) {
			tmp.append(c);	c++;
			
			if( c == '+' || c == '-' ) {
				tmp.append(c);	c++;
			}
			
			for( ; c; c++ ) {
				if( ! c.isDigit() && c != '_' ) break;
				tmp.append(c);
			}
		}
	}
		
	// suffix
	for( ; c; c++ ) {
		if( ! c.isAlpha() ) break;
		tmp.append(c);
	}
	
	return _setToken( token, TokenType::t_number, tmp );
}

void	Lexer::nextToken() {

	for(;;){
		bool b = getToken( token );
		if( !b ) return;
		
		if( token.is_comment() ) continue;
		break;
	}

}

}} //namespace