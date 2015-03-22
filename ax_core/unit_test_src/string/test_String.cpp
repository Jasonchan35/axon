//
//  test_ImString.cpp
//  ax_core_unit_test
//
//  Created by Jason Chan on Mar 17, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#include <ax/core.h>

namespace ax {

using namespace System;

void test_String() {
	auto a = ax_sz("ABCD");

	std::cout << a;
}


} //namespace