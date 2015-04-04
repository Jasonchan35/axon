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

	void	resolveStructBaseTypes();
	void	parsePropPass	();

private:
	void	resolvePropTypePass();

	bool	propPass;

	void	parse_NamespaceBody();
	
	void	parse_namespace		();
	
	void	parse_StructNode	( DeclarationModifier & modifier );
	
	bool	resolve_StructBaseType	( ax_Obj< StructureType > 	structNode );
	bool	resolve_PropType		( ax_Obj< Prop >		Prop );
	
	void	parse_StructBody	( ax_Obj< StructureType > structNode );
	
	void	parse_PropNode		( DeclarationModifier & modifier );
	void	parse_FuncNode		( DeclarationModifier & modifier );	
};


}} //namespace

#endif /* defined(__axc__DeclarePass__) */
