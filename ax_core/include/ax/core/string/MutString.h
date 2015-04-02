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

const ax_int MutString_default_LOCAL_BUF_SIZE = 40; // LOCAL_BUF_SIZE default is 40 to ensure large enough for GUID/UUID

//! Mutable String Interface
template< typename T >
class MutStringX : public Object {
	typedef	StringX<T>		base;
public:
	struct	ax_type_on_gc_trace : public std::true_type {};

	virtual	~MutStringX() {}

	ax_ALWAYS_INLINE(  void	clear()		);
	ax_ALWAYS_INLINE(  void	release()	);
	
	
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

						void		move		( MutStringX<T> &  rhs ) { onMove( rhs ); }

	bool	operator==( const MutStringX & rhs ) const {
		if( _data == rhs._data && _size == rhs._size ) return true;
		return ax_str_equals( _data, rhs._data );
	}
//---------------
	ax_ALWAYS_INLINE(	void		operator=	( const StringX<T> &  rhs 		) )				{ clear(); append( rhs ); }
	ax_ALWAYS_INLINE(	void		operator=	( const MutStringX &  rhs 		) )				{ clear(); append( rhs ); }
	ax_ALWAYS_INLINE(	void		operator=	(		MutStringX && rhs 		) ) 			{ move(rhs); }

	ax_ALWAYS_INLINE( 	void		assign			( const T& ch ) )							{ clear(); append( ch );  }
	ax_ALWAYS_INLINE( 	void		assignRepeat	( const T& ch, ax_int repeat ) )			{ clear(); appendRepeat( ch, repeat );  }
	
	ax_ALWAYS_INLINE( 	void		assign			( const StringX<T>    &  rhs ) )			{ clear(); append( rhs ); }
	ax_ALWAYS_INLINE( 	void		assign			( const MutStringX<T> &  rhs ) )			{ clear(); append( rhs ); }
	ax_ALWAYS_INLINE( 	void		assign			(       MutStringX<T> && rhs ) )			{ move(rhs); }
	
	ax_ALWAYS_INLINE( 	void		assign			( const T* data, ax_int data_size ) )		{ clear(); append( data, data_size ); }
	
	template< typename R > inline
	MutStringX &	operator << ( const R & rhs ) { append( rhs ); return *this; }
	
	ax_ALWAYS_INLINE( 	void		append			( const T& ch ) );
	ax_ALWAYS_INLINE( 	void		append			( const StringX<T>    &  rhs ) )			{ _append( rhs.c_str(), rhs.size(), 1 ); }
	ax_ALWAYS_INLINE( 	void		append			( const MutStringX<T> &  rhs ) )			{ _append( rhs.c_str(), rhs.size(), 1 ); }
	ax_ALWAYS_INLINE( 	void		append			(       MutStringX<T> && rhs ) );
	
	ax_ALWAYS_INLINE( 	void		append			( const T* data, ax_int data_size ) )		{ _append( data, data_size, 1 ); }
	
	ax_ALWAYS_INLINE( 	void		appendRepeat	( const T& ch, ax_int repeat ) );
	ax_ALWAYS_INLINE( 	void		appendRepeat	( const StringX<T> & s, ax_int repeat ) ) 	{ _append( s.c_str(), s.size(), repeat ); }
	

	#define ax_TYPE_LIST_ITEM( NAME, UTF ) \
		ax_ALWAYS_INLINE(	void	assignUtf			( const UTF& ch )									)	{ clear(); appendUtf( ch ); } \
		ax_ALWAYS_INLINE(	void	assignUtf			( const StringX<UTF> & s )							)	{ clear(); appendUtf( s ); } \
		ax_ALWAYS_INLINE(	void	assignUtf			( const MutStringX<UTF> & s )						)	{ clear(); appendUtf( s ); } \
		ax_ALWAYS_INLINE(	void	assignUtf			( const UTF* data, ax_int data_size )				)	{ clear(); appendUtf( data, data_size ); } \
		ax_ALWAYS_INLINE(	void	assignUtfRepeat		( const UTF& ch, ax_int repeat )					)	{ clear(); appendUtfRepeat( ch, repeat ); } \
		\
		ax_ALWAYS_INLINE(	void	appendUtf			( const UTF& ch  ) 									)	{ _appendUtf( &ch, 1, 1 ); } \
		ax_ALWAYS_INLINE(	void	appendUtf			( const StringX<UTF> & s ) 							)	{ _appendUtf( s.c_str(), s.size(), 1 ); } \
		ax_ALWAYS_INLINE(	void	appendUtf			( const MutStringX<UTF> & s ) 						)	{ _appendUtf( s.c_str(), s.size(), 1 ); } \
		ax_ALWAYS_INLINE(	void	appendUtf			( const UTF* data, ax_int data_size )				)	{ _appendUtf( data, data_size, 1 ); } \
		\
		ax_ALWAYS_INLINE(	void	appendUtfRepeat		( const UTF& ch, ax_int repeat )					)	{ _appendUtf( &ch, 1, repeat ); } \
		ax_ALWAYS_INLINE(	void	appendUtfRepeat		( const StringX<UTF> & s, ax_int repeat ) 			)	{ _appendUtf( s.c_str(), s.size(), repeat ); } \
		ax_ALWAYS_INLINE(	void	appendUtfRepeat		( const MutStringX<UTF> & s, ax_int repeat ) 		)	{ _appendUtf( s.c_str(), s.size(), repeat ); } \
		ax_ALWAYS_INLINE(	void	appendUtfRepeat		( const UTF* data, ax_int data_size, ax_int repeat ))	{ _appendUtf( data, data_size, repeat ); } \
		\
		ax_ALWAYS_INLINE(	void	append_static_cast	( const StringX<UTF> & s ) 							)	{ _append_static_cast( s.c_str(), s.size(), 1 ); } \
		ax_ALWAYS_INLINE(	void	append_static_cast	( const MutStringX<UTF> & s ) 						)	{ _append_static_cast( s.c_str(), s.size(), 1 ); } \
		ax_ALWAYS_INLINE(	void	append_static_cast	( const UTF* data, ax_int data_size ) 				)	{ _append_static_cast( data, data_size, 1 ); } \
	//-------------
		ax_TYPE_LIST_all_char
	#undef ax_TYPE_LIST_ITEM
	
	ax_int	GetHash() const { return ax_c_str_hash(_data); }
	
	StringX<T>	to_StringX() 	{ return StringX<T>::Clone( c_str(), size() ); }

	StringA		to_StringA() 	{ return StringA::CloneUtf( c_str(), size() ); }
	
	String		to_string()		{ return String::CloneUtf( c_str(), size() ); }
	
	template< typename R >
	ax_ALWAYS_INLINE(	void	OnStringReq( ToStringReq_<R> & req ) const );
	
	virtual	bool	isDataOnHeap	() const { return true; }
	
	T*			_data;
	ax_int		_size;
	ax_int		_capacity;
	
protected:
	MutStringX() : _data(nullptr), _size(0), _capacity(0) {}
	
	virtual	void	onMove			( MutStringX<T> &  rhs );
	
	virtual	T*		onMalloc		( ax_int req_size, ax_int & capacity ) = 0;
	virtual	void	onFree			( T* p ) = 0;
	
			void	_do_reserve		( ax_int newSize );
	
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

template< typename T, ax_int LOCAL_BUF_SIZE = MutString_default_LOCAL_BUF_SIZE >
class MutStringX_ : public MutStringX<T>, private LocalBuffer< T, LOCAL_BUF_SIZE > {
	typedef	MutStringX<T>	base;
	typedef	LocalBuffer< T, LOCAL_BUF_SIZE >	BUF;	
public:
	virtual ~MutStringX_() { base::release(); }

	MutStringX_	() {}
	MutStringX_	( MutStringX<T>    && rhs ) { base::move(rhs); }
	void operator=	( MutStringX<T>    && rhs ) { base::move(rhs); }
	
	template< typename UTF >
	MutStringX_		( const StringX<UTF> & s ) { base::appendUtf(s); }

	template< typename UTF >
	MutStringX_		( const MutStringX<UTF> & s ) { base::appendUtf(s); }
	
	virtual	bool	isDataOnHeap	() const { return base::dataPtr() != nullptr && base::dataPtr() != getLocalBufferPtr(); }
	
	virtual	void	onMove( MutStringX<T> & rhs ) {
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
	
	MutStringX<T> &		as_MutString() { return static_cast< MutStringX<T> >( *this ); }

	virtual	const	T*	getLocalBufferPtr() const { return BUF::localBufPtr(); }
	
protected:
	virtual	T*		onMalloc	( ax_int req_size, ax_int & out_capacity ) {
		//std::cout << this << " onMalloc " << reqSize <<"\n";
	
		if( req_size+1 <= LOCAL_BUF_SIZE ) { // +1 for string null terminator
			out_capacity = LOCAL_BUF_SIZE;
			return BUF::localBufPtr();
		}else{
			auto s = base::size();
			auto n = ax_max( s + s/2, req_size ); //auto resize to 1.5x times
			auto p = Memory::AllocUncollect<T>( n + 1 ); // +1 for string null terminator
			out_capacity = n;
			return p;
		}
	}
	
	virtual	void	onFree		( T* p ) {
		if( p != BUF::localBufPtr() ) {
			Memory::DeallocUncollect<T>( p );
		}
	}
};

typedef	MutStringX< ax_char >	MutString;
typedef MutStringX< char >		MutStringA;

template< ax_int LOCAL_BUF_SIZE = MutString_default_LOCAL_BUF_SIZE > using MutString_ = MutStringX_< ax_char, LOCAL_BUF_SIZE >;

template< typename T >	using TempString_ = MutStringX_<T,200>;
typedef	TempString_< ax_char >	TempString;

typedef	TempString_< char >		TempStringA;
typedef	TempString_< wchar_t >	TempStringW;


//-----------
template< typename T > inline
void 	MutStringX<T>::clear() {
	resize(0);
}

//-----------
template< typename T > inline
void 	MutStringX<T>::release() {
	clear();
	if( _data ) {
		onFree( _data );
		_data = nullptr;
		_capacity = 0;
	}
}

//-----------
template< typename T > inline
void 	MutStringX<T>::resize( ax_int new_size ) {
	if( new_size < 0 ) throw Err_Undefined();
	auto old_size = _size;
	if( new_size == old_size ) return;
	
	if( new_size < old_size ) {
		auto dst = _data    + new_size;
		auto n   = old_size - new_size;
		
		ax_bzero( dst, n * sizeof(T) );
		
	}else{
		reserve( new_size );
	}
	_size = new_size;
	
	if( _data ) {
		_data[ new_size ] = 0;
	}
}

template< typename T > inline
void	MutStringX<T>::reserve		( ax_int new_size ) {
	if( new_size <= _capacity ) return;
	_do_reserve( new_size );
}

template< typename T > inline
void	MutStringX<T>::onMove	( MutStringX<T> & rhs ) {
	clear();

	resize( rhs.size() );
	ax_memcpy( dataPtr(), rhs.dataPtr(), rhs.size() * sizeof(T) );
	rhs.resize(0);
}

template< typename T > inline
void	MutStringX<T>::append ( MutStringX<T> && rhs ) {
	if( _size == 0 ) return move( rhs );

	auto old_size = _size;
	resize( old_size + rhs.size() );
	ax_memcpy( dataPtr() + old_size, rhs.dataPtr(), rhs.size() * sizeof(T) );
	rhs.resize(0);
}

template< typename T > inline
std::ostream& operator<< ( std::ostream& o, const MutStringX<T>& v ) {
	o << v.c_str();
	return o;
}

template< typename T, ax_int LOCAL_BUF_SIZE > inline
std::ostream& operator<< ( std::ostream& o, const MutStringX_< T, LOCAL_BUF_SIZE >& v ) {
	o << v.c_str();
	return o;
}


}} //namespace

#endif
