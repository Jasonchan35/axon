//
//  LexerPos.h
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__LexerPos__
#define __axc__LexerPos__

#include "define.h"

namespace ax {
namespace Compile {

class	SourceFile;
class	MetaNode;

class LexerPos {
public:
	struct	ax_type_on_gc_trace : public std::false_type {};

	LexerPos() {
		valid = false;
		filePos = 0;
		line = 0;
		col = 0;
		blockLevel = 0;
	}
	
	ax_string	dumpSourceLine	() const;
	ax_string	getSourceLine	() const;

	ax_Nullable< ax_Obj< SourceFile > >		file;
	ax_Nullable< ax_Obj< MetaNode   > >		inNode;

	bool		valid;

	ax_int		filePos;
	
	ax_int		line;
	ax_int		col;

	ax_int		blockLevel;
	
	void OnStringReq( ax_ToStringReq & req ) const {
		req << dumpSourceLine();
	}
};

}} //namespace

#endif /* defined(__axc__LexerPos__) */
