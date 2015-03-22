//
//  ExternalArray.h
//  ax_core
//
//  Created by Jason on 2015-03-22.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_ExternalArray_h
#define ax_core_ExternalArray_h

#include "Array.h"

namespace ax {
namespace System {

template< typename T >
class ExternalArray : public Array<T> {
	typedef	Array<T>	base;
public:
	ExternalArray() {}
	ExternalArray( T* p, ax_int size ) : base( p, size, size ) {}

protected:
	virtual	T*		onMalloc		( ax_int req_size, ax_int & capacity ) { return nullptr; }
	virtual	void	onFree			( T* p, bool call_from_destructor ) {}

};

}} //namespace

#endif
