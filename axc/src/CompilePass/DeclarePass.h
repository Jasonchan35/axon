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
	void	process( ax_Obj< SourceFile > srcFile );

private:
	void	_process();
	
//	void	parseUsing				();
//	void	parseTypealias			();
	void	parse_namespace			();
	
//	void	parseStruct				( DeclarationModifier & modifier );
//	void	parseClass				( DeclarationModifier & modifier );
//	void	parseEnum				( DeclarationModifier & modifier );
//	void	parseInterface			( DeclarationModifier & modifier );
//
//	void	parseFunc				( DeclarationModifier & modifier );
//	void	parseProperty			( DeclarationModifier & modifier );
//	
//	void	parseMembers			( DeclarationModifier & modifier );
	
};


}} //namespace

#endif /* defined(__axc__DeclarePass__) */
