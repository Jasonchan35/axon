//
//  Compiler.h
//  Compile
//
//  Created by Jason on 2014-11-10.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__Compiler__
#define __axc__Compiler__

#include "SourceFile.h"
#include "Metadata.h"

namespace ax {
namespace Compile {

class Compiler {
public:
	Compiler();
	
	void	compile( const ax_string & project_root );
	void	loadAllSourceFiles();
	
	void	dumpMetadata();
	
	void	declarePass();
	
	void	genCppPass();

	ax_string							project_root;
	ax_Array_< ax_Obj<SourceFile> >		sourceFiles;
	
	Metadata		metadata;
};

extern Compiler*	g_compiler;

}} //namespace


#endif /* defined(__axc__Compiler__) */
