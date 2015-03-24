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
	root->name = ax_txt("<root>");
}

void Metadata::OnStringReq( ax_ToStringReq & req ) const {
	req << ax_txt("======= Metadata =======\n");
	req << root;	
	req << ax_txt("======= End Metadata =======\n");
}

}} //namespace