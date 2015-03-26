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

	MetaNode( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	template< typename T >
	ax_NullableObj<T>	getUpperByType	() {
//		ax_dump( name );
		ax_assert( this );
		
		ax_if_let( p, this->ax_as<T>() ) {
			return p;
		}
		
		ax_if_let( o, parent ) {
			return o->getUpperByType<T>();
		}else{
			return nullptr;
		}
	}


	ax_string			name;
	LexerPos			pos;

	ax_NullableObj< MetaNode >	parent;
	
	typedef ax_Dict< ax_string, ax_Obj< MetaNode > >	ChildrenDict;
	
	ax_Obj<	ChildrenDict >	children;
	
	virtual void OnStringReq( ax_ToStringReq & req ) const;
};

class Node_namespace : public MetaNode {
	ax_DefObject( Node_namespace, MetaNode )
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	Node_namespace( ax_NullableObj< MetaNode > parent, const LexerPos & pos, const ax_string & name );

	ax_Obj< Node_namespace > getOrAddNamespace	( const ax_string & name, LexerPos & pos );
};


}} //namespace

#endif /* defined(__axc__MetaNode__) */
