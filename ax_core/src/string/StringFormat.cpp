//
//  StringFormat.cpp
//  ax_core
//
//  Created by Jason on 2015-03-17.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#include <ax/core/string/StringFormat.h>
#include <ax/core/base/Log.h>

namespace ax {
namespace System {

template< typename T >
StringX<T> StringFormat_<T>::ToString_Params( const StringX<T> & fmt, const Params< StringFormatArg_<T> > & args ) {
	TempString_<T>	tmp;
	AppendToBuffer_Params( tmp, fmt, args );
	return tmp.to_StringX();
}

template< typename T >
MutStringX<T> & StringFormat_<T>::AppendToBuffer_Params( MutStringX<T> & buf, const StringX<T> & fmt_str, const Params< StringFormatArg_<T> > & args ) {
	if( fmt_str.size() <= 0 ) return buf;
	
	MutStringX_<T>		tmp_buf;
	
	ToStringReq_<T>	req( tmp_buf, &buf );

	MutStringX_<T>		_str_idx;
	MutStringX_<T>		_str_padding;

	ax_int	curParamIdx = 0;
	
	const T* fmt = fmt_str.c_str();
	const T* p = fmt;
	const T* s = fmt;
	
	while( *p ) {
		if( *p == '{' ) {
			ax_int bracketStart = p-fmt;
		
			buf.append( s, p-s );
			s = p;
			
			++p;
			if( *p == 0 ) throw Err_StringFormat_MissingCloseCurlyBracket( bracketStart );
			
			if( *p == '{' ) {
				buf.append(*p);
				break;
			}

			s = p;
			for( ;; ++p ) {
				if( *p == 0 ) throw Err_StringFormat_MissingCloseCurlyBracket( bracketStart );
				if( *p == ',' || *p == ':' || *p == '}' ) break;
			}
							
			_str_idx.assign( s, p-s );
			s = p;

			_str_padding.clear();
			if( *p == ',' ) {
				++p;
				s = p;
				for( ;; ++p ) {
					if( *p == 0 ) throw Err_StringFormat_MissingCloseCurlyBracket( bracketStart );
					if( *p == ':' || *p == '}' ) break;
				}
				_str_padding.assign( s, p-s );
				s = p;
			}
			
			if( *p == ':' ) {
				++p;
				s = p;
				for( ;; ++p ) {
					if( *p == 0 ) throw Err_StringFormat_MissingCloseCurlyBracket( bracketStart );
					if( *p == '}' ) break;
				}
				req.opt.assign( s, p-s );
				s = p;
			}

			if( *p != '}' ) throw Err_StringFormat_MissingCloseCurlyBracket( bracketStart );
			++p;
			s = p;

			//---------
			req.paramIndex = -1;
			
			if( _str_idx.size() == 0 ) {
				ax_std_dump( fmt );
				throw Err_StringFormat_MissingCloseCurlyBracket( bracketStart );
			}
			
			if( _str_idx[0] == '?' ) {
				req.paramIndex = curParamIdx;
			}else{
				if( ! ax_c_str_to( _str_idx.c_str(), req.paramIndex ) ) {
					throw Err_Undefined();
				}
			}

			if( req.paramIndex < 0 || req.paramIndex >= args.size() ) {
				throw Err_Undefined();
			}
			
			req.padding = 0;
			if( _str_padding.size() > 0 ) {
				if( ! ax_c_str_to( _str_padding.c_str(), req.padding ) ) {
					throw Err_Undefined();
				}
			}
			
			auto & a = args[ req.paramIndex ];
			a->func( req, a->value );
			
			req.tmpBufToFinal();
											
			++curParamIdx;
			
			continue;
		} // *p == '{'
		
		p++;
	}
	
	if( *s ) {
		buf.append( s, p-s );
	}
	
	return buf;
}

#define	ax_TYPE_LIST_ITEM( NAME, T )	\
	/*---- The explicit instantiation ---*/ \
	template struct StringFormat_<T>; \
//--------
	ax_TYPE_LIST_all_char	
#undef	ax_TYPE_LIST_ITEM

}} //namespace

