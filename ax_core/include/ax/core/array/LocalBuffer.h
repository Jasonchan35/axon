//
//  LocalBuffer.h
//  ax_core
//
//  Created by Jason Chan on Mar 16, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_LocalBuffer_h
#define ax_core_LocalBuffer_h

namespace ax {
namespace System {

template< typename T, ax_int LOCAL_BUF_SIZE=0 >
class LocalBuffer {
public:
	ax_ALWAYS_INLINE( 			ax_int	localBufSize() 			) { return LOCAL_BUF_SIZE; }
	ax_ALWAYS_INLINE( 			T*		localBufPtr	() 			) { return reinterpret_cast<	  T*>(_localBuf); }
	ax_ALWAYS_INLINE( 	const 	T*		localBufPtr	() const	) { return reinterpret_cast<const T*>(_localBuf); }

private:
	char	_localBuf[ sizeof(T) * LOCAL_BUF_SIZE ];
};

template< typename T >
class LocalBuffer<T,0> {
public:
	ax_ALWAYS_INLINE( 			ax_int	localBufSize() 			) { return 0; }
	ax_ALWAYS_INLINE( 			T*		localBufPtr	() 			) { return reinterpret_cast<T*>(0); }
	ax_ALWAYS_INLINE( 	const	T*		localBufPtr	() const	) { return reinterpret_cast<T*>(0); }
};

}} // namespace

#endif
