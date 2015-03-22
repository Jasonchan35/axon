//
//  test_Dict.cpp
//  ax_core_unit_test
//
//  Created by Jason on 2015-03-20.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#include <ax/core.h>

namespace ax {

using namespace System;

class TestDict_Data {
public:
	TestDict_Data( int v = 10 ) : v(v) {}
	
	int	v;
	int foo[300];
	
};

void test_Dict() {
	Dict< ax_int, ax_int >	dict;
	
	for( ax_int i=0; i<40; ++i ) {
		dict.add( i, i*10 );
	}
	
//	ax_dump( dict );
}

} //namespace