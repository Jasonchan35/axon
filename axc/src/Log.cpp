//
//  Log.cpp
//  axc
//
//  Created by Jason on 2014-12-09.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include "Log.h"
#include "Compiler.h"

namespace ax {
namespace Compile {

void Log::dumpMetadata() {
	g_compiler->dumpMetadata();
}

}} //namespace