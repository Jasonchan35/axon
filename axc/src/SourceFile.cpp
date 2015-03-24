//
//  SourceFile.cpp
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "SourceFile.h"

namespace ax {
namespace Compile {

void SourceFile::loadFile( const ax_string & filename ) {
	this->filename = filename;
	this->source = System::IO::File::ReadAllText( filename );
}

}} //namespace