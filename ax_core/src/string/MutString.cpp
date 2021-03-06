//
//  MutString.cpp
//  ax_core
//
//  Created by Jason Chan on Mar 17, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#include <ax/core/string/MutString.h>

namespace ax {
namespace System {

template< typename T >
void	MutStringX<T>::_do_reserve( ax_int new_size ) {
	if( new_size < 0 ) throw Err_Undefined();
	auto np = new_size == 0 ? nullptr : onMalloc( new_size, _capacity );
	if( np != _data ) {
		if( _data ) {
			ArrayUtility::Copy( np, _data, _size );
			onFree( _data );
		}		
		_data = np;
	}
}

template< typename T >
void MutStringX<T>::append	( const T& ch ) {
	auto old_size = size();
	resize( old_size + 1 );
	unsafe_at( old_size ) = ch;
}

template< typename T >
void MutStringX<T>::appendRepeat	( const T& ch, ax_int repeat ) {
	if( repeat <= 0 ) return;

	auto old_size = size();
	resize( old_size + repeat );
	
	auto dst = dataPtr() + old_size;
	auto e   = dst + repeat;
	for( ; dst<e; dst++ ) {
		*dst = ch;
	}
}


template< typename T >
void	MutStringX<T>::_append	( const T*	 data, ax_int data_size, ax_int repeat ) {
	if( repeat    <= 0 ) return;
	if( data_size <= 0 ) return;

	auto req_len  = data_size;
	auto old_size = size();
	auto new_size = old_size + req_len * repeat;

	resize( new_size );
	auto dst = dataPtr() + old_size;
	
	for( ax_int i=0; i<repeat; ++i ) {
		ArrayUtility::Copy( dst, data, data_size );
		dst += req_len;
	}
}

//-----------
template< typename T >
template< typename R >
void	MutStringX<T>::_append_static_cast	( const R*	 data, ax_int data_size, ax_int repeat ) {
	if( repeat    <= 0 ) return;
	if( data_size <= 0 ) return;

	auto req_len  = data_size;
	auto old_size = size();
	auto new_size = old_size + req_len * repeat;

	resize( new_size );
	auto dst = dataPtr() + old_size;
	
	for( ax_int i=0; i<repeat; ++i ) {
		ArrayUtility::StaticCastCopy( dst, data, data_size );
		dst += req_len;
	}
}

//-----------
template< typename T >
template< typename UTF >
void	MutStringX<T>::_appendUtf( const UTF* data, ax_int data_size, ax_int repeat ) { \
	if( repeat    <= 0 ) return;
	if( data_size <= 0 ) return;
	ax_int req_len = Utf::GetConvertedCount< UTF,T >( data, data_size );

	auto old_size = size();
	auto new_size = old_size + req_len * repeat;

	resize( new_size );

	try{
		auto dst = dataPtr() + old_size;
		Utf::Convert( dst, req_len, data, data_size );

		auto p0 = dst;
		dst += req_len;
	
		for( ax_int i=1; i<repeat; ++i ) {
			ArrayUtility::Copy( dst, p0, req_len );
			dst += req_len;
		}
		
	}catch(...){
		resize( old_size );
		throw;
	}
}


//-----------

#define	ax_TYPE_LIST_ITEM( NAME, T )	\
	template<> \
	template<> \
	inline void	MutStringX<T>::_appendUtf( const T* data, ax_int data_size, ax_int repeat ) { _append( data, data_size, repeat ); } \
	/*---- The explicit instantiation ---*/ \
	template class MutStringX<T>; \
//--------
	ax_TYPE_LIST_all_char	
#undef	ax_TYPE_LIST_ITEM


}} //namespace

