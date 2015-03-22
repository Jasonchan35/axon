//
//  test_MutString.cpp
//  ax_core_unit_test
//
//  Created by Jason Chan on Mar 17, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#include <ax/core.h>

namespace ax {

using namespace System;

void test_MutString() {
	MutStringL	a( ax_str("ABCDEF\n") );
	MutStringL	b( ax_strW("Testing wchar_t\n") );

	std::cout << a << b;
	
	
}

} //namespace