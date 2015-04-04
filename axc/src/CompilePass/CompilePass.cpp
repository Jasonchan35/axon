//
//  CompilePass.cpp
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "CompilePass.h"

namespace ax {
namespace Compile {


void CompilePass::nextToken() { parser.nextToken(); }


ax_NullableObj< MetaNode >	CompilePass::parseNode	() {
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

ax_NullableObj< TypeNode > CompilePass::parseTypename	() {
	if( token.is_roundBracketOpen() ) {	//tuple
		nextToken();
		
		ax_Array_< ax_Obj<TypeNode>, 16 >	elements;
		
		for(;;) {
			ax_if_not_let( e, parseTypename() ) {
				break;
			}
			elements.add(e);
			
			if( token.is_comma() ) {
				nextToken();
				continue;
			}
		}
		
		if( ! token.is_roundBracketClose() ) {
			Log::Error( token, ax_txt(" ')' expected ") );
		}
		nextToken();
		
		return g_compiler->metadata.tupleTable.getOrAddTuple( token.pos, elements );
	}

	ax_if_not_let( o, parseNode() ) {
		return nullptr;
	}
	
	ax_if_not_let( t, o->ax_as< TypeNode >() ) {
		Log::Error( token, ax_txt("type expected") );
	}
	return t;
}



ax_NullableObj< ExprAST >	CompilePass::parseExpression() {
	ax_NullableObj<ExprAST>	expr;
	
	auto modifier = parser.parseDeclarationModifier();
	
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

ax_NullableObj< ExprAST > CompilePass::parseExpr_Identifier() {
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
				parser.skipAngleBracket();
				
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

ax_NullableObj< ExprAST > CompilePass::parseExpr_Number() {
	assert( token.is_number() );

	auto expr = ax_new_obj( NumberLiteralAST, token.pos, token.str );
	nextToken();

	return expr;
}

ax_NullableObj< ExprAST > CompilePass::parseExpr_StringLiteral() {
	auto expr = ax_new_obj( StringLiteralAST, token.pos, token.str );
	nextToken();
	return expr;
}

ax_NullableObj< ExprAST > CompilePass::parseExpr_BinaryOp( ax_int exprPrec, ax_Obj<ExprAST>& lhs ) {
	
	for(;;) {
		int tokenPrec = token.opPrec();
		if( tokenPrec < exprPrec ) {
			return lhs;
		}
	
		ax_if_not_let( lt, lhs->returnType.type ) {
			Log::Error( lhs->pos, ax_txt("unknown type from lhs") );
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
			op = TokenType::t_op_call;
		}
		
		if( op == TokenType::t_op_call ) {
			auto e = ax_new_obj( FuncArgAST, token.pos );
			rhs = e;
			
			if( token.type == closeBracket ) {
				nextToken();
			}else{
				for(;;) {
					auto a = parseExpression();
					ax_if_let( a_, a ) {
						auto & fp = params.addNew();
						fp.rtype = a_->returnType;

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
				fp.rtype = rhs_->returnType;
			}
			
		}
		
		int nextPrec = token.opPrec();
		if( tokenPrec < nextPrec ) {
			ax_if_let( rhs_, rhs ) {
				rhs = parseExpr_BinaryOp( tokenPrec + 1, rhs_ );
				if( rhs == nullptr ) return nullptr;
			}else{
				Log::Error( pos, ax_txt("rhs is null") );
			}
		}

		ax_Array_< ax_Obj< FuncOverload >, 32 > candidate;
		
		ax_if_not_let( fn, lt->getOperatorFunc( op ) ) {
			Log::Error( op_pos, ax_txt("operator '{?}' function not found"), op );
		}
		
		ax_if_not_let( fo, fn->getOverload( candidate, params ) ) {
			Log::Error( op_pos, ax_txt("no '{?}.func {?} ({?})' overload"), lt->name, op, params );
		}

		ax_if_let( rhs_, rhs ) {
			lhs = ax_new_obj( BinaryAST, op_pos, fo, lhs, rhs_, true );
		}
	}
	
}

ax_NullableObj< ExprAST >	CompilePass::parseExpr_Primary() {
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
		case TokenType::t_number:			expr = parseExpr_Number(); 			break;
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
	
	return expr;}

}} //namespace