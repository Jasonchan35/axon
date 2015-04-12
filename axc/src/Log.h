//
//  Log.h
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__Log__
#define __axc__Log__

#include "Token.h"

namespace ax {
namespace Compile {

class Log : public System::StaticClass {
public:
	template< typename... Args >
	static void Error( Token & token, Args &&... args ) { Error( &token, &token.pos, args... ); }

	template< typename... Args >
	static void Error( const Location & pos, Args &&... args ) { Error( nullptr, &pos, args... ); }

	static void dumpMetadata();

	template< typename... Args >
	static void Error( Token* token, const Location* pos, const ax_string & fmt ) {
		dumpMetadata();
		
		ax_log( ax_txt("!! ERROR:") );
		ax_log( fmt );
		if( token ) ax_log( ax_txt("{?}"), *token );
		if( pos ) 	ax_log( ax_txt("{?}"), *pos );
		throw System::Err_Undefined();
	}
	
	template< typename... Args >
	static void Error( Token* token, const Location* pos, Args &&... args ) {
		dumpMetadata();
		
		ax_log( ax_txt("!! ERROR:") );
		ax_log( args... );
		if( token ) ax_log( ax_txt("{?}"), *token );
		if( pos ) 	ax_log( ax_txt("{?}"), *pos );
		throw System::Err_Undefined();
	}

	
};


}} //namespace
#endif /* defined(__axc__Log__) */
