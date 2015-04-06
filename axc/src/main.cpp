//
//  main.cpp
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "Compiler.h"

using namespace ax;

int main() {

	ax::Compile::Compiler	c;

#if 1
	auto filename = ax_txt(__FILE__);
	auto changeDir = System::IO::Path::GetDirectoryName( filename );
	ax_dump( changeDir );
	
	System::IO::Directory::SetCurrent( changeDir );

	c.compile( ax_txt("../../example/test001") );
#else
	c.compile( ax_txt("./") );
#endif

	return 0;
}