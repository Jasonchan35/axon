//
//  Token.cpp
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "Token.h"

namespace ax {
namespace Compile {


class TokenTable {
public:
	TokenTable() {
		inited = false;
	}
	
	void init() {
		if( inited ) return;
		inited = true;
		
	//init keywords
		#define Token_KEYWORD(T) keywords.add( ax_txt(#T), TokenType::t_##T );
			Token_KEYWORD_LIST
		#undef Token_KEYWORD
		/*
		ax_log("-- Keyword Table --");
		for( auto & a : keywords ) {
			ax_log_var( a );
		}
		ax_log("-- End of Keyword table --");
		*/
	}
	
	bool	inited;
	
	ax_Dict< ax_string, TokenType >	keywords;
};

TokenTable table;

int Token_opPrec( TokenType t ) {
	switch( t ){
		case TokenType::t_assign:
			return 1000;
	
		case TokenType::t_not:
		case TokenType::t_and:
		case TokenType::t_or:
		case TokenType::t_xor:
			return 1100;

		case TokenType::t_equal:
		case TokenType::t_notEqual:
		case TokenType::t_less:
		case TokenType::t_lessEqual:
		case TokenType::t_greater:
		case TokenType::t_greaterEqual:
			return 2000;
	
		case TokenType::t_add:
		case TokenType::t_sub:
			return 3000;

		case TokenType::t_mul:
		case TokenType::t_div:
		case TokenType::t_mod:
			return 4000;
			
		case TokenType::t_roundBracketOpen:
		case TokenType::t_squareBracketOpen:
			return 8000;

		case TokenType::t_dot:
			return 9000;
			
			
		default: return -1;
	}
}

Token::Token() {
	type = TokenType::t_unknown;
}

void ax_to_string_req( ax::System::ToStringReq & req, const TokenType & v ) {
	req << ax_to_string( v );
}

ax_string ax_to_string( TokenType type ) {
	#define Token_TYPE(T,S)  case TokenType::t_##T: return ax_txt(S);
//	#define Token_TYPE(T,S)  case TokenType::t_##T: return ax_txt(#T);
	#define Token_KEYWORD(T) case TokenType::t_##T: return ax_txt(#T);
	
	if( type == TokenType::t_unknown ) return ax_txt("unknown");
	
	switch(type) {
		Token_TYPE_LIST
		Token_KEYWORD_LIST
		default: break;
	}
	#undef Token_TYPE
	#undef Token_KEYWORD
	
	return ax_txt("unknown");
}

ax_string	Token::typeName() const {
	return ax_to_string( type );
}

bool Token::checkKeyword() {
	if( type != TokenType::t_identifier ) return false;
	
	table.init();
	
	ax_if_let( v, table.keywords.tryGetValue( str ) ) {
		type = v;
		return true;
	}else{
		return false;
	}
}


}} //namespace

