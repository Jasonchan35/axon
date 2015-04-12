//
//  DeclarePass.cpp
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "DeclarePass.h"
#include "Compiler.h"

namespace ax {
namespace Compile {

void DeclarePass::parseFile( ax_Obj< SourceFile > sourceFile ) {
	inPropPass = false;
	
	ax_log( ax_txt("compiling... [{?}]"), sourceFile->filename );

	Location pos;
	pos.reset( sourceFile );
	pos.inNode = g_metadata->root;
	
	setPos( pos );
	parseNamespaceBody();
}

void DeclarePass::run2ndPass() {
	resolveStructTypePass();
	
	parsePropPass();
	resolvePropPass();
	
	resolveFuncParamPass();
}

void DeclarePass::parsePropPass	() {
	inPropPass = true;

	ax_foreach( &s, g_metadata->structList ) {
		if( s->buildin ) continue;
		parseStructTypeBody( s );
	}
	
	inPropPass = false;
}

void DeclarePass::resolveFuncParamPass () {
	ax_foreach( &fo, g_metadata->funcOverloadList ) {
		if( fo->buildin ) continue;
		resolveFuncParam( fo );
	}
}

void DeclarePass::resolveFuncParam( ax_Obj< FuncOverload > fo ) {
	setPos( fo->paramPos );
	
	for(;;) {
		if( token.is_roundBracketClose() ) {
			nextToken();
			break;
		}

		if( ! token.is_identifier() ) {
			Log::Error( token, ax_txt("parameter name expected when resolving\n  {?}"), fo );
		}
	
		nextToken();

		Type		paramType;
		Location	typePos;
		
		ax_NullableObj< AST >	defaultValueExpr;
		
		if( token.is_identifier() ) {
			paramType = parseTypename();
			typePos   = token.pos;
			
			if( paramType.is_null() ) {
				Log::Error( token, ax_txt("parameter type expected") );
			}
		}
		
		if( token.is_assign() ) {
			nextToken();
			ax_if_not_let( expr, parseExpression() ) {
				Log::Error( token, ax_txt("error expression") );
			}
			defaultValueExpr = expr;
			paramType = expr->returnType;
		}

		auto & param = fo->addParam( token.str, token.pos, paramType, typePos );
		param.defaultValueExpr = defaultValueExpr;
				
		if( token.is_comma() ) {
			nextToken();
			continue;
		}
		
		if( ! token.is_roundBracketClose() ) {
			Log::Error( token, ax_txt("unexpected tokenr") );
		}
	}
	
	
	// function return type
	if( token.is_identifier() ) {
		fo->returnType = parseTypename();
	}else{
		fo->returnType = Type::MakeValue( g_metadata->type_void, false );
	}
	
	if( ! token.is_curlyBracketOpen() ) {
		Log::Error( token, ax_txt("function body { expected") );
	}
}

void DeclarePass::resolvePropPass() {
	ax_Array_< ax_Obj< Prop > >	list[2];
	list[0].reserve( 64 * 1024 );
	list[1].reserve( 64 * 1024 );

	auto procList = & list[0];
	auto waitList = & list[1];

	waitList->addRange( g_metadata->propList );
	
	for(;;) {
		ax_swap( procList, waitList );
		waitList->resize(0);
		
		if( procList->size() <= 0 ) break;
		ax_int	ok_count = 0;
		
		ax_foreach( &s, *procList ) {
			if( resolveProp( s ) ) {
				ok_count++;
			}else{
				waitList->add( s );
			}
		}
		
		if( ok_count == 0 ) {
			ax_foreach( &s, *procList ) {
				Log::Error( s->pos, ax_txt("unable to resolve type for property {?}"), s->name() );
			}
		}
	}
}

void DeclarePass::resolveStructTypePass() {
	ax_Array_< ax_Obj< CompositeTypeSpec > >	list[2];
	list[0].reserve( 64 * 1024 );
	list[1].reserve( 64 * 1024 );

	auto procList = & list[0];
	auto waitList = & list[1];

	waitList->addRange( g_metadata->structList );
	
	for(;;) {
		ax_swap( procList, waitList );
		waitList->resize(0);
		
		if( procList->size() <= 0 ) break;
		ax_int	ok_count = 0;
		
		ax_foreach( &s, *procList ) {
			if( resolveStructType( s ) ) {
				ok_count++;
			}else{
				waitList->add( s );
			}
		}
		
		if( ok_count == 0 ) {
			ax_foreach( &s, *procList ) {
				Log::Error( s->pos, ax_txt("unable to resolve types {?}"), s->name() );
			}
		}
	}
}


bool DeclarePass::resolveProp( ax_Obj< Prop >	node ) {

	if( node->type.is_null() ) {
		if( node->typePos.valid ) {
			setPos( node->typePos );
			node->type = parseTypename();
		}
	}
	
	if( node->initExprPos.valid ) {
		setPos( node->initExprPos );
		
		ax_if_let( expr, parseExpression() ) {
			node->initExpr = expr;
			
			if( ! node->type.is_null() ) {
				if( node->type.canAssignFrom( expr->returnType ) ) {
					Log::Error( node->initExprPos, ax_txt("'{?}' type mis-match '{?}' expected"), expr->returnType, node->type );
				}
			}
			node->type = expr->returnType;
		}
	}
	
	if( node->type.is_null() ) {
		return false;
	}
		
	return true;
}

bool DeclarePass::resolveStructType( ax_Obj< CompositeTypeSpec > node ) {

	auto n = node->baseOrInterfacePos.size();
	ax_int c = 0;
	
	if( ! node->baseType.is_null() ) {
		c++;
	}
	
	for( auto i=c; i<n; i++ ) {
		auto pos = node->baseOrInterfacePos[i];
		setPos( pos );
		
		auto rt = parseTypename();
		ax_if_not_let( t, rt.type ) {
			return false;
		}
		
		ax_if_not_let( s, t->ax_as< CompositeTypeSpec >() ) {
			Log::Error( pos, ax_txt("interface / class / struct expected") );
		}

		ax_if_let( f, t->ax_as< Interface >() ) {
			node->interfaces.add( f );
			continue;
		}
		
		if( i != 0 ) {
			Log::Error( pos, ax_txt("cannot have multiple base class") );
		}
		node->baseType = s;
	}
	
	if( node->baseType.is_null() && node->ax_is< Class >() ) {
		node->baseType = g_metadata->type_object;
	}
	
	{
		ax_if_not_let( ctor, node->getFunc( k_ctor_name ) ) {
			auto fn = node->getOrAddFunc( k_ctor_name );
			auto fo = fn->addOverload( node->pos );
			fo->pos		= node->pos;
			fo->buildin = true;
			fo->returnType = Type::MakeValue( node, false );
		}
	}
	
	return true;
}


void DeclarePass::parseNamespaceBody() {
	for(;;) {
		skipSemicolonOrNewlines();
		if( token.is_EOF() ) break;

		auto modifier = parseDeclarationModifier();
		
		if( token.is_class() 		) { parseStructType	(modifier); continue; }
		if( token.is_struct() 		) { parseStructType	(modifier); continue; }
		if( token.is_interface() 	) { parseStructType	(modifier); continue; }
		
//		if( token.is_func() 	) { parse_func		(modifier); continue; }

		if( ! modifier.isEmpty() ) {
			Log::Error( token, ax_txt("unexpected token after declaration modifier") );
		}
	//-----
	
//		if( token.is_typealias() ) { parseTypealias	(); continue; }
		if( token.is_namespace() ) { parseNamespace();	continue; }
	
		if( token.is_curlyBracketClose() ) {
			nextToken();
			break;
		}
		
		Log::Error( token, ax_txt("unexpected token {?}"), token.str );
	}
}

void DeclarePass::parseNamespace() {
	assert( token.is_namespace() );
	nextToken();

	auto scope_inNode = ax_scope_value( pos.inNode );
	
	ax_if_not_let( ns0, pos.inNode ) {
		throw System::Err_Undefined();
	}
	
	ax_if_not_let( ns, ns0->getUpperByType< Namespace >() ) {
		throw System::Err_Undefined();
	}
	
	for(;;) {
		auto scope_ns = ax_scope_value( ns );		
		for(;;) {
			if( ! token.is_identifier() ) {
				Log::Error( token, ax_txt("namespace name expected") );
			}

			ns = ns->getOrAddNamespace( token.str, token.pos );
			
			nextToken();
			
			if( token.is_dot() ) {
				nextToken();
				continue;
			}			
			pos.inNode = ns;
			
			break;
		}
		
		if( token.is_curlyBracketOpen() ) {
			nextToken();
			parseNamespaceBody();
			
			break;
		}
		
		Log::Error( token, ax_txt("unexpected token") );
	}
}

void DeclarePass::parseStructType( Modifier & modifier ) {
	
	if( ! token.is_class() && ! token.is_struct() && ! token.is_interface() ) {
		Log::Error( token, ax_txt("class / struct / interface expected") );
	}
	
	if( inPropPass ) {
		skipUntil( TokenType::t_curlyBracketOpen );
		nextToken();
		skipCurlyBracket();
		return;
	}

	ax_if_not_let( inNode, pos.inNode ) {
		throw System::Err_Undefined();
	}
	
	auto nodeType = token.type;
				
	nextToken();
	if( ! token.is_identifier() ) Log::Error( token, ax_txt("type name expected") );
				
	ax_Obj< CompositeTypeSpec >	new_node;
	switch( nodeType ) {
		case TokenType::t_interface: 	new_node = ax_new_obj( Interface,	inNode, token.str, token.pos );	break;
		case TokenType::t_struct: 		new_node = ax_new_obj( Struct,		inNode, token.str, token.pos );	break;
		case TokenType::t_class: 		new_node = ax_new_obj( Class,		inNode, token.str, token.pos );	break;

		default:
			Log::Error( token, ax_txt("class / struct / interface expected") );
	}	

	if( inNode->ax_is< Namespace >() ) {
	}else if( inNode->ax_is< CompositeTypeSpec >() ) {
		new_node->isNestedType = true;
	}else{
		Log::Error( token, ax_txt("cannot delcare sturcture type here") );
	}
		
	nextToken();
	
	new_node->modifier = modifier;
	
	if( token.is_colon() ) {
		nextToken();
		
		for(;;) {
			if( token.is_identifier() ) {
				new_node->baseOrInterfacePos.add( token.pos );
				skipTypeName();
			}
			
			if( token.is_comma() ) {
				nextToken();
				continue;
			}
			
			break;
		}
	}

	if( ! token.is_curlyBracketOpen() ) {
		Log::Error( token, ax_txt("{{ is missing") );
	}
	nextToken();
	
	new_node->bodyPos = token.pos;
	
	parseStructTypeBody( new_node );
	
}

void DeclarePass::parseStructTypeBody( ax_Obj< CompositeTypeSpec > node ) {
	setPos( node->bodyPos );

	auto scope_inNode = ax_scope_value( pos.inNode, node );
	
	for(;;) {
		skipSemicolonOrNewlines();
		if( token.is_curlyBracketClose() ) {
			nextToken();
			break;
		}
		
		auto modifier = parseDeclarationModifier();
		
		if( token.is_var()			) {	parseProp		(modifier); continue; }
		if( token.is_let()			) { parseProp		(modifier); continue; }
		if( token.is_fn()			) {	parseFunc		(modifier); continue; }
					
		if( token.is_class() 		) { parseStructType	(modifier); continue; }
		if( token.is_struct() 		) { parseStructType	(modifier); continue; }
		if( token.is_interface() 	) { parseStructType	(modifier); continue; }

//		if( token.is_enum()			) { parseEnum		(modifier); continue; }

		Log::Error( token, ax_txt("unexpected token") );
	}
}

void DeclarePass::parseFunc( Modifier & modifier ) {
	if( ! token.is_fn() ) Log::Error( token, ax_txt("fn expected") );

	if( inPropPass ) {
		skipUntil( TokenType::t_curlyBracketOpen );
		nextToken();
		skipCurlyBracket();
		return;
	}

	nextToken();

	ax_if_not_let( inNode, pos.inNode ) {
		throw System::Err_Undefined();
	}
		
	if( ! inNode->ax_is< Class >() && ! inNode->ax_is< Struct >() && ! inNode->ax_is< Interface >() ) {
		Log::Error( token, ax_txt("cannot delcare function here") );
	}

	if( ! token.is_identifier() ) {
		Log::Error( token, ax_txt("function name expected") );
	}
	
	auto fn = inNode->getOrAddFunc( token.str );
	auto fo = fn->addOverload( token.pos );
	nextToken();

	if( ! token.is_roundBracketOpen() ) {
		Log::Error( token, ax_txt("( is expected") );
	}
	nextToken();
	
	fo->modifier = modifier;
	fo->paramPos = token.pos;
	
	skipRoundBracket();
	
	if( token.is_identifier() ) {
		fo->returnTypePos = token.pos;
		skipTypeName();
	}
	
	if( ! token.is_curlyBracketOpen() ) {
		if( ! modifier.b_extern ) {
			Log::Error( token, ax_txt("{{ expected") );
		}
	}else{
		fo->bodyPos = token.pos;
		nextToken();
		skipCurlyBracket();
	}
}

void DeclarePass::parseProp( Modifier & modifier ) {
	if( ! token.is_var() && ! token.is_let() ) {
		Log::Error( token, ax_txt("let / var expected") );
	}
	
	if( ! inPropPass ) {
		skipUntilEndOfLine();
		nextToken();
		return;
	}
	
	bool is_let = token.is_let();

	ax_if_not_let( inNode, pos.inNode ) {
		throw System::Err_Undefined();
	}
	
	if( ! inNode->ax_is< Class >() && ! inNode->ax_is< Struct >() ) {
		Log::Error( token, ax_txt("cannot delcare property here") );
	}
	
	nextToken();

	for(;;) {
		if( ! token.is_identifier() ) Log::Error( token, ax_txt("var name expected") );
		
		auto new_node = ax_new_obj( Prop, inNode, token.str, token.pos );
		new_node->is_let = is_let;
		nextToken();
				
//		new_node->modifier = modifier;
						
		if( token.is_identifier() ) {
			new_node->typePos = token.pos;
			skipTypeName();
		}
		
		if( token.is_assign() ) {
			nextToken();
//			new_node->initExpr = parseExpression();

			new_node->initExprPos = token.pos;
			skipExpression();
		}
				
//		if( ! new_node->dataTypePos.valid && ! new_node->initExprPos.valid ) {
//			Log::Error( token, ax_txt("unknown type for property '{?}'"), new_node->name );
//		}
	
		if( token.is_comma() ) {
			nextToken();
			continue;
		}
			
		break;
	}
				
	if( ! token.is_newLine_or_semiColon() ) {
		Log::Error( token, ax_txt("unexpected token") );
	}
	nextToken();
}

}} //namespace


