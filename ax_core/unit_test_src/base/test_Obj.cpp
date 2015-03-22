//
//  test_Obj.cpp
//  ax_core_unit_test
//
//  Created by Jason Chan on Mar 19, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#include <ax/core.h>

namespace ax {

using namespace System;

class TestObj_Data : public Object {
public:
	struct	ax_type_on_gc_trace : public std::true_type {};
	
	static	ax_int	count;

	TestObj_Data( ax_int v ) : v(v) {
		count++;
	}
	
	virtual ~TestObj_Data() {
		count--;
//		std::cout << "~TestObj_Data( " << v << " " << count << " )\n";
	}
	
	ax_int	v;
	int foo[300];
};

class Vec2 {
public:
	struct	ax_type_on_gc_trace : public std::false_type {};
	
	int x,y;
};

class TestObj_POD {
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	int a;
	Vec2	v;
};

ax_int TestObj_Data::count = 0;

void test_Obj() {
	ax_dump( Memory::GC_GetHeapSize() );

	auto a = ax_obj_new( TestObj_Data, 1000 );
	auto b = ax_obj_new( TestObj_Data, 1001 );

	for( ax_int i=0; i<100; ++i ) {
		ax_obj_new( TestObj_Data, i );
	}
	
	ax_dump( Memory::GC_GetHeapSize() );
	Memory::GC_Collect();
	ax_dump( Memory::GC_GetHeapSize() );
	
	ax_dump( ax_type_gc_trace<int>::value );
	ax_dump( ax_type_gc_trace<int*>::value );
	ax_dump( ax_type_gc_trace<int&>::value );
	ax_dump( ax_type_gc_trace< Ptr<int> >::value );
	ax_dump( ax_type_gc_trace<TestObj_POD>::value );
	ax_dump( ax_type_gc_trace<TestObj_POD*>::value );
	ax_dump( ax_type_gc_trace<Vec2>::value );
	Memory::GC_Collect();
}

} //namespace