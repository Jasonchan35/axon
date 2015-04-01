//
//  DeclarePass.cpp
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "DeclarePass.h"

namespace ax {
namespace Compile {

void DeclarePass::parseFile( ax_Obj< SourceFile > sourceFile ) {
	propPass = false;
	
	ax_log( "compiling... [{?}]", sourceFile->filename );
	
	parser.reset( sourceFile );
	parser.lexer.c.pos.inNode = g_compiler->metadata.root;
	
	parser.nextToken();
	parse_NamespaceBody();
}

void DeclarePass::parsePropPass	() {
	propPass = true;

	ax_foreach( &s, g_compiler->metadata.structList ) {
		parse_StructBody( s );
	}
	
	resolvePropTypePass();
}

void DeclarePass::resolvePropTypePass() {
	ax_Array_< ax_Obj< PropNode > >	list[2];
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
				Log::Error( s->pos, ax_txt("unable to resolve types {?}"), s->name );
			}
		}
	}
}

void DeclarePass::resolveStructBaseTypes() {
	ax_Array_< ax_Obj< StructureType > >	list[2];
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


void DeclarePass::parse_NamespaceBody() {
	for(;;) {
		parser.skipNewLines();
		if( token.is_EOF() ) break;

		auto modifier = parser.parseDeclarationModifier();
		
		if( token.is_class() 		) { parse_StructNode	(modifier); continue; }
		if( token.is_struct() 		) { parse_StructNode	(modifier); continue; }
		if( token.is_interface() 	) { parse_StructNode	(modifier); continue; }
		
//		if( token.is_func() 	) { parse_func		(modifier); continue; }

		if( ! modifier.isEmpty() ) {
			Log::Error( token, ax_txt("unexpected token after declaration modifier") );
		}
	//-----
	
//		if( token.is_typealias() ) { parseTypealias	(); continue; }
		if( token.is_namespace() ) { parse_namespace();	continue; }
	
		if( token.is_curlyBracketClose() ) {
			parser.nextToken();
			break;
		}
		
		Log::Error( token, ax_txt("unexpected token {?}"), token.str );
	}
}

void DeclarePass::parse_namespace() {
	assert( token.is_namespace() );
	parser.nextToken();

	auto scope_inNode = ax_scope_value( pos.inNode );
	
	ax_if_not_let( ns0, pos.inNode ) {
		throw System::Err_Undefined();
	}
	
	ax_if_not_let( ns, ns0->getUpperByType< NamespaceNode >() ) {
		throw System::Err_Undefined();
	}
	
	for(;;) {
		auto scope_ns = ax_scope_value( ns );		
		for(;;) {
			if( ! token.is_identifier() ) {
				Log::Error( token, ax_txt("namespace name expected") );
			}

			ns = ns->getOrAddNamespace( token.str, token.pos );
			
			parser.nextToken();
			
			if( token.is_dot() ) {
				parser.nextToken();
				continue;
			}			
			pos.inNode = ns;
			
			break;
		}
		
		if( token.is_curlyBracketOpen() ) {
			parser.nextToken();
			parse_NamespaceBody();
			
			break;
		}
		
		Log::Error( token, ax_txt("unexpected token") );
	}
}

void DeclarePass::parse_StructNode( DeclarationModifier & modifier ) {
	
	if( ! token.is_class() && ! token.is_struct() && ! token.is_interface() ) {
		Log::Error( token, ax_txt("class / struct / interface expected") );
	}
	
	if( propPass ) {
		parser.skipUntil( TokenType::t_curlyBracketOpen );
		nextToken();
		parser.skipCurlyBracket();
		return;
	}

	ax_if_not_let( inNode, pos.inNode ) {
		throw System::Err_Undefined();
	}
	
	auto nodeType = token.type;
				
	nextToken();
	if( ! token.is_identifier() ) Log::Error( token, ax_txt("type name expected") );
				
	ax_Obj< StructureType >	new_node;
	switch( nodeType ) {
		case TokenType::t_interface: 	new_node = ax_new_obj( InterfaceNode,	inNode, token.pos, token.str );	break;
		case TokenType::t_struct: 		new_node = ax_new_obj( StructNode,		inNode, token.pos, token.str );	break;
		case TokenType::t_class: 		new_node = ax_new_obj( ClassNode,		inNode, token.pos, token.str );	break;

		default:
			Log::Error( token, ax_txt("class / struct / interface expected") );
	}

	if( inNode->ax_is< NamespaceNode >() ) {
	}else if( inNode->ax_is< StructureType >() ) {
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
				parser.skipTypeName();
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
	
	parse_StructBody( new_node );
	
}

void DeclarePass::parse_StructBody( ax_Obj< StructureType > structNode ) {
	parser.setPos( structNode->bodyPos );

	auto scope_inNode = ax_scope_value( pos.inNode, structNode );
	
	for(;;) {
		parser.skipNewLines();
		if( token.is_curlyBracketClose() ) {
			nextToken();
			break;
		}
		
		auto modifier = parser.parseDeclarationModifier();
		
		if( token.is_var()			) {	parse_PropNode		(modifier); continue; }
		if( token.is_let()			) { parse_PropNode		(modifier); continue; }
		if( token.is_fn()			) {	parse_FuncNode		(modifier); continue; }
					
		if( token.is_class() 		) { parse_StructNode	(modifier); continue; }
		if( token.is_struct() 		) { parse_StructNode	(modifier); continue; }
		if( token.is_interface() 	) { parse_StructNode	(modifier); continue; }

//		if( token.is_enum()			) { parseEnum		(modifier); continue; }

		Log::Error( token, ax_txt("unexpected token") );
	}
}

bool DeclarePass::resolve_PropType( ax_Obj< PropNode >	node ) {
	return true;
}

bool DeclarePass::resolve_StructBaseType( ax_Obj< StructureType > node ) {

	auto n = node->baseOrInterfacePos.size();
	ax_int c = 0;
	
	if( ! node->baseType.is_null() ) {
		c++;
	}
	
	for( auto i=c; i<n; i++ ) {
		auto pos = node->baseOrInterfacePos[i];
		parser.setPos( pos );
		
		ax_if_not_let( t, parseTypename() ) {
			return false;
		}
		
		ax_if_not_let( s, t->ax_as< StructureType >() ) {
			Log::Error( pos, ax_txt("interface / class / struct expected") );
		}

		ax_if_let( f, t->ax_as< InterfaceNode >() ) {
			node->interfaces.add( f );
			continue;
		}
		
		if( i != 0 ) {
			Log::Error( pos, ax_txt("cannot have multiple base class") );
		}
		node->baseType = s;
	}
	
	if( node->baseType.is_null() && node->ax_is< ClassNode >() ) {
		node->baseType = g_compiler->metadata.type_object;
	}
	return true;
}

void DeclarePass::parse_FuncNode( DeclarationModifier & modifier ) {
	if( ! token.is_fn() ) Log::Error( token, ax_txt("fn expected") );

	if( propPass ) {
		parser.skipUntil( TokenType::t_curlyBracketOpen );
		nextToken();
		parser.skipCurlyBracket();
		return;
	}

	nextToken();

	ax_if_not_let( inNode, pos.inNode ) {
		throw System::Err_Undefined();
	}
		
	if( ! inNode->ax_is< ClassNode >() && ! inNode->ax_is< StructNode >() && ! inNode->ax_is< InterfaceNode >() ) {
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
	
	parser.skipRoundBracket();
	
	if( token.is_identifier() ) {
		fo->returnTypePos = token.pos;
		parser.skipTypeName();
	}
	
	if( ! token.is_curlyBracketOpen() ) {
		if( ! modifier.b_extern ) {
			Log::Error( token, ax_txt("{{ expected") );
		}
	}else{
		fo->bodyPos = token.pos;
		nextToken();
		parser.skipCurlyBracket();
	}
}

void DeclarePass::parse_PropNode( DeclarationModifier & modifier ) {
	if( ! token.is_var() && ! token.is_let() ) {
		Log::Error( token, ax_txt("let / var expected") );
	}
	
	if( ! propPass ) {
		parser.skipUntilEndOfLine();
		nextToken();
		return;
	}
	
	auto nodeType = token.type;

	ax_if_not_let( inNode, pos.inNode ) {
		throw System::Err_Undefined();
	}
	
	if( ! inNode->ax_is< ClassNode >() && ! inNode->ax_is< StructNode >() ) {
		Log::Error( token, ax_txt("cannot delcare property here") );
	}
	
	nextToken();

	for(;;) {
		if( ! token.is_identifier() ) Log::Error( token, ax_txt("var name expected") );
		
		auto new_node = ax_new_obj( PropNode, inNode, token.pos, token.str );
		nextToken();
				
		new_node->nodeType = nodeType;
		new_node->modifier = modifier;
						
		if( token.is_identifier() ) {
			new_node->typePos = token.pos;
			parser.skipTypeName();
		}
		
		if( token.is_assign() ) {
			nextToken();
			new_node->initExpr = parseExpr_Primary();
			
//			new_node->initExprPos = token.pos;
//			parser.skipExpression();
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
}

}} //namespace


