//
//  main.cpp
//  axon_test
//
//  Created by Jason on 2015-03-14.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

namespace ax {

#define	ax_test_case( fn )	\
	std::cout << "\n==== " #fn " =======\n";	\
	void	test_##fn(); \
	test_##fn(); \
//----

void run_test_cases() {
//	ax_test_case( c_str );
//	ax_test_case( String );
//	ax_test_case( MutStringBuffer );
	ax_test_case( StringFormat );
	ax_test_case( Obj );
	ax_test_case( Dict );
}

} //namespace

int main() {
	ax::run_test_cases();
	ax::System::Memory::GC_Collect();
//--------
	std::cout << "\n\n ==== Program Ended ==== \n\n";
#if ax_COMPILER_VC
	std::cout << " ! Press Enter to Quit\n\n"
	getchar();
#endif
	return 0;
}

