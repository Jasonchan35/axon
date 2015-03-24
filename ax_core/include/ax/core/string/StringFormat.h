//
//  StringFormat.h
//  ax_core
//
//  Created by Jason Chan on Mar 17, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_StringFormat_h
#define ax_core_StringFormat_h

#include "ToStringReq.h"
#include "../base/Params.h"

namespace ax {
namespace System {

template< typename T, typename VALUE > inline
ax_ALWAYS_INLINE( static void StringFormatArg_Func( ToStringReq_<T> &req, const void* v ) ) {
	ax_to_string_req( req, *reinterpret_cast< const VALUE* >( v ) );
}

template< typename T >
class StringFormatArg_ {
public:
	template< typename VALUE >
	StringFormatArg_( const VALUE & v ) {
		value = &v;
		func  = StringFormatArg_Func< T, VALUE >;
	}
	const void*	value;
	void (*func)( ToStringReq_<T> & req, const void* v );
};

template< typename T >
struct StringFormat_ : public StaticClass {

	ax_Func_Params1( static StringX<T>,	ToString,  	   		const StringX<T> & /*fmt*/,               StringFormatArg_<T> );
					 static	StringX<T>	ToString_Params ( 	const StringX<T> &   fmt,   const Params< StringFormatArg_<T> > & args );
	
	ax_Func_Params2( static MutStringX<T>&, AppendToBuffer,	    MutStringX<T> & /*buf*/, const StringX<T> & /*fmt*/,               StringFormatArg_<T> );
					 static	MutStringX<T>&  AppendToBuffer_Params( MutStringX<T> &   buf,   const StringX<T> &   fmt,   const Params< StringFormatArg_<T> > & args );
	
	ax_Func_Params2( static MutStringX<T>&, ToBuffer,			    MutStringX<T> & /*buf*/, const StringX<T> & /*fmt*/,               StringFormatArg_<T> );
					 static	MutStringX<T>&  ToBuffer_Params	(   MutStringX<T> &   buf,   const StringX<T> &   fmt,   const Params< StringFormatArg_<T> > & args ) {
					 	buf.clear();
						AppendToBuffer_Params( buf, fmt, args );
						return buf;
					 }
};

//----------------

#define ax_format( fmt, ... )						ax::System::StringFormat_< ax_char >::ToString 		 ( 		ax_txt(fmt), __VA_ARGS__ )
#define ax_format_to_buffer( buf, fmt, ... )		ax::System::StringFormat_< ax_char >::ToBuffer 		 ( buf,	ax_txt(fmt), __VA_ARGS__ )
#define ax_format_append_to_buffer( buf, fmt, ... )	ax::System::StringFormat_< ax_char >::AppendToBuffer ( buf, ax_txt(fmt), __VA_ARGS__ )

typedef	StringFormat_< ax_char >	StringFormat;
typedef StringFormatArg_< ax_char >	StringFormatArg;

}} //namespace

#endif
