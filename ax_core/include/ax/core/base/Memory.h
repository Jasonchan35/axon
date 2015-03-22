//
//  Memory.h
//  ax_core
//
//  Created by Jason on 2015-03-15.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_Memory_h
#define ax_core_Memory_h

#include "Exception.h"


#define	ax_new(  T, ... )			( ::new( ax::System::Memory::Alloc<T>(1) ) T(__VA_ARGS__) )
#define	ax_new_uncollect(  T, ... )	( ::new( ax::System::Memory::AllocUncollect<T>(1) ) T(__VA_ARGS__) )

#define ax_delete( p )				( ax::System::Memory::Delete(p) )
#define ax_delete_uncollect( p )	( ax::System::Memory::DeleteUncollect(p) )


namespace ax {
namespace System {

class Memory {
public:
	template< typename T >
	ax_ALWAYS_INLINE(	static	T*	Alloc ( ax_int n ) ) {
		auto req_bytes = n * sizeof(T);
		return	(T*)( ax_type_gc_trace<T>() ? _AllocBytes<true,true>( req_bytes ) : _AllocBytes<false,true>( req_bytes ) );
	}

	template< typename T >
	ax_ALWAYS_INLINE(	static	T*	AllocUncollect	( ax_int n ) ) {
		auto req_bytes = n * sizeof(T);
		return	(T*)( ax_type_gc_trace<T>() ? _AllocBytes<true,false>( req_bytes ) : _AllocBytes<false,false>( req_bytes ) );
	}
	
	typedef	void	(*FinalizeFunc)( void* obj, void* clientData );
	static	void	GC_RegisterFinalizer( void* obj, FinalizeFunc func, void* clientData );

	template< typename T >
	ax_ALWAYS_INLINE(	static	void	Dealloc( T* p ) ){
		return ax_type_gc_trace<T>() ? _DeallocBytes<true,true>(p) : _DeallocBytes<false,true>(p);
	}

	template< typename T >
	ax_ALWAYS_INLINE(	static	void	DeallocUncollect( T* p ) ) {
		return ax_type_gc_trace<T>() ? _DeallocBytes<true,false>(p) : _DeallocBytes<false,false>(p);
	}

	template< typename T >
	ax_ALWAYS_INLINE(	static	void	Delete( T* p ) ) {
		p->~T();
		Dealloc( p );
	}

	template< typename T >
	ax_ALWAYS_INLINE(	static	void	DeleteUncollect( T* p ) ) {
		p->~T();
		DeallocUncollect( p );
	}

	static	void	GC_Collect();
	static	ax_int	GC_GetHeapSize();

private:

	template< bool TRACE, bool COLLECT >	static	void*	_DoAllocBytes	( ax_int req_bytes );
	template< bool TRACE, bool COLLECT >	static	void*	_AllocBytes		( ax_int req_bytes ) {
		auto p = _DoAllocBytes< TRACE, COLLECT >( req_bytes );
		if( p == nullptr ) throw Err_NotEnoughMemory();
		return p;
	}
	
	template< bool TRACE, bool COLLECT >	static	void	_DeallocBytes	( void* p );


	struct Init {
		Init();
	};
	
	static	Init	s_init;
};

}} //namespace

#endif
