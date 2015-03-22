//
//  Memory.cpp
//  ax_core
//
//  Created by Jason on 2015-03-15.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#define	GC_DEBUG	1
#include "../../../../bdwgc/include/gc.h"

#include <ax/core/base/Memory.h>

namespace ax {
namespace System {

Memory::Init::Init() {
	std::cout << "GC_INIT";
	GC_INIT();
	GC_enable_incremental(); // Incremental/Generational Collection
}
Memory::Init	Memory::s_init;

void	Memory::GC_Collect() 		{ ::GC_gcollect(); }
ax_int	Memory::GC_GetHeapSize() 	{ return ::GC_get_heap_size(); }

void	Memory::GC_RegisterFinalizer( void* obj, FinalizeFunc func, void* clientData ) {
	GC_REGISTER_FINALIZER( obj, func, clientData, nullptr, nullptr );
}

template<>	void*	Memory::_DoAllocBytes<false,false>	( ax_int req_bytes ) { return ::malloc( req_bytes ); }
template<>	void*	Memory::_DoAllocBytes<true, true>	( ax_int req_bytes ) { return ::GC_MALLOC( req_bytes ); }
template<>	void*	Memory::_DoAllocBytes<false,true>	( ax_int req_bytes ) { return ::GC_MALLOC_ATOMIC( req_bytes ); }
template<>	void*	Memory::_DoAllocBytes<true, false>	( ax_int req_bytes ) { return ::GC_MALLOC_UNCOLLECTABLE( req_bytes ); }

template<>	void	Memory::_DeallocBytes<false,false>	( void* p ) { return ::free(p); }
template<>	void	Memory::_DeallocBytes<true, true>	( void* p ) { return ::GC_FREE(p); }
template<>	void	Memory::_DeallocBytes<false,true>	( void* p ) { return ::GC_FREE(p); }
template<>	void	Memory::_DeallocBytes<true, false>	( void* p ) { return ::GC_FREE(p); }


}} //namespace

