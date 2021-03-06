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
	void	run2ndPass				();
	void	findBuildinTypePass		();

private:
	template< typename T >
	void	findBuildinType	( ax_Obj<T> & o, const ax_string & name ) {
		ax_if_not_let( p, _findBuildinType( name ) ) {
			Log::Error( nullptr, nullptr, ax_txt("cannot find buildin type {?}"), name );
		}
		if( ! p->ax_is<T>() ) {
			Log::Error( nullptr, nullptr, ax_txt("invalid buildin type {?}"), name );
		}
		o = p->ax_cast<T>();
		o->buildin = true;
	}
	
	ax_NullableObj< MetaNode >	_findBuildinType( const ax_string & name );

	void	resolveStructTypePass	();
	
	void	parsePropPass			();
	void	resolvePropPass			();
	
	void	resolveFuncParamPass	();

	bool	inPropPass;

	void	parseNamespace			();
	void	parseNamespaceBody		();
	
	void	parseCompsiteType			( Modifier & modifier );
	void	parseStructTypeBody		( ax_Obj< CompositeTypeSpec >	node );
	
	bool	resolveStructType		( ax_Obj< CompositeTypeSpec > 	node );
	bool	resolveProp				( ax_Obj< Prop >		Prop );
		
	void	parseProp				( Modifier & modifier );
	void	parseFunc				( Modifier & modifier );
	
	void	resolveFuncParam		( ax_Obj< FuncOverload > fo );
};


}} //namespace

#endif /* defined(__axc__DeclarePass__) */
