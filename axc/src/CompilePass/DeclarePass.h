//
//  DeclarePass.h
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__DeclarePass__
#define __axc__DeclarePass__

#include "CompilePass.h"

namespace ax {
namespace Compile {

class DeclarePass : public CompilePass {
public:
	void	parseFile( ax_Obj< SourceFile > srcFile );

private:
	void	_process();
	
	void	parse_namespace			();
	
	void	parse_StructNode	( DeclarationModifier & modifier );
	void	parse_PropNode		( DeclarationModifier & modifier );
	void	parse_FuncNode		( DeclarationModifier & modifier );
	
};


}} //namespace

#endif /* defined(__axc__DeclarePass__) */
