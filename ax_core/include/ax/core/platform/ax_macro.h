//
//  macro.h
//  ax_core
//
//  Created by Jason on 2014-11-21.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_macro_h
#define ax_core_macro_h

#define ax_COMMA	,

#define ax__FILE__				ax_txt(__FILE__)
#define ax__FILE_BASENAME__		ax::System::StringX<ax_char>::_MakeExternal_c_str( ax_c_str_file_basename( ax_sz(__FILE__) ) )

#define ax__LINE__			__LINE__


#define ax_JOIN_WORD(w0,w1)		w0##w1
#define ax_JOIN_WORD3(w0,w1,w2)	w0 ## w1 ## w2

#define	ax_debug_only_sz( a )	a

//#ifdef _DEBUG
//	#define	ax_debug_only_sz( a )	a
//#else
//	#define	ax_debug_only_sz( a )	""
//#endif


#define	ax_call_constructor( T, p, ... )	( ::new(p) T(__VA_ARGS__) )
#define ax_call_destructor(  T, p )			( p->~T() )


#define ax_foreach( IT, LIST )	for( auto IT : (LIST).getEnumerator() )

// use for Nullable<T> / Weak<T>
#define ax_if_let( VAR_NAME, RHS ) \
	decltype( (RHS).unsafeGet() ) 	VAR_NAME; \
	if( (RHS).tryGet( VAR_NAME ) )
//---------

#define ax_if_not_let( VAR_NAME, RHS ) \
	decltype( (RHS).unsafeGet() ) 	VAR_NAME; \
	if( ! (RHS).tryGet( VAR_NAME ) )
//---------

#if _DEBUG
	#define ax_assert(a)	assert(a)
	#define	ax_assert_equals( a, b )	\
		{	\
			if( (a) != (b) ) {	\
				std::cout << "!! assert_equals\n"; \
				ax_std_dump( a ); \
				ax_std_dump( b ); \
				std::wcout << '\n'; \
			}	\
			ax_assert( (a) == (b) );	\
		} \
	//-------
#else
	#define ax_assert(a)				/* nothing */
	#define	ax_assert_equals( a, b )	/* nothing */
#endif


#define	ax_move(a)		std::move(a)
#define ax_sizeof(a)	static_cast<ax_int>( sizeof(a) )


#define ax_VA_NUM_ARGS(...) 			ax_VA_NUM_ARGS_IMPL_((__VA_ARGS__, 16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1))
#define ax_VA_NUM_ARGS_IMPL_(tuple) 	ax_VA_NUM_ARGS_IMPL tuple
#define ax_VA_NUM_ARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,N,...) N

#define ax_MACRO_DISPATCHER(func, ...)   			ax_MACRO_DISPATCHER_(func, ax_VA_NUM_ARGS(__VA_ARGS__))
#define ax_MACRO_DISPATCHER_(func, nargs)           ax_MACRO_DISPATCHER__(func, nargs)
#define ax_MACRO_DISPATCHER__(func, nargs)          func ## nargs


#endif
