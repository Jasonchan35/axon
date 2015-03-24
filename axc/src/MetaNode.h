//
//  MetaNode.h
//  axc
//
//  Created by Jason on 2014-12-07.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__MetaNode__
#define __axc__MetaNode__

#include "Log.h"

namespace ax {
namespace Compile {

class LexerPos;
class MetaNode : public System::Object {
	ax_DefObject( MetaNode, System::Object )
public:

	void init( MetaNode* parent, LexerPos & pos, const ax_string & name );

	template< typename T >
	ax_Nullable< ax_Obj<T> >	getUpperByType	() {
//		ax_dump( name );
		ax_assert( this );
		auto p = this->ax_as<T>();
		if( p.getValue() ) return p;
		return parent ? parent->getUpperByType<T>() : ax_Nullable< ax_Obj<T> >();
	}


	ax_string			name;

	ax_ptr< MetaNode >	parent;
	ax_Obj< ax_Dict< ax_string, ax_Obj< MetaNode > > >	children;
	
	virtual void OnStringReq( ax_ToStringReq & req ) const;
};

class Node_namespace : public MetaNode {
	ax_DefObject( Node_namespace, MetaNode )
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	Node_namespace( MetaNode* parent, LexerPos & pos, const ax_string & name );

	ax_Obj< Node_namespace > getOrAddNamespace	( const ax_string & name, LexerPos & pos );
};


}} //namespace

#endif /* defined(__axc__MetaNode__) */
