//
//  ArrayUtility.h
//  ax_core
//
//  Created by Jason on 2015-03-15.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_ArrayUtility_h
#define ax_core_ArrayUtility_h

#include "../base/Ptr.h"
#include "../base/Memory.h"

namespace ax {
namespace System {

const ax_int ax_buffer_size_to_use_memcpy = 2048;

template< typename T > inline
void ax_copy_loop( T* dst, const T* src, ax_int len ) {
	auto e = src + len;
	for( ; src < e; ++src, ++dst ) {
		*dst = * src;
	}
}

inline void ax_memcpy( void* dst, const void* src, ax_int len ) {
	if( len <= 0 ) return;
	if( len > ax_buffer_size_to_use_memcpy ) {
		::memcpy( dst, src, len );
		return;
	}
	const ax_int w = sizeof( size_t );
	auto n = len / w;
	auto r = len % w;
	auto j = n*w;
	ax_copy_loop( (size_t*)dst,   (size_t*)src,   n );
	ax_copy_loop( (char*  )dst+j, (char*  )src+j, r );
	
}

template< typename T > inline
void ax_set_zero_loop( T* dst, ax_int len ) {
	auto e = dst + len;
	for( ; dst < e; ++dst ) {
		*dst = 0;
	}
}

inline void ax_bzero( void* dst, ax_int len ) {
	if( len <= 0 ) return;
	if( len > ax_buffer_size_to_use_memcpy ) {
		::bzero( dst, len );
		return;
	}
	const ax_int w = sizeof( size_t );
	auto n = len / w;
	auto r = len % w;
	auto j = n*w;
	ax_set_zero_loop( (size_t*)dst,   n );
	ax_set_zero_loop( (char*  )dst+j, r );
}


struct ArrayUtility : StaticClass {
	template< typename T >
	ax_ALWAYS_INLINE( 	static	void	Copy( T* dst, const T* src, ax_int n ) 	) {
		if( n <= 0 ) return;
		
		if( dst+n > src && dst < src+n ) {
			throw Err_BufferOverlapped();
		}
		
		if( ax_type_is_pod<T>() ) {
			ax_memcpy( dst, src, n * sizeof(T) );
		}else{
			ax_copy_loop( dst, src, n );
		}
	}

	template< typename DST, typename SRC >
	ax_ALWAYS_INLINE( 	static	void	StaticCastCopy( DST* dst, const SRC* src, ax_int n ) 	) {
		auto e = src + n;
		for( ; src<e; ++src, ++dst ) {
			*dst = static_cast<DST>(*src);
		}
	}

	template< typename T >
	ax_ALWAYS_INLINE( 	static	void	SetAllZeroForGC( T* p, ax_int n ) 		) {
		if( ax_type_gc_trace<T>() ) {
			if( n <= 0 ) return;
			ax_bzero( p, n * sizeof(T) );
		}
	}

	template< typename T >
	ax_ALWAYS_INLINE( 	static	void	Constructor( T* p, ax_int n ) 		) {
		if( n <= 0 ) return;
		
		if( ax_type_is_pod<T>() ) {
			return ax_bzero( p,n * sizeof(T) );
		}
		
		auto s = p;
		auto e = p + n;
		
		try{
			for( ; s<e; ++s ) {
				ax_call_constructor( T, s );
			}
				
		}catch(...){
			for( ; s>=p; --s ) {
				ax_call_destructor( T, s );
			}
			throw;
		}
	}

	template< typename T >
	ax_ALWAYS_INLINE( 	static	void	CopyConstructor( T* dst, const T* src, ax_int n ) 	) {
		if( n <= 0 ) return;
	
		if( ax_type_is_pod<T>() ) {
			ax_memcpy( dst, src, n * sizeof(T) );
		}else{
			auto s = src;
			auto e = src + n ;
			
			try{
				for( ; s<e; ++s, ++dst ) {
					ax_call_constructor( T, dst, *s );
				}
					
			}catch(...){
				--s;
				for( ; s>=src; --s, --dst ) {
					ax_call_destructor( T, dst );
				}
				throw;
			}
		}
	}

	template< typename T >
	ax_ALWAYS_INLINE( 	static	void	MoveConstructor( T* dst, T* src, ax_int n ) 	){
		if( n <= 0 ) return;
	
		if( ax_type_is_pod<T>() ) {
			ax_memcpy( dst, src, n * sizeof(T) );
		}else{
			auto s = src;
			auto e = src + n ;
			
			try{
				for( ; s<e; ++s, ++dst ) {
					ax_call_constructor( T, dst, ax_move(*s) );
				}
					
			}catch(...){			
				// move back
				--s;
				for( ; s>=src; --s, --dst ) {
					*src = ax_move(*dst);
					ax_call_destructor( T, dst );
				}
				throw;
			}
		}
	}

	template< typename T >
	ax_ALWAYS_INLINE( 	static	void	Destructor( T* p, ax_int n ) 	) {
		if( ax_type_is_pod<T>() ) return;
		
		if( n <= 0 ) return;
		
		T *e = p + n;
		for( ; p<e; ++p ) {
			ax_call_destructor( T, p );
		}
	}
	
};

}} // namespace ax

#endif
