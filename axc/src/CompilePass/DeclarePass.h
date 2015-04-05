//
//  DeclarePass.h
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__DeclarePass__
#define __axc__DeclarePass__

#include "../Parser.h"
#include "MetaNode.h"

namespace ax {
namespace Compile {

class DeclarePass : public Parser {
public:
	void	parseFile( ax_Obj< SourceFile > srcFile );

	void	resolveStructBaseTypes();
	void	parsePropPass	();

private:
	void	resolvePropTypePass();

	bool	propPass;

	void	parse_Namespace		();
	void	parse_NamespaceBody();	
	
	void	parse_StructType		( DeclarationModifier & modifier );
	void	parse_StructTypeBody	( ax_Obj< StructType > structNode );
	
	bool	resolve_StructBaseType	( ax_Obj< StructType > 	structNode );
	bool	resolve_PropType		( ax_Obj< Prop >		Prop );
	
	
	void	parse_Prop				( DeclarationModifier & modifier );
	void	parse_Func				( DeclarationModifier & modifier );
};


}} //namespace

#endif /* defined(__axc__DeclarePass__) */
