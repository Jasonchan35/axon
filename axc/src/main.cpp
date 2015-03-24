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
	
	auto filename = ax_txt(__FILE__);

	auto changeDir = System::IO::Path::GetDirectoryName( filename );
	ax_dump( changeDir );
	
	System::IO::Directory::SetCurrent( changeDir );

	ax::Compile::Compiler	c;
	c.compile( ax_txt("../../../../example/src/test003") );

	return 0;
}