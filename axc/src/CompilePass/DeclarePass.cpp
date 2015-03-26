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

	ax_log( "compiling... [{?}]", sourceFile->filename );
	
	parser.reset( sourceFile );
	parser.lexer.c.pos.inNode = g_compiler->metadata.root;
	
	parser.nextToken();
	_process();
}

void DeclarePass::_process() {
	for(;;) {
		parser.skipNewLines();
		if( token.is_EOF() ) break;

		auto modifier = parser.parseDeclarationModifier();
		
		if( token.is_class() 	) { parse_StructureNode	(modifier); continue; }
//		if( token.is_struct() 	) { parse_struct	(modifier); continue; }
//		if( token.is_interface()) { parse_interface	(modifier); continue; }
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
	
	ax_if_not_let( ns, ns0->getUpperByType< namespace_node >() ) {
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
			_process();
			
			break;
		}
		
		Log::Error( token, ax_txt("unexpected token") );
	}
}



void DeclarePass::parse_StructureNode( DeclarationModifier & class_modifier ) {
	
	auto declare_type = token.type;
	nextToken();
	
	ax_if_not_let( inNode, pos.inNode ) {
		throw System::Err_Undefined();
	}
	
	if( ! inNode->ax_is< namespace_node	>() && ! inNode->ax_is< StructureNode >() ) {
		Log::Error( token, ax_txt("cannot delcare class here") );
	}

	if( ! token.is_identifier() ) Log::Error( token, ax_txt("type name expected") );
	
	ax_Obj< StructureNode >	new_node;
	
	switch( declare_type ) {
	case TokenType::t_class:	{ new_node	= ax_new_obj( class_node,		inNode, token.pos, token.str ); }break;
	case TokenType::t_struct:	{ new_node	= ax_new_obj( struct_node,		inNode, token.pos, token.str ); }break;
	case TokenType::t_interface:{ new_node	= ax_new_obj( interface_node,	inNode, token.pos, token.str ); }break;
	default: { Log::Error( token, ax_txt("class / struct / interface expected") ); } break;
	}

	nextToken();
	
	new_node->modifier = class_modifier;
	
	if( token.is_colon() ) {
		nextToken();
		
		for(;;) {
			if( token.is_identifier() ) {
				new_node->baseOrInterfacePos.append( token.pos );
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
	
	auto scope_inNode = ax_scope_value( pos.inNode, new_node );
	
	for(;;) {
		parser.skipNewLines();
		if( token.is_curlyBracketClose() ) {
			nextToken();
			break;
		}
		
		auto modifier = parser.parseDeclarationModifier();
		
		if( token.is_var()		) {	parse_prop		(modifier); continue; }
//		if( token.is_let()		) {	do_prop			(modifier); continue; }
//		if( token.is_fn()		) {	do_func			(modifier); continue; }
//
//		if( token.is_interface()) { parseInterface	(modifier); continue; }
//		if( token.is_class()	) { parseClass		(modifier); continue; }
//		if( token.is_struct()	) { parseStruct		(modifier); continue; }
//		if( token.is_enum()		) { parseEnum		(modifier); continue; }
		
		
		Log::Error( token, ax_txt("unexpected token") );
	}
}


void DeclarePass::parse_prop( DeclarationModifier & prop_modifier ) {
	auto scope_inNode = ax_scope_value( pos.inNode );

	ax_if_not_let( inNode, pos.inNode ) {
		throw System::Err_Undefined();
	}
	
	if( ! inNode->ax_is< class_node >() && ! inNode->ax_is< struct_node >() ) {
		Log::Error( token, ax_txt("cannot delcare property here") );
	}

	
	if( ! token.is_var() && ! token.is_let() ) Log::Error( token, ax_txt("let / var expected") );
	
	bool is_let = token.is_let();
	nextToken();


	for(;;) {
		if( ! token.is_identifier() ) Log::Error( token, ax_txt("var name expected") );
		
		auto new_node = ax_new_obj( PropNode, inNode, token.pos, token.str );
		nextToken();
				
		new_node->is_let = is_let;
		new_node->modifier = prop_modifier;
				
		if( token.is_colon() ) {
			nextToken();
			new_node->dataTypePos = token.pos;
			parser.skipTypeName();
		}
		
		if( token.is_assign() ) {
			nextToken();
			new_node->initExprPos = token.pos;
			parser.skipExpression();
		}
				
		if( ! new_node->dataTypePos.valid && ! new_node->initExprPos.valid ) {
			Log::Error( token, ax_txt("unknown type for property") );
		}
	
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


