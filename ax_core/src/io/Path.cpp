//
//  Path.cpp
//  ax_core
//
//  Created by Jason Chan on 12/2/14.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include <ax/core/io/Path.h>

namespace ax {
namespace System {
namespace IO {

const ax_char*	Path::k_seperators			= ax_sz("/\\");
const ax_char	Path::k_seperator_Unix		= ax_ch('/');
const ax_char	Path::k_seperator_Windows	= ax_ch('\\');


ax_string	Path::GetDirectoryName( const ax_string & path ) {
	auto sz = path.c_str();
	auto p  = ax_strrchr_list( sz, ax_sz("\\/") );
	if( !p ) return ax_string::Clone_c_str( sz );

	return ax_string::Clone( sz, p-sz );
}

ax_string	Path::GetFileName	( const ax_string & path, bool with_extension ) {
	auto sz = path.c_str();
	auto p = ax_strrchr_list( sz, k_seperators );
	if( !p ) {
		p = sz;
	}else{
		p++;
	}
	
	const ax_char* e = nullptr;
	if( ! with_extension ) {
		e = ax_strrchr( p, (ax_char)'.' );
	}
	
	if( e ) {
		return ax_string::Clone( p, e-p );
	}
	
	return ax_string::Clone_c_str( p );
}

ax_string	Path::GetExtension	( const ax_string	& path ) {
	auto sz = path.c_str();
	auto p = ax_strrchr( sz, (ax_char)'.');
	if( !p ) {
		return String();
	}
	return ax_string::Clone_c_str( p );
}

ax_string	Path::ChangeExtension	( const ax_string & path, const ax_string & new_extension ) {
	auto sz = path.c_str();
	auto p = ax_strrchr( sz,(ax_char)'.');
	if( !p ) {
		return path + new_extension;
	}else{
		return ax_string::Clone( sz, p-sz+1 ) + new_extension;
	}
}


}}} //namespace