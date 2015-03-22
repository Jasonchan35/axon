//
//  ToStringReq.h
//  ax_core
//
//  Created by Jason on 2015-03-17.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_ToStringReq_h
#define ax_core_ToStringReq_h

#include "MutString.h"

namespace ax {
namespace System {

template< typename T >
class ToStringReq_ : public NonCopyable {
public:
	StringBuilderX_< T >	opt;
	ax_int				paramIndex;
	ax_int				padding;
	ax_int				indentLevel;

	ToStringReq_&	indent()	{ if( indentLevel > 0 ) outbuf().appendRepeat( ax_sz("  "), indentLevel ); return *this; }
	ToStringReq_&	newLine() 	{ outbuf().append('\n'); /*indent(); */ return *this; }

	template< typename VALUE >
	ToStringReq_&	operator << ( const VALUE & value );

	ToStringReq_( StringBuilderX<T> & tmp_buf, StringBuilderX<T>* final_buf = nullptr )
	: paramIndex(0)
	, padding(0)
	, indentLevel(0)
	, _tmp_buf( & tmp_buf )
	, _final_buf( final_buf )
	{
	
	}

	StringBuilderX<T> &	outbuf() {
		if( _final_buf && padding == 0 ) return *_final_buf;
		return *_tmp_buf;
	}


	void	tmpBufToFinal() {
		if( ! _final_buf ) return;
		
		auto n = _tmp_buf->size();
		if( padding < 0 ) {
			_final_buf->append( *_tmp_buf );
			_final_buf->appendRepeat( ' ', -padding-n );
		}else{
			_final_buf->appendRepeat( ' ', padding - n );
			_final_buf->append( *_tmp_buf );
		}
		_tmp_buf->clear();
	}

private:
// output to buffer
	StringBuilderX<T>*	_tmp_buf;
	StringBuilderX<T>*	_final_buf;

};

typedef	ToStringReq_< ax_char >		ToStringReq;

}} //namespace

//-----------

template< typename T, typename VALUE >
void ax_to_string_req( ax::System::ToStringReq_<T> & req, const VALUE & v );

template< typename T, typename VALUE > inline
void ax_to_string_req( ax::System::ToStringReq_<T> & req, const VALUE & v ) {
	v.to_string(req);
}

#define ax_TYPE_LIST_ITEM( NAME, VALUE ) \
	template< typename T > inline \
	void ax_to_string_req( ax::System::ToStringReq_<T> & req, const VALUE & v ) { \
		req.outbuf().append( v ); \
	} \
//-------
	ax_TYPE_LIST_all_char
#undef ax_TYPE_LIST_ITEM

template< typename T > void ax_to_string_req( ax::System::ToStringReq_<T> & req, const void* & v );

#define ax_TYPE_LIST_ITEM( NAME, VALUE ) \
	template< typename T > void ax_to_string_req( ax::System::ToStringReq_<T> & req, const VALUE & v ); \
//-----
	ax_TYPE_LIST_all_bool
	ax_TYPE_LIST_all_int
	ax_TYPE_LIST_all_uint
	ax_TYPE_LIST_all_float
	ax_TYPE_LIST_all_char
#undef ax_TYPE_LIST_ITEM


//---------------------------------

namespace ax {
namespace System {

template< typename T >
template< typename VALUE >
ToStringReq_<T> & ToStringReq_<T>::operator << ( const VALUE & value ) {
	ax_to_string_req( *this, value );
	return *this;
}

template< typename C >
template< typename T > inline
void StringX<C>::to_string( ax::System::ToStringReq_<T> & req ) const {
	req.outbuf().appendUtf( *this );
}

}} //namespace


#endif
