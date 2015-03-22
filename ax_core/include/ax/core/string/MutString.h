//
//  MutString.h
//  ax_core
//
//  Created by Jason Chan on Mar 17, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_MutString_h
#define ax_core_MutString_h

#include "ax_c_str.h"
#include "UtfConverter.h"
#include "String.h"
#include "../array/Array.h"

namespace ax {
namespace System {

//! Mutable String Interface
template< typename T >
class StringBuilderX : public Object {
	typedef	StringX<T>		base;
public:
	virtual	~StringBuilderX() {}

	ax_ALWAYS_INLINE(  void	clear()		);
	ax_ALWAYS_INLINE(  void	release()	)	{ _release(false); }
	
	
	ax_ALWAYS_INLINE(	 	  T &	operator[]	( ax_int  i )		) { return at(i); }
	ax_ALWAYS_INLINE(	const T &	operator[]	( ax_int  i ) const	) { return at(i); }

	ax_ALWAYS_INLINE(	 	  T &	at			( ax_int  i )		) { _checkBound(i); return ax_const_cast( _data[i] ); }
	ax_ALWAYS_INLINE(	const T &	at			( ax_int  i ) const	) { _checkBound(i); return _data[i]; }

	ax_ALWAYS_INLINE( 		  T &	unsafe_at	( ax_int  i ) 		) { _debug_checkBound(i); return ax_const_cast( _data[i] ); }
	ax_ALWAYS_INLINE( 	const T &	unsafe_at	( ax_int  i ) const	) { _debug_checkBound(i); return _data[i]; }

	ax_ALWAYS_INLINE( 		  T &	last		()					) { return at( _size-1 ); }
	ax_ALWAYS_INLINE( 	const T &	last		() 		 	 const	) { return at( _size-1 ); }

	ax_ALWAYS_INLINE( 		  T &	last		( ax_int  i )		) { return at( _size-i-1 ); }
	ax_ALWAYS_INLINE( 	const T &	last		( ax_int  i ) const	) { return at( _size-i-1 ); }

	ax_ALWAYS_INLINE( 			T*	dataPtr		()					) { return ax_const_cast( _data ); }
	ax_ALWAYS_INLINE(	const 	T*	dataPtr		() const			) { return _data; }


	ax_ALWAYS_INLINE(		bool	inBound		( ax_int  i ) const	) { return i >= 0 && i < _size; }

	ax_int		size	() const				{ return _size; }
		
	const T*	c_str	() const 				{ return _data ? _data : ax_empty_c_str<T>(); }
	const T*	c_str	( ax_int offset ) const {
		if( !_data ) {
			if( offset != 0 ) throw  Err_Array_OutOfRange();
			return ax_empty_c_str<T>();
		}
		return & at(offset);
	}

	ax_ALWAYS_INLINE( 		void	resize		( ax_int new_size ) );
	ax_ALWAYS_INLINE( 		void	incSize		( ax_int n ) )			{ resize( _size+n ); }

	ax_ALWAYS_INLINE( 		void	reserve		( ax_int new_size ) );
	
	ax_int	capacity() const { return _capacity; }

						void		move		( StringBuilderX<T> &  rhs ) { onMove( rhs ); }

//---------------
	ax_ALWAYS_INLINE(	void		operator=	( const StringX<T>     &  rhs 		) )	{ clear(); append( rhs ); }
	ax_ALWAYS_INLINE(	void		operator=	( const StringBuilderX &  rhs 		) )	{ clear(); append( rhs ); }
	ax_ALWAYS_INLINE(	void		operator=	(		StringBuilderX && rhs 		) ) { move(rhs); }

	ax_ALWAYS_INLINE( 	void		assign			( const T& ch ) )							{ clear(); append( ch );  }
	ax_ALWAYS_INLINE( 	void		assignRepeat	( const T& ch, ax_int repeat ) )			{ clear(); appendRepeat( ch, repeat );  }
	
	ax_ALWAYS_INLINE( 	void		assign			( const StringX<T>    &  rhs ) )			{ clear(); append( rhs ); }
	ax_ALWAYS_INLINE( 	void		assign			( const StringBuilderX<T> &  rhs ) )			{ clear(); append( rhs ); }
	ax_ALWAYS_INLINE( 	void		assign			(       StringBuilderX<T> && rhs ) )			{ move(rhs); }
	
	ax_ALWAYS_INLINE( 	void		assign			( const T* data, ax_int data_size ) )		{ clear(); append( data, data_size ); }
	
	ax_ALWAYS_INLINE( 	void		append			( const T& ch ) );
	ax_ALWAYS_INLINE( 	void		append			( const StringX<T>    &  rhs ) )			{ _append( rhs.c_str(), rhs.size(), 1 ); }
	ax_ALWAYS_INLINE( 	void		append			( const StringBuilderX<T> &  rhs ) )			{ _append( rhs.c_str(), rhs.size(), 1 ); }
	ax_ALWAYS_INLINE( 	void		append			(       StringBuilderX<T> && rhs ) );
	
	ax_ALWAYS_INLINE( 	void		append			( const T* data, ax_int data_size ) )		{ _append( data, data_size, 1 ); }
	
	ax_ALWAYS_INLINE( 	void		appendRepeat	( const T& ch, ax_int repeat ) );
	ax_ALWAYS_INLINE( 	void		appendRepeat	( const StringX<T> & s, ax_int repeat ) ) 	{ _append( s.c_str(), s.size(), repeat ); }
	

	#define ax_TYPE_LIST_ITEM( NAME, UTF ) \
		ax_ALWAYS_INLINE(	void	assignUtf			( const UTF& ch )									)	{ clear(); appendUtf( ch ); } \
		ax_ALWAYS_INLINE(	void	assignUtf			( const StringX<UTF> & s )							)	{ clear(); appendUtf( s ); } \
		ax_ALWAYS_INLINE(	void	assignUtf			( const UTF* data, ax_int data_size )				)	{ clear(); appendUtf( data, data_size ); } \
		ax_ALWAYS_INLINE(	void	assignUtfRepeat		( const UTF& ch, ax_int repeat )					)	{ clear(); appendUtfRepeat( ch, repeat ); } \
		\
		ax_ALWAYS_INLINE(	void	appendUtf			( const UTF& ch  ) 									)	{ _appendUtf( &ch, 1, 1 ); } \
		ax_ALWAYS_INLINE(	void	appendUtf			( const StringX<UTF> & s ) 						)	{ _appendUtf( s.c_str(), s.size(), 1 ); } \
		ax_ALWAYS_INLINE(	void	appendUtf			( const UTF* data, ax_int data_size )				)	{ _appendUtf( data, data_size, 1 ); } \
		\
		ax_ALWAYS_INLINE(	void	appendUtfRepeat		( const UTF& ch, ax_int repeat )					)	{ _appendUtf( &ch, 1, repeat ); } \
		ax_ALWAYS_INLINE(	void	appendUtfRepeat		( const StringX<UTF> & s, ax_int repeat ) 			)	{ _appendUtf( s.c_str(), s.size(), repeat ); } \
		ax_ALWAYS_INLINE(	void	appendUtfRepeat		( const UTF* data, ax_int data_size, ax_int repeat ))	{ _appendUtf( data, data_size, repeat ); } \
		\
		ax_ALWAYS_INLINE(	void	append_static_cast	( const StringX<UTF> & s ) 						)	{ _append_static_cast( s.c_str(), s.size(), 1 ); } \
		ax_ALWAYS_INLINE(	void	append_static_cast	( const UTF* data, ax_int data_size ) 				)	{ _append_static_cast( data, data_size, 1 ); } \
	//-------------
		ax_TYPE_LIST_all_char
	#undef ax_TYPE_LIST_ITEM
	
	StringX<T>	to_String() { return StringX<T>::Make( c_str(), size() ); }
	
	virtual	bool	isDataOnHeap	() const { return false; }
	
	T*			_data;
	ax_int		_size;
	ax_int		_capacity;
	
protected:
	StringBuilderX() : _data(nullptr), _size(0), _capacity(0) {}
	
	virtual	void	onMove			( StringBuilderX<T> &  rhs );
	
	virtual	T*		onMalloc		( ax_int req_size, ax_int & capacity ) = 0;
	virtual	void	onFree			( T* p, bool call_from_destructor ) = 0;
	
			void	_do_reserve		( ax_int newSize );
			void	_release		( bool call_from_destructor );
	
	ax_ALWAYS_INLINE( void 	_checkBound			( ax_int i ) const ) { if( ! inBound(i) ) throw Err_Array_OutOfRange(); }
	ax_ALWAYS_INLINE( void	_debug_checkBound	( ax_int i ) const ) {
		#if _DEBUG
			_checkBound(i);
		#endif
	}
	
private:
								void	_append				( const T*	 data, ax_int data_size, ax_int repeat );
	template< typename UTF >	void	_appendUtf			( const UTF* data, ax_int data_size, ax_int repeat );
	template< typename R >		void	_append_static_cast	( const R*	 data, ax_int data_size, ax_int repeat );

};


//! Buffered string, LOCAL_BUF_SIZE default is 40 to ensure large enough for GUID/UUID
template< typename T, ax_int LOCAL_BUF_SIZE = 40 >
class StringBuilderX_ : public StringBuilderX<T>, private LocalBuffer< T, LOCAL_BUF_SIZE > {
	typedef	StringBuilderX<T>	base;
	typedef	LocalBuffer< T, LOCAL_BUF_SIZE >	BUF;	
public:
	virtual ~StringBuilderX_() { base::_release(true); }

	StringBuilderX_	() {}
	StringBuilderX_	( StringBuilderX<T>    && rhs ) { base::move(rhs); }
	void operator=	( StringBuilderX<T>    && rhs ) { base::move(rhs); }
	
	
	template< typename UTF >
	StringBuilderX_		( const StringX<UTF> & s ) { base::appendUtf(s); }
	
	virtual	bool	isDataOnHeap	() const { return base::dataPtr() != nullptr && base::dataPtr() != getLocalBufferPtr(); }
	
	virtual	void	onMove( StringBuilderX<T> & rhs ) {
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
	
	StringBuilderX<T> &		asIMutString() { return static_cast< StringBuilderX<T> >( *this ); }

	virtual	const	T*	getLocalBufferPtr() const { return BUF::localBufPtr(); }
	
protected:
	virtual	T*		onMalloc	( ax_int req_size, ax_int & out_capacity ) {
		//std::cout << this << " onMalloc " << reqSize <<"\n";
	
		if( req_size <= LOCAL_BUF_SIZE ) {
			out_capacity = LOCAL_BUF_SIZE;
			return BUF::localBufPtr();
		}else{
			return base::onMalloc( req_size, out_capacity );
		}
	}
	
	virtual	void	onFree		( T* p, bool call_from_destructor ) {
		if( p == BUF::localBufPtr() ) {
			if( ! call_from_destructor && ax_type_gc_trace<T>() ) {
				ArrayUtility::SetAllZero( p, LOCAL_BUF_SIZE );
			}
		}else{
			base::onFree( p, call_from_destructor );
		}
	}
};


typedef	StringBuilderX< ax_char >	MutString;
typedef StringBuilderX_< ax_char >	MutStringL;

template< typename T >	using TempString_ = StringBuilderX_<T,256>;
typedef	TempString_< ax_char >	TempString;

//-----------
template< typename T > inline
void 	StringBuilderX<T>::clear() {
	_size = 0;
}

//-----------
template< typename T > inline
void 	StringBuilderX<T>::_release( bool call_from_destructor ) {
	clear();
	if( dataPtr() ) {
		onFree( dataPtr(), call_from_destructor );
		_data = nullptr;
		_size = 0;
		_capacity = 0;
	}
}

//-----------
template< typename T > inline
void 	StringBuilderX<T>::resize( ax_int new_size ) {
	if( new_size < 0 ) throw Err_Undefined();
	auto old_size = _size;
	if( new_size == old_size ) return;
	
	if( new_size >=  old_size ) {
		reserve( new_size );
	}
	_size = new_size;
	
	if( dataPtr() ) {
		dataPtr()[ new_size ] = 0;
	}
}

template< typename T > inline
void	StringBuilderX<T>::reserve		( ax_int new_size ) {
	if( new_size <= _capacity ) return;
	_do_reserve( new_size );
}

template< typename T > inline
void	StringBuilderX<T>::onMove	( StringBuilderX<T> & rhs ) {
	clear();
	
	auto old_size = _size;
	resize( old_size+ rhs.size() );
	ArrayUtility::MoveConstructor( dataPtr() + old_size, rhs.dataPtr(), rhs.size() );
	rhs._size = 0;
}

template< typename T > inline
void	StringBuilderX<T>::append ( StringBuilderX<T> && rhs ) {
	if( _size == 0 ) return move( rhs );

	auto old_size = _size;
	resize( old_size+ rhs.size() );
	ArrayUtility::MoveConstructor( dataPtr() + old_size, rhs.dataPtr(), rhs.size() );
	rhs._size = 0;
}


template< typename T > inline
T*	StringBuilderX<T>::onMalloc	( ax_int req_size, ax_int & out_capacity ) {
	//std::cout << this << " onMalloc " << reqSize <<"\n";
	
	auto s = size();
	auto n = ax_max( s + s/2, req_size ); //auto resize to 1.5x times
	auto p = Memory::AllocUncollect<T>( n );
	
	return p;
}

template< typename T > inline
void	StringBuilderX<T>::onFree	( T* p, bool call_from_destructor ) {
	Memory::DeallocUncollect<T>( p );
}


template< typename T > inline
std::ostream& operator<< ( std::ostream& o, const StringBuilderX<T>& v ) {
	o << v.c_str();
	return o;
}

template< typename T, ax_int LOCAL_BUF_SIZE > inline
std::ostream& operator<< ( std::ostream& o, const StringBuilderX_< T, LOCAL_BUF_SIZE >& v ) {
	o << v.c_str();
	return o;
}


}} //namespace

#endif
