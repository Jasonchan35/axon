//
//  CompilePass.h
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__CompilePass__
#define __axc__CompilePass__

#include "../Parser.h"
#include "../Compiler.h"

namespace ax {
namespace Compile {

class CompilePass {
public:
	CompilePass() : parser( token, pos ) {}


	void	nextToken();

	Token		token;
	LexerPos	pos;
	Parser		parser;
};

}} //namespace


#endif /* defined(__axc__CompilePass__) */
