//
//  Parser.cpp
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "Parser.h"
#include "MetaNode.h"
#include "Log.h"
#include "Compiler.h"

namespace ax {
namespace Compile {

void Parser::setPos	( const LexerPos & pos ) {
	lexer.setPos( pos );
	lexer.getToken( token );
}

void Parser::skipTypeName	() {
	if( token.is_roundBracketOpen() ) {	//tuple
		nextToken();
		skipRoundBracket();
	}

	for(;;) {
		if( ! token.is_identifier() ) Log::Error( token, ax_txt("invalid type name") );
		nextToken();
					
		if( token.is_less() ) {
			nextToken();
			skipAngleBracket();
		}
		
		if( token.is_squareBracketOpen() ) {
			nextToken();
			skipSquareBracket();
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

void Parser::skipSemicolonOrNewlines() {
	while( token.is_newLine() || token.is_semiColon() ) {
		nextToken();
	}
}

void Parser::skipUntilEndOfLine	() {
	for(;;) {
		if( token.is_newLine_or_semiColon() ) break;
		nextToken();
	}
}

void Parser::skipUntil( TokenType t ) {
	for(;;) {
		if( token.type == t ) break;
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
		if( token.is_curlyBracketOpen() ) {
			nextToken();
			skipCurlyBracket();
			
		}else if( token.is_roundBracketOpen() ) {
			nextToken();
			skipRoundBracket();
			
		}else if( token.is_squareBracketOpen() ) {
			nextToken();
			skipSquareBracket();
		}

		if( token.is_newLine_or_semiColon() ) break;
		if( token.is_comma() ) break;
		
		nextToken();
	}
}


void Parser::skipCurlyBracket () { _skipBracket( TokenType::t_curlyBracketOpen,  TokenType::t_curlyBracketClose ); }
void Parser::skipRoundBracket () { _skipBracket( TokenType::t_roundBracketOpen,  TokenType::t_roundBracketClose ); }
void Parser::skipSquareBracket() { _skipBracket( TokenType::t_squareBracketOpen, TokenType::t_squareBracketClose ); }
void Parser::skipAngleBracket () { _skipBracket( TokenType::t_less,  			 TokenType::t_greater ); }


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

ax_NullableObj< MetaNode >	Parser::parseNode	() {
	ax_if_not_let( o, token.pos.inNode ) {
		Log::Error( nullptr, nullptr, ax_txt("error") );
	}

	for(;;) {
		if( token.is_identifier() ) {
			ax_if_not_let( p, o->getNode( token.str ) ) {
				return nullptr;
			}
			
			o = p;
			nextToken();
			
			if( token.is_dot() ) {
				nextToken();
				continue;
			}
			
			return o;
		}
		
		break;
	}
	return nullptr;
}

RType Parser::parseTypename () {
	if( token.is_roundBracketOpen() ) {	//tuple
		nextToken();
		
		ax_Array_< RType, 16 >	elements;
		
		for(;;) {
			auto t = parseTypename();
			if( t.is_null() ) break;
			
			elements.add( t );
			
			if( token.is_comma() ) {
				nextToken();
				continue;
			}
		}
		
		if( ! token.is_roundBracketClose() ) {
			Log::Error( token, ax_txt(" ')' expected ") );
		}
		nextToken();
		
		throw System::Err_Undefined(); //TODO !!
//		auto tuple = g_metadata->tupleTable.getOrAddTuple( token.pos, elements );
//		return RType::MakeTypename( tuple );
	}

	ax_if_not_let( o, parseNode() ) {
		return RType();
	}
	
	ax_if_not_let( t, o->ax_as< TypeNode >() ) {
		Log::Error( token, ax_txt("type expected") );
	}
	return RType::MakeTypename( t );
}



ax_NullableObj< ExprAST >	Parser::parseExpression() {
	ax_NullableObj<ExprAST>	expr;
	
	auto modifier = parseDeclarationModifier();
	
	switch( token.type ) {
		default: break;
//		case TokenType::t_var:	return parseExpr_LocalVar( modifier );
//		case TokenType::t_let:	return parseExpr_LocalVar( modifier );
	}
	
	if( ! modifier.isEmpty() ) {
		Log::Error( token, ax_txt("unexpected token after declaration modifier") );
	}
	
	switch( token.type ) {
		default: break;
		
//		case TokenType::t_if:			return parseExpr_If();
//		case TokenType::t_while:		return parseExpr_While();
//		case TokenType::t_do:			return parseExpr_DoWhile();
//		case TokenType::t_for:			return parseExpr_For();
//		
//		case TokenType::t_return:		return parseExpr_Return();
//		case TokenType::t_break:		return parseExpr_Break();
//		case TokenType::t_continue:		return parseExpr_Continue();
	}

	auto lhs = parseExpr_Primary();
	ax_if_let( lhs_, lhs ) {
		return 	parseExpr_BinaryOp( 0, lhs_ );
	}else{
		return nullptr;
	}
}

ax_NullableObj< ExprAST > Parser::parseExpr_Identifier() {
	if( ! token.is_identifier() ) Log::Error( token, ax_txt("identifier expected") );
	
	ax_if_not_let( inNode, token.pos.inNode ) {
		Log::Error( token, ax_txt("error") );
	}
	
	ax_if_not_let( p, inNode->getNode( token.str ) ) {
		Log::Error( token, ax_txt("unknown identifier '{0}'"), token.str );
	}
	nextToken();
	
	for(;;) {
		if( token.is_dot() ) {
			if( p->ax_is< Namespace >() || p->ax_is< TypeNode >() ) {
				nextToken();
				ax_if_not_let( p0, p->getNode( token.str ) ) {
					Log::Error( token, ax_txt("unknown identifier '{0}'"), token.str );
				}
				p = p0;
				nextToken();
				
				continue;
			}
		}else if( token.is_less() ) {
			if( p->ax_is< TypeNode >() ) {
				nextToken();
				
			//template
				Parser::skipAngleBracket();
				
				continue;
			}
		}
		
		break;
	}

	ax_if_let( prop, p->ax_as< Prop >() ) {
		return ax_new_obj( PropAST, token.pos, prop );
	}
	
	ax_if_let( type, p->ax_as< TypeNode >() ) {
		return ax_new_obj( TypeAST, token.pos, type );
	}

	Log::Error( token, ax_txt("Unknown identifier type {?}"), p->getTypeInfo().name() );
	return nullptr;
}

ax_NullableObj< ExprAST > Parser::parseExpr_NumberLiteral() {
	assert( token.is_number() );

	auto expr = ax_new_obj( NumberLiteralAST, token.pos, token.str );
	nextToken();

	return expr;
}

ax_NullableObj< ExprAST > Parser::parseExpr_StringLiteral() {
	auto expr = ax_new_obj( StringLiteralAST, token.pos, token.str );
	nextToken();
	return expr;
}

ax_NullableObj< ExprAST > Parser::parseExpr_BinaryOp( ax_int exprPrec, ax_Obj<ExprAST>& lhs ) {
	
	for(;;) {
		int tokenPrec = token.opPrec();
		if( tokenPrec < exprPrec ) {
			return lhs;
		}
	
		auto lt = lhs->returnType;
		if( lt.is_null() ) {
			return nullptr;
		}
	
	
		auto op = token.type;
		auto op_pos = token.pos;
		nextToken();
		
				
		ax_Array_< FuncParam, 32 >	params;
		
		ax_NullableObj<ExprAST>	rhs;
		
		auto closeBracket = TokenType::t_unknown;
		
		if( op == TokenType::t_roundBracketOpen ) {
			closeBracket = TokenType::t_roundBracketClose;
			op = TokenType::t_op_call;
			
		}else if( op == TokenType::t_squareBracketOpen ) {
			closeBracket = TokenType::t_squareBracketClose;
			op = TokenType::t_op_subscript;
		}
		
		if( op == TokenType::t_op_call || op == TokenType::t_op_subscript ) {
			auto e = ax_new_obj( FuncArgAST, token.pos );
			rhs = e;
			
			if( token.type == closeBracket ) {
				nextToken();
			}else{
				for(;;) {
					auto a = parseExpression();
					ax_if_let( a_, a ) {
					
						auto & fp = params.addNew();
						fp.type = a_->returnType;

						e->args.add( a_ );
						
						if( token.type == closeBracket ) {
							nextToken();
							break;
						}
						if( token.is_comma() ) {
							nextToken();
							continue;
						}
						
						Log::Error( token, ax_txt("expected ')' ") );
					}else{
						return nullptr;
					}					
				}
			}
		}else{
			rhs = parseExpr_Primary();
			
			ax_if_let( rhs_, rhs ) {
				auto & fp = params.addNew();
				fp.type = rhs_->returnType;
			}
			
		}
		
		int nextPrec = token.opPrec();
		if( tokenPrec < nextPrec ) {
			ax_if_let( rhs_, rhs ) {
				rhs = parseExpr_BinaryOp( tokenPrec + 1, rhs_ );
				if( rhs == nullptr ) return nullptr;
			}else{
				Log::Error( rhs_->pos, ax_txt("rhs is null") );
			}
		}

		ax_Array_< ax_Obj< FuncOverload >, 32 > candidate;
		
		ax_if_not_let( ltype, lt.type ) {
			Log::Error( op_pos, ax_txt("type is expected") );
		}
		
		ax_if_not_let( fn, lt.getOperatorFunc( op, token.pos ) ) {
			Log::Error( op_pos, ax_txt("operator '{?}' function not found from {?}"), op, lt );
		}
		
		ax_if_not_let( fo, fn->getOverload( candidate, params ) ) {
			Log::Error( op_pos, ax_txt("function overload not match: {?} {?} ({?})'\n ----------\n {?} \n----------\n"), fn, op, params, fn->overloads );
		}

		ax_if_let( rhs_, rhs ) {
			lhs = ax_new_obj( BinaryAST, op_pos, fo, lhs, rhs_, true );
		}
	}
	
}

ax_NullableObj< ExprAST >	Parser::parseExpr_Primary() {
	ax_NullableObj< ExprAST >		expr;
	TokenType	prefixOp = TokenType::t_unknown;
	LexerPos	prefixPos;
	
//----- prefix -----
	switch( token.type ) {
		default: break;
		case TokenType::t_add:
		case TokenType::t_sub:
		case TokenType::t_add2:
		case TokenType::t_sub2:
			prefixPos = token.pos;
			prefixOp  = token.type;
			
			nextToken();
			break;
	}
	
//-------------
	switch( token.type ) {
		default: break;
		case TokenType::t_identifier: 		expr = parseExpr_Identifier(); 		break;
		case TokenType::t_number:			expr = parseExpr_NumberLiteral(); 	break;
		case TokenType::t_stringLiteral:	expr = parseExpr_StringLiteral(); 	break;
		/*
		case TokenType::t_squareBracketOpen: { // Array or Dict const
			nextToken();
			expr = parseExpr_ArrayOrDictConst();
		}break;
		
		case TokenType::t_roundBracketOpen:{ // Parenthesis or Tuple
			nextToken();
			expr = parseExpression();
			
			//Tuple
			if( token.is_comma() ) {
				nextToken();
				
				ax_if_let( expr_, expr ) {
					expr = parseExpr_Tuple( expr_ );
				}else{
					Log::Error( token, ax_txt("error tuple") );
				}
			}
			
			if( ! token.is_roundBracketClose() ) Log::Error( token, ax_txt(") expected") );
			
			nextToken();
		}break;
		*/
	}
	
	if( prefixOp != TokenType::t_unknown ) {
		ax_if_not_let( expr_, expr ) {
			Log::Error( token, ax_txt("error prefix {?}"), prefixOp );
		}else{
			auto & t = expr_->returnType;
			
			ax_if_not_let( nodeType, t.type ) {
				Log::Error( nullptr, &expr_->pos, ax_txt("no 'prefix func {?}' "), prefixOp, t );
			}
			
			ax_if_not_let( opFunc, nodeType->getPrefixOperatorFunc( prefixOp ) ) {
				Log::Error( nullptr, &expr_->pos, ax_txt("no 'prefix func {?}' "), prefixOp, t );
			}

			ax_Array_< FuncParam, 32 >				params;
			ax_Array_< ax_Obj< FuncOverload >, 16 >	candidate;
			ax_if_not_let( fo, opFunc->getOverload( candidate, params ) ) {
				Log::Error( nullptr, &expr_->pos, ax_txt("no 'prefix func {?} ' overload"), prefixOp );
			}

			auto prefix = ax_new_obj( PrefixAST, prefixPos, fo, expr_ );
			
			expr = prefix;
		}

	}

//--- postfix ----
	switch( token.type ) {
		default: break;
		case TokenType::t_add2:
		case TokenType::t_sub2: {
			ax_if_not_let( expr_, expr ) {
				Log::Error( token, ax_txt("invalid expr") );
			}
			
			auto op = token.type;
			auto &t = expr_->returnType;
			
			ax_if_not_let( nodeType, t.type ) {
				Log::Error( nullptr, &expr_->pos, ax_txt("no 'postfix func {?}' "), op, t );
			}
			
			ax_if_not_let( opFunc, nodeType->getOperatorFunc( op ) ) {
				Log::Error( nullptr, &expr_->pos, ax_txt("no 'postfix func {?}' "), op, t );
			}

			ax_Array_< FuncParam, 32 >					params;
			ax_Array_< ax_Obj< FuncOverload >, 16 >		candidate;
			ax_if_not_let( fo, opFunc->getOverload( candidate, params ) ) {
				Log::Error( nullptr, &expr_->pos, ax_txt("no 'postfix func {?} ' overload"), op );
			}

			expr = ax_new_obj( PostfixAST, token.pos, fo, expr_ );
			nextToken();
		}break;
	}
	
	return expr;
}

}} //namespace