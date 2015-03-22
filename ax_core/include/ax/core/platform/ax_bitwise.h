//
//  ax_bitwise.h
//  axon_core
//
//  Created by Jason on 2014-12-15.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef axon_core_ax_bitwise_h
#define axon_core_ax_bitwise_h

#define	axTYPE_LIST( T ) \
    inline bool ax_bit_hasAny		( T value,  T bits )			{ return (value & bits) != 0;   }	\
    inline bool ax_bit_hasAll		( T value,  T bits )			{ return (value & bits) == bits; }	\
	\
    inline T	ax_bit_or			( T value, T bits )				{ return value | bits; }	\
    inline T	ax_bit_and			( T value, T bits )				{ return value & bits; }	\
    inline T	ax_bit_xor			( T value, T bits )				{ return value ^ bits; }	\
    inline T	ax_bit_clr			( T value, T bits )				{ return value & ~bits; }	\
    inline T	ax_bit_set			( T value, T bits )				{ return value | bits; }	\
    inline T	ax_bit_set			( T value, T bits, bool b )		{ return ( b ) ? ax_bit_or ( value, bits ) : ax_bit_clr( value, bits ); }	\
    inline T	ax_bit_toggle		( T value, T bits )				{ return value ^ bits; }	\
	\
    inline void ax_bit_or_it		( T & value, T bits )			{ value |= bits; }	\
    inline void ax_bit_and_it		( T & value, T bits )			{ value &= bits; }	\
    inline void ax_bit_xor_it		( T & value, T bits )			{ value ^= bits; }	\
    inline void ax_bit_clr_it		( T & value, T bits )			{ value &= ~bits; }	\
    inline void ax_bit_set_it		( T & value, T bits )			{ value |= bits; }	\
    inline void ax_bit_set_it		( T & value, T bits, bool b )	{ if( b ) ax_bit_or_it ( value, bits ); else ax_bit_clr_it( value, bits ); }	\
    inline void ax_bit_toggle_it	( T & value, T bits )			{ value ^= bits; }	\
//-----
    axTYPE_LIST( uint64_t );
    axTYPE_LIST( uint32_t );
    axTYPE_LIST( uint16_t );
    axTYPE_LIST( uint8_t  );
#undef axTYPE_LIST


#endif
