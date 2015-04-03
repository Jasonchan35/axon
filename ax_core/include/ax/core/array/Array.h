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
	struct	ax_type_on_gc_trace : public ax_type_gc_trace<T> {};

	virtual ~Array() {}

	ax_ALWAYS_INLINE(	void	clear	()	);
	ax_ALWAYS_INLINE(	void	release	() 	);
	
	ax_ALWAYS_INLINE(	 	  T &	operator[]	( ax_int  i )		) { return at(i); }
	ax_ALWAYS_INLINE(	const T &	operator[]	( ax_int  i ) const	) { return at(i); }

	ax_ALWAYS_INLINE(	 	  T &	at			( ax_int  i )		) { _checkBound(i); return _data[i]; }
	ax_ALWAYS_INLINE(	const T &	at			( ax_int  i ) const	) { _checkBound(i); return _data[i]; }

	ax_ALWAYS_INLINE( 		  T &	unsafe_at	( ax_int  i ) 		) { _debug_checkBound(i); return _data[i]; }
	ax_ALWAYS_INLINE( 	const T &	unsafe_at	( ax_int  i ) const	) { _debug_checkBound(i); return _data[i]; }

	ax_ALWAYS_INLINE( 		  T &	last		()					) { return at( _size-1 ); }
	ax_ALWAYS_INLINE( 	const T &	last		() 		 	 const	) { return at( _size-1 ); }

	ax_ALWAYS_INLINE( 		  T &	unsafe_last	()					) { return unsafe_at( _size-1 ); }
	ax_ALWAYS_INLINE( 	const T &	unsafe_last	() 		 	 const	) { return unsafe_at( _size-1 ); }

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

	ax_ALWAYS_INLINE(		T&		addNew		() )					{ incSize(1); return unsafe_last(); }

	ax_ALWAYS_INLINE(	 	void	add			( const T &  v ) 	);
	ax_ALWAYS_INLINE( 		void	add			(       T && v ) 	);
	
	ax_ALWAYS_INLINE(	 	void	add			( const T* data, ax_int data_size 	) );
	ax_ALWAYS_INLINE( 		void	add			( const Array<T> &  rhs ) );
	ax_ALWAYS_INLINE(		void	add			( 		Array<T> && rhs )	);
	
	ax_ALWAYS_INLINE(		void	assign		( const T* data, ax_int data_size 	) ) { clear(); add( data, data_size ); }
	ax_ALWAYS_INLINE(		void	assign		( const Array<T> &  rhs				) ) { clear(); add( rhs ); }
	ax_ALWAYS_INLINE(		void	operator=	( const Array<T> &  rhs 			) )	{ clear(); add( rhs ); }
	ax_ALWAYS_INLINE(		void	operator=	(		Array<T> && rhs 			) ) { move(rhs); }

							void	move		( Array<T> &  rhs ) { onMove( rhs ); }

	virtual	bool	isDataOnHeap	() const { return true; }

	ArrayEnumerator< THIS_CLASS,       T >			getEnumerator		() 		 { return ArrayEnumerator< THIS_CLASS,      T >		 ( *this, dataPtr(), dataPtr()+_size ); }
	ArrayEnumerator< THIS_CLASS, const T >			getEnumerator		() const { return ArrayEnumerator< THIS_CLASS,const T >		 ( *this, dataPtr(), dataPtr()+_size ); }
	
	ArrayInverseEnumerator< THIS_CLASS,       T >	getInverseEnumerator() 		 { return ArrayInverseEnumerator< THIS_CLASS,      T >( *this, dataPtr(), dataPtr()+_size ); }
	ArrayInverseEnumerator< THIS_CLASS, const T >	getInverseEnumerator() const { return ArrayInverseEnumerator< THIS_CLASS,const T >( *this, dataPtr(), dataPtr()+_size ); }


	void	OnStringReq( ax_ToStringReq & req ) const;


#if ax_DEBUG_Enumerating
	RefCount	_enumerating;
#endif

	T*			_data;
	ax_int		_size;
	ax_int		_capacity;

protected:
	Array() : _data(nullptr), _size(0), _capacity(0) {}

	ax_ALWAYS_INLINE( void 	_checkBound			( ax_int i ) const ) { if( ! inBound(i) ) throw Err_Array_OutOfRange(); }
	ax_ALWAYS_INLINE( void	_debug_checkBound	( ax_int i ) const ) {
		#if _DEBUG
			_checkBound(i);
		#endif
	}

	Array( T* data, ax_int size, ax_int capacity ) : _data(data), _size(size), _capacity(capacity) {}

	virtual	void	onMove			( Array<T> &  rhs );
	
	virtual	T*		onMalloc		( ax_int req_size, ax_int & capacity ) = 0;
	virtual	void	onFree			( T* p ) = 0;
	
			void	_do_reserve		( ax_int newSize );
};

//! Array with LocalBuffer
template< typename T, ax_int LOCAL_BUF_SIZE = ( sizeof(void*)*4 ) / sizeof(T) >
class Array_ : public Array<T>, private LocalBuffer< T, LOCAL_BUF_SIZE >  {
	typedef	Array< T >	base;
	typedef	LocalBuffer< T, LOCAL_BUF_SIZE >	BUF;
public:

	virtual	~Array_() { base::release(); }

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
			out_capacity = n;
			return p;
		}
	}
	
	virtual	void	onFree		( T* p ) {
		//std::cout << this << " onFree   " << "\n";
		if( p != BUF::localBufPtr() ) {
			Memory::DeallocUncollect<T>( p );
		}
	}

};

template< typename T > using ArrayObj = Obj< Array<T> >;

typedef	Array< ax_byte >	ByteArray;
typedef ArrayObj< ax_byte >	ByteArrayObj;



template< typename T > inline
void	Array<T>::clear() {
	resize(0);
}

template< typename T > inline
void	Array<T>::release() {
	clear();
	if( _data ) {
		onFree( _data );
		_data = nullptr;
		_capacity = 0;
	}
}

template< typename T > inline
void	Array<T>::resize		( ax_int new_size ) {
	if( new_size < 0 ) throw Err_Undefined();
	auto old_size = _size;
	if( new_size == old_size ) return;
	
	if( new_size <  old_size ) {
		auto dst = dataPtr() + new_size;
		auto n   = old_size  - new_size;
		ArrayUtility::Destructor ( dst, n );
		ArrayUtility::SetAllZeroForGC( dst, n );
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
void	Array<T>::add		( const T& v ) {
	reserve( _size+1 );
	ax_call_constructor( T, _data + _size, v );
	++_size;
}

template< typename T > inline
void	Array<T>::add		( T && v ) {
	reserve( _size+1 );
	ax_call_constructor( T, _data + _size, v );
	++_size;
}

template< typename T > inline
void	Array<T>::add		( const Array<T> & rhs ) {
	add( rhs.dataPtr(), rhs.size() );
}

template< typename T > inline
void	Array<T>::add		( const T* data, ax_int data_size ) {
	reserve( _size + data_size );
	ArrayUtility::CopyConstructor( dataPtr() + _size, data, data_size );
	_size += data_size;
}

template< typename T > inline
void	Array<T>::onMove		( Array<T> &  rhs ) {
	clear();
	
	reserve( rhs.size() );
	ArrayUtility::MoveConstructor( dataPtr(), rhs.dataPtr(), rhs.size() );
	_size = rhs.size();
	
	rhs.resize(0);
}

template< typename T > inline
void	Array<T>::add		( Array<T> && rhs ) {
	if( _size == 0 ) return move( rhs );

	reserve( _size + rhs.size() );
	ArrayUtility::MoveConstructor( dataPtr() + _size, rhs.dataPtr(), rhs.size() );
	_size += rhs.size();
	rhs.resize(0);
}

template< typename T > inline
void	Array<T>::_do_reserve( ax_int new_size ) {
	auto np = onMalloc( new_size, _capacity );
	if( np != _data ) {
		ArrayUtility::MoveConstructor( np, dataPtr(), _size );
		ArrayUtility::Destructor     (     dataPtr(), _size );
		
		if( _data ) {
			onFree( dataPtr() );
		}
		_data = np;
	}
}

}} //namespace

#endif
