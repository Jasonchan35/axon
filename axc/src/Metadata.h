//
//  Metadata.h
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef __axc__Metadata__
#define __axc__Metadata__

#include "MetaNode.h"

namespace ax {
namespace Compile {

class Metadata {
public:
	Metadata();

	ax_Obj< namespace_node > root;

	void OnStringReq( ax_ToStringReq & req ) const;
	
	ax_Array_< ax_Obj<StructNode> >	structList;
};


}} //namespace

#endif /* defined(__axc__Metadata__) */
