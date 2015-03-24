//
//  Array.h
//  ax_core
//
//  Created by Jason Chan on Mar 16, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_Array_h
#define ax_core_Array_h

#include "../base/Obj.h"
#include "ArrayEnumerator.h"
#include "LocalBuffer.h"

namespace ax {
namespace System {

template< typename T >
class Array : public Object {
	typedef	Object		base;
	typedef Array<T>	THIS_CLASS;
public:

	Array() : _data(nullptr), _size(0), _capacity(0) {}
	virtual ~Array() {}

	ax_ALWAYS_INLINE(	void	clear		()	);
	ax_ALWAYS_INLINE(	void	release	() 	)	{ _release(false); }
	
	ax_ALWAYS_INLINE(	 	  T &	operator[]	( ax_int  i )		) { return at(i); }
	ax_ALWAYS_INLINE(	const T &	operator[]	( ax_int  i ) const	) { return at(i); }

	ax_ALWAYS_INLINE(	 	  T &	at			( ax_int  i )		) { _checkBound(i); return _data[i]; }
	ax_ALWAYS_INLINE(	const T &	at			( ax_int  i ) const	) { _checkBound(i); return _data[i]; }

	ax_ALWAYS_INLINE( 		  T &	unsafe_at	( ax_int  i ) 		) { _debug_checkBound(i); return _data[i]; }
	ax_ALWAYS_INLINE( 	const T &	unsafe_at	( ax_int  i ) const	) { _debug_checkBound(i); return _data[i]; }

	ax_ALWAYS_INLINE( 		  T &	last		()					) { return at( _size-1 ); }
	ax_ALWAYS_INLINE( 	const T &	last		() 		 	 const	) { return at( _size-1 ); }

	ax_ALWAYS_INLINE( 		  T &	last		( ax_int  i )		) { at( _size-i-1 ); }
	ax_ALWAYS_INLINE( 	const T &	last		( ax_int  i ) const	) { at( _size-i-1 ); }
	
	ax_ALWAYS_INLINE(		bool	inBound		( ax_int  i ) const	) { return i >= 0 && i < _size; }
	
	ax_ALWAYS_INLINE(		ax_int	size		() const			) { return _size; }

	ax_ALWAYS_INLINE( 			T*	dataPtr		()					) { return _data; }
	ax_ALWAYS_INLINE(	const 	T*	dataPtr		() const			) { return _data; }
	

	ax_ALWAYS_INLINE(		ax_int	capacity	() const			) { return _capacity; }

	ax_ALWAYS_INLINE( 		void	resize		( ax_int new_size ) );
	ax_ALWAYS_INLINE( 		void	incSize		( ax_int n ) )			{ resize( _size+n ); }

	ax_ALWAYS_INLINE( 		void	reserve		( ax_int new_size ) );


	ax_ALWAYS_INLINE(	 	void	append		( const T &  v ) 	);
	ax_ALWAYS_INLINE( 		void	append		(       T && v ) 	);
	
	ax_ALWAYS_INLINE(	 	void	append		( const T* data, ax_int data_size 	) );
	ax_ALWAYS_INLINE( 		void	append		( const Array<T> &  rhs ) );
	ax_ALWAYS_INLINE(		void	append		( 		Array<T> && rhs )	);
	
	ax_ALWAYS_INLINE(		void	assign		( const T* data, ax_int data_size 	) ) { clear(); append( data, data_size ); }
	ax_ALWAYS_INLINE(		void	assign		( const Array<T> &  rhs			) ) { clear(); append( rhs ); }
	ax_ALWAYS_INLINE(		void	operator=	( const Array<T> &  rhs 			) )	{ clear(); append( rhs ); }
	ax_ALWAYS_INLINE(		void	operator=	(		Array<T> && rhs 			) ) { move(rhs); }

							void	move		( Array<T> &  rhs ) { onMove( rhs ); }

	virtual	bool	isDataOnHeap	() const { return false; }

	ArrayEnumerator< THIS_CLASS,       T >			getEnumerator		() 		 { return ArrayEnumerator< THIS_CLASS,      T >		 ( *this, dataPtr(), dataPtr()+_size ); }
	ArrayEnumerator< THIS_CLASS, const T >			getEnumerator		() const { return ArrayEnumerator< THIS_CLASS,const T >		 ( *this, dataPtr(), dataPtr()+_size ); }
	
	ArrayInverseEnumerator< THIS_CLASS,       T >	getInverseEnumerator() 		 { return ArrayInverseEnumerator< THIS_CLASS,      T >( *this, dataPtr(), dataPtr()+_size ); }
	ArrayInverseEnumerator< THIS_CLASS, const T >	getInverseEnumerator() const { return ArrayInverseEnumerator< THIS_CLASS,const T >( *this, dataPtr(), dataPtr()+_size ); }


#if ax_DEBUG_Enumerating
	RefCount	_enumerating;
#endif

	T*			_data;
	ax_int		_size;
	ax_int		_capacity;

protected:

	ax_ALWAYS_INLINE( void 	_checkBound			( ax_int i ) const ) { if( ! inBound(i) ) throw Err_Array_OutOfRange(); }
	ax_ALWAYS_INLINE( void	_debug_checkBound	( ax_int i ) const ) {
		#if _DEBUG
			_checkBound(i);
		#endif
	}

	Array( T* data, ax_int size, ax_int capacity ) : _data(data), _size(size), _capacity(capacity) {}

	virtual	void	onMove			( Array<T> &  rhs );
	
	virtual	T*		onMalloc		( ax_int req_size, ax_int & capacity ) = 0;
	virtual	void	onFree			( T* p, bool call_from_destructor ) = 0;
	
			void	_do_reserve		( ax_int newSize );
			void	_release		( bool call_from_destructor );
};

//! Array with LocalBuffer
template< typename T, ax_int LOCAL_BUF_SIZE = ( sizeof(void*)*4 ) / sizeof(T) >
class Array_ : public Array<T>, private LocalBuffer< T, LOCAL_BUF_SIZE >  {
	typedef	Array< T >	base;
	typedef	LocalBuffer< T, LOCAL_BUF_SIZE >	BUF;
public:

	virtual	~Array_() { base::_release(true); }

	Array_			() {}
	Array_			( Array<T>    && rhs ) { base::move(rhs); }
	void operator=	( Array<T>    && rhs ) { base::move(rhs); }

	virtual	bool	isDataOnHeap	() const { return base::dataPtr() != nullptr && base::dataPtr() != getLocalBufferPtr(); }
	
	virtual	void	onMove( Array<T> & rhs ) {
		if( rhs.isDataOnHeap() ) {
			base::release();
			
			base::_data		= rhs._data;
			base::_size		= rhs._size;
			base::_capacity	= rhs._capacity;
			
			rhs._data		= nullptr;
			rhs._size		= 0;
			rhs._capacity	= 0;
		}else{
			base::onMove( rhs );
		}
	}
	
	Array<T> &		asIArray() { return static_cast< Array<T> >( *this ); }

	virtual	const	T*	getLocalBufferPtr() const { return BUF::localBufPtr(); }
	
protected:
	virtual	T*		onMalloc	( ax_int req_size, ax_int & out_capacity ) {
		//std::cout << this << " onMalloc " << reqSize <<"\n";
	
		if( req_size <= LOCAL_BUF_SIZE ) {
			out_capacity = LOCAL_BUF_SIZE;
			return BUF::localBufPtr();
		}else{
			auto s = base::size();
			auto n = ax_max( s + s/2, req_size ); //auto resize to 1.5x times

			auto p =  Memory::AllocUncollect<T>( n );
			return p;
		}
	}
	
	virtual	void	onFree		( T* p, bool call_from_destructor ) {
		//std::cout << this << " onFree   " << "\n";
		if( p == BUF::localBufPtr() ) {
			if( ! call_from_destructor && ax_type_gc_trace<T>() ) {
				ArrayUtility::SetAllZero( p, LOCAL_BUF_SIZE );
			}
		}else{
			Memory::DeallocUncollect<T>( p );
		}
	}

};

typedef	Array< ax_byte >	ByteArray;



template< typename T > inline
void	Array<T>::clear() {
	ArrayUtility::Destructor( _data, _size );
	_size = 0;
}

template< typename T > inline
void	Array<T>::_release( bool call_from_destructor ) {
	clear();
	if( dataPtr() ) {
		onFree( dataPtr(), call_from_destructor );
		_data = nullptr;
		_size = 0;
		_capacity = 0;
	}
}

template< typename T > inline
void	Array<T>::resize		( ax_int new_size ) {
	if( new_size < 0 ) throw Err_Undefined();
	auto old_size = _size;
	if( new_size == old_size ) return;
	
	if( new_size <  old_size ) {
		ArrayUtility::Destructor ( dataPtr() + new_size, old_size - new_size );
	}else{
		reserve( new_size );
		ArrayUtility::Constructor( dataPtr() + old_size, new_size - old_size );
	}
	_size = new_size;
}

template< typename T > inline
void	Array<T>::reserve		( ax_int new_size ) {
	if( new_size <= _capacity ) return;
	_do_reserve( new_size );
}


template< typename T > inline
void	Array<T>::append		( const T& v ) {
	reserve( _size+1 );
	ax_call_constructor( T, _data + _size, v );
	++_size;
}

template< typename T > inline
void	Array<T>::append		( T && v ) {
	reserve( _size+1 );
	ax_call_constructor( T, _data + _size, v );
	++_size;
}

template< typename T > inline
void	Array<T>::append		( const Array<T> & rhs ) {
	append( rhs.dataPtr(), rhs.size() );
}

template< typename T > inline
void	Array<T>::append		( const T* data, ax_int data_size ) {
	reserve( _size + data_size );
	ArrayUtility::CopyConstructor( dataPtr() + _size, data, data_size );
	_size += data_size;
}

template< typename T > inline
void	Array<T>::onMove		( Array<T> &  rhs ) {
	clear();
	
	reserve( _size + rhs.size() );
	ArrayUtility::MoveConstructor( dataPtr() + _size, rhs.dataPtr(), rhs.size() );
	_size += rhs.size();
	rhs._size = 0;
}

template< typename T > inline
void	Array<T>::append		( Array<T> && rhs ) {
	if( _size == 0 ) return move( rhs );

	reserve( _size + rhs.size() );
	ArrayUtility::MoveConstructor( dataPtr() + _size, rhs.dataPtr(), rhs.size() );
	_size += rhs.size();
	rhs._size = 0;
}

template< typename T > inline
void	Array<T>::_do_reserve( ax_int new_size ) {
	auto np = onMalloc( new_size, _capacity );
	if( np != _data ) {
		ArrayUtility::MoveConstructor( np, dataPtr(), _size );
		ArrayUtility::Destructor     (     dataPtr(), _size );
		
		if( _data ) {
			onFree( dataPtr(), false );
		}
		_data = np;
	}
}

}} //namespace

#endif
