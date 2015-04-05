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
	propPass = false;
	
	ax_log( ax_txt("compiling... [{?}]"), sourceFile->filename );

	LexerPos pos;
	pos.reset( sourceFile );
	pos.inNode = g_compiler->metadata.root;
	
	setPos( pos );
	nextToken();
	parse_NamespaceBody();
}

void DeclarePass::parsePropPass	() {
	propPass = true;

	ax_foreach( &s, g_compiler->metadata.structList ) {
		parse_StructTypeBody( s );
	}
	
	resolvePropTypePass();
}

void DeclarePass::resolvePropTypePass() {
	ax_Array_< ax_Obj< Prop > >	list[2];
	list[0].reserve( 64 * 1024 );
	list[1].reserve( 64 * 1024 );

	auto procList = & list[0];
	auto waitList = & list[1];

	waitList->add( g_compiler->metadata.propList );
	
	for(;;) {
		ax_swap( procList, waitList );
		waitList->resize(0);
		
		if( procList->size() <= 0 ) break;
		ax_int	ok_count = 0;
		
		ax_foreach( &s, *procList ) {
			if( resolve_PropType( s ) ) {
				ok_count++;
			}else{
				waitList->add( s );
			}
		}
		
		if( ok_count == 0 ) {
			ax_foreach( &s, *procList ) {
				Log::Error( s->pos, ax_txt("unable to resolve type for property {?}"), s->name );
			}
		}
	}
}

void DeclarePass::resolveStructBaseTypes() {
	ax_Array_< ax_Obj< StructType > >	list[2];
	list[0].reserve( 64 * 1024 );
	list[1].reserve( 64 * 1024 );

	auto procList = & list[0];
	auto waitList = & list[1];

	waitList->add( g_compiler->metadata.structList );
	
	for(;;) {
		ax_swap( procList, waitList );
		waitList->resize(0);
		
		if( procList->size() <= 0 ) break;
		ax_int	ok_count = 0;
		
		ax_foreach( &s, *procList ) {
			if( resolve_StructBaseType( s ) ) {
				ok_count++;
			}else{
				waitList->add( s );
			}
		}
		
		if( ok_count == 0 ) {
			ax_foreach( &s, *procList ) {
				Log::Error( s->pos, ax_txt("unable to resolve types {?}"), s->name );
			}
		}
	}
}


bool DeclarePass::resolve_PropType( ax_Obj< Prop >	node ) {
	ax_log( ax_txt("resolve_PropType {?}"), node->name );

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

bool DeclarePass::resolve_StructBaseType( ax_Obj< StructType > node ) {

	auto n = node->baseOrInterfacePos.size();
	ax_int c = 0;
	
	if( ! node->baseType.is_null() ) {
		c++;
	}
	
	for( auto i=c; i<n; i++ ) {
		auto pos = node->baseOrInterfacePos[i];
		setPos( pos );
		
		ax_if_not_let( t, parseTypename() ) {
			return false;
		}
		
		ax_if_not_let( s, t->ax_as< StructType >() ) {
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
		node->baseType = g_compiler->metadata.type_object;
	}
	return true;
}


void DeclarePass::parse_NamespaceBody() {
	for(;;) {
		skipSemicolonOrNewlines();
		if( token.is_EOF() ) break;

		auto modifier = parseDeclarationModifier();
		
		if( token.is_class() 		) { parse_StructType	(modifier); continue; }
		if( token.is_struct() 		) { parse_StructType	(modifier); continue; }
		if( token.is_interface() 	) { parse_StructType	(modifier); continue; }
		
//		if( token.is_func() 	) { parse_func		(modifier); continue; }

		if( ! modifier.isEmpty() ) {
			Log::Error( token, ax_txt("unexpected token after declaration modifier") );
		}
	//-----
	
//		if( token.is_typealias() ) { parseTypealias	(); continue; }
		if( token.is_namespace() ) { parse_namespace();	continue; }
	
		if( token.is_curlyBracketClose() ) {
			nextToken();
			break;
		}
		
		Log::Error( token, ax_txt("unexpected token {?}"), token.str );
	}
}

void DeclarePass::parse_namespace() {
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
			parse_NamespaceBody();
			
			break;
		}
		
		Log::Error( token, ax_txt("unexpected token") );
	}
}

void DeclarePass::parse_StructType( DeclarationModifier & modifier ) {
	
	if( ! token.is_class() && ! token.is_struct() && ! token.is_interface() ) {
		Log::Error( token, ax_txt("class / struct / interface expected") );
	}
	
	if( propPass ) {
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
				
	ax_Obj< StructType >	new_node;
	switch( nodeType ) {
		case TokenType::t_interface: 	new_node = ax_new_obj( Interface,	inNode, token.pos, token.str );	break;
		case TokenType::t_struct: 		new_node = ax_new_obj( Struct,		inNode, token.pos, token.str );	break;
		case TokenType::t_class: 		new_node = ax_new_obj( Class,		inNode, token.pos, token.str );	break;

		default:
			Log::Error( token, ax_txt("class / struct / interface expected") );
	}

	if( inNode->ax_is< Namespace >() ) {
	}else if( inNode->ax_is< StructType >() ) {
		new_node->isNestedType = true;
	}else{
		Log::Error( token, ax_txt("cannot delcare sturcture type here") );
	}
	
			
	g_compiler->metadata.structList.add( new_node );
	
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
	
	parse_StructTypeBody( new_node );
	
}

void DeclarePass::parse_StructTypeBody( ax_Obj< StructType > node ) {
	setPos( node->bodyPos );

	auto scope_inNode = ax_scope_value( pos.inNode, node );
	
	for(;;) {
		skipSemicolonOrNewlines();
		if( token.is_curlyBracketClose() ) {
			nextToken();
			break;
		}
		
		auto modifier = parseDeclarationModifier();
		
		if( token.is_var()			) {	parse_Prop		(modifier); continue; }
		if( token.is_let()			) { parse_Prop		(modifier); continue; }
		if( token.is_fn()			) {	parse_Func		(modifier); continue; }
					
		if( token.is_class() 		) { parse_StructType	(modifier); continue; }
		if( token.is_struct() 		) { parse_StructType	(modifier); continue; }
		if( token.is_interface() 	) { parse_StructType	(modifier); continue; }

//		if( token.is_enum()			) { parseEnum		(modifier); continue; }

		Log::Error( token, ax_txt("unexpected token") );
	}
}

void DeclarePass::parse_Func( DeclarationModifier & modifier ) {
	if( ! token.is_fn() ) Log::Error( token, ax_txt("fn expected") );

	if( propPass ) {
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
	auto fo = ax_new_obj( FuncOverload, fn, token.pos );
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

void DeclarePass::parse_Prop( DeclarationModifier & modifier ) {
	if( ! token.is_var() && ! token.is_let() ) {
		Log::Error( token, ax_txt("let / var expected") );
	}
	
	if( ! propPass ) {
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
		
		auto new_node = ax_new_obj( Prop, inNode, token.pos, token.str, is_let );
		g_compiler->metadata.propList.add( new_node );
		nextToken();
				
		new_node->modifier = modifier;
						
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


