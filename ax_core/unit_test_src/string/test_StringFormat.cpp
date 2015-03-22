//
//  test_StringFormat.cpp
//  ax_core_unit_test
//
//  Created by Jason Chan on Mar 18, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#include <ax/core.h>

namespace ax {

using namespace System;

void test_StringFormat() {
	ax_get_typeinfo<int32_t>();
	std::cout << ax_format("string format = {?} {?,10} {?}\n", 1, ax_str("abcd"), 3 );
}

} //namespace