//
//  test_c_str.cpp
//  ax_core_unit_test
//
//  Created by Jason on 2015-03-14.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#include <ax/core.h>

namespace ax {

using namespace System;

void test_c_str() {
	std::cout << ax_strlen( "abcd" ) << " Hello\n";
}

} // namespace


