//
//  LexerPos.cpp
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "LexerPos.h"
#include "SourceFile.h"

namespace ax {
namespace Compile {

ax_string LexerPos::getSourceLine () const {
	ax_TempString str;

	ax_if_not_let( file_, file ) {
		return ax_string();
	}
	
	auto lineStartPos = filePos - col +1; // col start from 1 instead of 0, so +1
	if( lineStartPos >= file_->source.size() ) return ax_string();
	if( lineStartPos < 0 ) lineStartPos = 0;
	
	auto p = file_->source.c_str( lineStartPos );
	auto e = ax_strchr( p, '\n' );
	if( e ) {
		return ax_string::Make( p, e-p );
	}else{
		return ax_string::Make_c_str( p );
	}
}

ax_string LexerPos::dumpSourceLine() const {
	ax_TempString		arrow;
	ax_TempString		buf;
	
	auto sourceLine = getSourceLine();

	if( sourceLine.size() > 0 ) {
		arrow.reserve( col * 2 );
		
		auto p = sourceLine.c_str();
		size_t i=1;
		for( ; *p; ++p, ++i ) {
			if( i >= col ) break;
			switch( *p ) {
				case '\t': {
					size_t r = 4 - ( arrow.size() % 4 );
					for( size_t j=0; j<r; ++j ) {
						arrow.append('-');
					}				
				}break;
				
				default:	arrow.append('-'); break;
			}
		}
		arrow.append( ax_txt("^") );
	}
	
	ax_string	filename;
	ax_if_let( file_, file ) {
		filename = file_->filename;
	}
	
	ax_format_to_buffer( buf, "\n Source: {?}\n --------{?}\n   File: {?}:{?},{?}\n", sourceLine, arrow, filename, line, col );
	return buf.to_String();
}

}} //namespace