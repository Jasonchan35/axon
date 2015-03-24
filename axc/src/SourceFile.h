//
//  SourceFile.h
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__SourceFile__
#define __axc__SourceFile__

#include "define.h"

namespace ax {
namespace Compile {

class SourceFile : public System::Object {
	ax_DefObject( SourceFile, System::Object );
public:

	void	loadFile( const ax_string & filename );

	ax_string	filename;
	ax_string	source;
};

}} //namespace

#endif /* defined(__axc__SourceFile__) */
