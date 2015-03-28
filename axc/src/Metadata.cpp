//
//  Metadata.cpp
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "Metadata.h"

namespace ax {
namespace Compile {

Metadata::Metadata() {
	root = ax_new_obj( namespace_node, ax_NullableObj< MetaNode >(nullptr), LexerPos(), ax_txt("<root>") );
	root->nodeType = TokenType::t_namespace;
}

void Metadata::OnStringReq( ax_ToStringReq & req ) const {
	req << ax_txt("======= Metadata =======\n");
	req << root;	
	req << ax_txt("======= End Metadata =======\n");
}

}} //namespace