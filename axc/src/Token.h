//
//  Token.h
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__Token__
#define __axc__Token__

#include "define.h"
#include "LexerPos.h"

namespace ax {
namespace Compile {

#define	Token_TYPE_LIST 							\
	Token_TYPE(EOF,					"EOF" )			\
	Token_TYPE(newLine,				"<newline>")	\
	Token_TYPE(identifier,			"<iden>" )		\
	Token_TYPE(number,				"<num>"  )		\
	Token_TYPE(stringLiteral,		"<str>"  )		\
	Token_TYPE(charLiteral,			"<char>" )		\
	Token_TYPE(comment,				"<comment>")	\
	\
	Token_TYPE(op_call,				"<call>")		\
	Token_TYPE(op_subscript,		"<subscript>")	\
	\
	Token_TYPE(equal,				"==" ) \
	Token_TYPE(notEqual,			"!=" ) \
	Token_TYPE(notEqual3,			"!==") \
	Token_TYPE(equal3,				"===") \
	\
	Token_TYPE(greater,				">"  ) \
	Token_TYPE(greater2,			">>" ) \
	Token_TYPE(greaterEqual,		">=" ) \
	\
	Token_TYPE(less,				"<"  ) \
	Token_TYPE(less2,				"<<"  ) \
	Token_TYPE(lessEqual,			"<=" ) \
	\
	Token_TYPE(lessGreater,			"<>" ) \
	\
	Token_TYPE(add,					"+"  ) \
	Token_TYPE(sub,					"-"  ) \
	Token_TYPE(mul,					"*"  ) \
	Token_TYPE(div,					"/"  ) \
	Token_TYPE(mod,					"%"  ) \
	\
	Token_TYPE(assign, 				"="  ) \
	Token_TYPE(colonAssign,			":=" ) \
	Token_TYPE(addAssign,			"+=" ) \
	Token_TYPE(subAssign,			"-=" ) \
	Token_TYPE(mulAssign,			"*=" ) \
	Token_TYPE(divAssign,			"/=" ) \
	Token_TYPE(modAssign,			"%=" ) \
	\
	Token_TYPE(add2, 				"++" ) \
	Token_TYPE(sub2, 				"--" ) \
	Token_TYPE(mul2, 				"**" ) \
	Token_TYPE(mod2, 				"%%" ) \
	\
	Token_TYPE(roundBracketOpen,	"("  ) \
	Token_TYPE(roundBracketClose,	")"  ) \
	Token_TYPE(curlyBracketOpen,	"{"  ) \
	Token_TYPE(curlyBracketClose,	"}"  ) \
	Token_TYPE(squareBracketOpen,	"["  ) \
	Token_TYPE(squareBracketClose,	"]"  ) \
	\
	Token_TYPE(exclamation,			"!"  ) \
	Token_TYPE(question,			"?"  ) \
	Token_TYPE(exclamationDot,		"!." ) \
	Token_TYPE(questionDot,			"?." ) \
	\
	Token_TYPE(comma,				","  ) \
	Token_TYPE(colon,				":"  ) \
	Token_TYPE(colon2,				"::" ) \
	Token_TYPE(semiColon,			";"  ) \
	Token_TYPE(dot,					"."  ) \
	Token_TYPE(at,					"@"  ) \
	Token_TYPE(hash,				"#"  ) \
	Token_TYPE(dollar,				"$"  ) \
	Token_TYPE(backMark,			"`"  ) \
	Token_TYPE(backSlash,			"\\" ) \
//----

#define	Token_KEYWORD_LIST 		\
	Token_KEYWORD(using)		\
	\
	Token_KEYWORD(mutating)		\
	Token_KEYWORD(nonmutating)	\
	\
	Token_KEYWORD(virtual)		\
	Token_KEYWORD(override)		\
	\
	Token_KEYWORD(unowned)		\
	Token_KEYWORD(weak)			\
	\
	Token_KEYWORD(static)		\
	Token_KEYWORD(extern)		\
	\
	Token_KEYWORD(namespace)	\
	Token_KEYWORD(typealias)	\
	Token_KEYWORD(typename)		\
	Token_KEYWORD(class)		\
	Token_KEYWORD(struct)		\
	Token_KEYWORD(interface)	\
	Token_KEYWORD(enum)			\
	\
	Token_KEYWORD(is)			\
	Token_KEYWORD(as)			\
	\
	Token_KEYWORD(not)			\
	Token_KEYWORD(and)			\
	Token_KEYWORD(or)			\
	Token_KEYWORD(xor)			\
	\
	Token_KEYWORD(var)			\
	Token_KEYWORD(let)			\
	Token_KEYWORD(func)			\
	\
	Token_KEYWORD(this)			\
	Token_KEYWORD(self)			\
	\
	Token_KEYWORD(if)			\
	Token_KEYWORD(else)			\
	Token_KEYWORD(while)		\
	Token_KEYWORD(do)			\
	Token_KEYWORD(for)			\
	Token_KEYWORD(foreach)		\
	Token_KEYWORD(in)			\
	Token_KEYWORD(return)		\
	Token_KEYWORD(switch)		\
	Token_KEYWORD(break)		\
	Token_KEYWORD(continue)		\
	Token_KEYWORD(fallthrough)	\
	\
	Token_KEYWORD(throw)		\
	Token_KEYWORD(try)			\
	Token_KEYWORD(catch)		\
//----

#define	Token_TYPE(T,S)		t_##T,
#define	Token_KEYWORD(T) 	t_##T,
enum class TokenType {
	t_unknown = 0,
	
	__keyword_start__ = 1000,
	Token_KEYWORD_LIST
	__keyword_end__,
	
	__type_start__ = 2000,
	Token_TYPE_LIST
	__type_end__,
	
	COUNT,
};
#undef	Token_TYPE
#undef	Token_KEYWORD

int Token_opPrec( TokenType t ); //operator precedence

class Token {
public:
	Token();

	ax_string typeName() const;

	operator	bool	() const	{ return type != TokenType::t_EOF; }

	#define	Token_TYPE(T,S)		bool is_##T() const { return type == TokenType::t_##T; }
	#define	Token_KEYWORD(T) 	bool is_##T() const { return type == TokenType::t_##T; }
		Token_KEYWORD_LIST
		Token_TYPE_LIST
	#undef	Token_TYPE
	#undef	Token_KEYWORD

	bool	is_newLine_or_semiColon()	{ return is_newLine() || is_semiColon(); }
	bool	is_operator	() { return Token_opPrec(type) > 0; }

	TokenType			type;
	ax_MutString_<>		str;
	LexerPos			pos;
	int					opPrec()	{ return Token_opPrec(type); }
	
	bool		checkKeyword();
	
	void	copy( const Token & rhs );
	
	void	OnStringReq( ax_ToStringReq & req ) const {
		req << ax_txt("Token[ ") << typeName() << ax_txt(", \"") << str << ax_txt("\" ] ");
	}
	
	static ax_string TokenTypeToString( TokenType t );
};


}} //namespace


#endif /* defined(__axc__Token__) */
