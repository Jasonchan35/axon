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

void DeclarePass::process( SourceFile & sourceFile ) {

	ax_log( "compiling... [{?}]", sourceFile.filename );
	
	parser.init( sourceFile );
	parser.lexer.c.pos.inNode = g_compiler->metadata.root.get();
	
	parser.nextToken();
	_process();
}

void DeclarePass::_process() {
	for(;;) {
		parser.skipNewLines();
		if( token.is_EOF() ) break;

		auto modifier = parser.getDeclarationModifier();
		
//		if( token.is_class() 	) { parseClass		(modifier); continue; }
//		if( token.is_struct() 	) { parseStruct		(modifier); continue; }
//		if( token.is_interface()) { parseInterface	(modifier); continue; }
//		if( token.is_func() 	) { parseFunc		(modifier); continue; }

		if( ! modifier.isEmpty() ) {
			Log::Error( token, "unexpected token after declaration modifier");
		}
	//-----
	
//		if( token.is_typealias() ) { parseTypealias	(); continue; }
		if( token.is_namespace() ) { parse_namespace();	continue; }
	
		if( token.is_curlyBracketClose() ) {
			parser.nextToken();
			break;
		}
		
		Log::Error( token, "unexpected token {?}", token.str );
	}
}

void DeclarePass::parse_namespace() {
	assert( token.is_namespace() );
	parser.nextToken();

	auto scope_inNode = ax_scope_value( pos.inNode );
	
	auto ns0 = pos.inNode->getUpperByType< Node_namespace >();
	auto ns = *ns0.getValue();

	
	for(;;) {
		auto scope_ns = ax_scope_value( ns );		
		for(;;) {
			if( ! token.is_identifier() ) {
				Log::Error( token, "namespace name expected" );
			}

			ns = ns->getOrAddNamespace( token.str, token.pos );
			
			parser.nextToken();
			
			if( token.is_dot() ) {
				parser.nextToken();
				continue;
			}			
			pos.inNode = ns.get();
			
			break;
		}
		
		if( token.is_curlyBracketOpen() ) {
			parser.nextToken();
			_process();
			
			break;
		}
		
		Log::Error( token, "unexpected token" );
	}
}



}} //namespace
