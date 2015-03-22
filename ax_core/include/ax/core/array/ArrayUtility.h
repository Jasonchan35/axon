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

struct ArrayUtility : StaticClass {
	enum { k_use_memcpy_byte_size = 1024 };
	
	template< typename T >
	ax_ALWAYS_INLINE( 	static	void	Copy( T* dst, const T* src, ax_int n ) 	) {
		if( n <= 0 ) return;
		
		if( dst+n > src && dst < src+n ) {
			throw Err_BufferOverlapped();
		}
		
		if( ax_type_is_pod<T>() && n * sizeof(T) < k_use_memcpy_byte_size ) {
			::memcpy( dst, src, n * sizeof(T) );
		}else{
			auto e = src+n;
			for( ; src<e; ++src, ++dst ) {
				*dst = *src;
			}
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
	ax_ALWAYS_INLINE( 	static	void	SetAllZero( T* p, ax_int n ) 		) {
		if( n <= 0 ) return;
		
		if( n * sizeof(T) < k_use_memcpy_byte_size ) {
			auto c = (char*)( p );
			auto e = (char*)( p + n );
			for( ; c<e; c++ ) {
				*c = 0;
			}
		}else{
			::memset( (void*)p, 0, n * sizeof(T) );
		}
	}

	template< typename T >
	ax_ALWAYS_INLINE( 	static	void	Constructor( T* p, ax_int n ) 		) {
		if( n <= 0 ) return;
		
		if( ax_type_is_pod<T>() ) {
			return SetAllZero( p,n );
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
			if( n * sizeof(T) >= k_use_memcpy_byte_size ) {
				::memcpy( (void*)dst, (void*)src, n * sizeof(T) );
				
			}else{
				auto s = src;
				auto e = src + n ;
				for( ; s<e; ++s, ++dst ) {
					*dst = *s;
				}
			}
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
			if( n * sizeof(T) >= k_use_memcpy_byte_size ) {
				::memcpy( (void*)dst, (void*)src, n * sizeof(T) );
				
			}else{
				auto s = src;
				auto e = src + n ;
				for( ; s<e; ++s, ++dst ) {
					*dst = ax_move(*s);
				}
			}
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
