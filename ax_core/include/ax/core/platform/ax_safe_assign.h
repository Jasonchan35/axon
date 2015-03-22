//
//  safe_assign.h
//  ax_core
//
//  Created by Jason on 2015-03-15.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_safe_assign_h
#define ax_core_safe_assign_h


template<class DST, class SRC> bool	ax_safe_assign( DST& dst, const SRC& src );

//same type assign
#define	ax_TYPE_LIST_ITEM( NAME, T )	\
	template<> inline bool ax_safe_assign( T& dst, const T& src ) { dst = src; return true; }
//-----
	ax_TYPE_LIST_all_int
	ax_TYPE_LIST_all_uint
	ax_TYPE_LIST_all_float
	ax_TYPE_LIST_all_char
#undef ax_TYPE_LIST_ITEM

template<class DST, class SRC> inline 
bool	ax_safe_assign( DST& dst, const SRC& src ) {
	DST tmp = (DST) src;

	//unsigned << signed
	if( ax_type_is_unsigned<DST>() && ! ax_type_is_unsigned<SRC>() ) {
		if( ax_less_than0( src ) ) {
			return false;
		}
	}
	
	// signed << unsigned
	if( ! ax_type_is_unsigned<DST>() && ax_type_is_unsigned<SRC>() ) {
		if( ax_less_than0( tmp ) ) {
			return false;
		}
	}

	//avoid overflow
	if( src != (SRC) tmp ) {
		return false;
	}
	
	dst = tmp; //done
	return true;
}

#endif
