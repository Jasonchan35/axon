//
//  Path.h
//  ax_core
//
//  Created by Jason Chan on 12/2/14.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_Path_h
#define ax_core_Path_h

#include "../String/String.h"

namespace ax {
namespace System {
namespace IO {

struct Path : public StaticClass {
	enum { k_max_char = 512 };

	static	const ax_char*	k_seperators;		// both '/' and '\\'
	static	const ax_char	k_seperator_Unix;	// '/'
	static	const ax_char	k_seperator_Windows;	// '\\'
	
	// e.g. ChangeExtension( "a.txt", ".doc" );
	static	ax_string	ChangeExt	( const ax_string & path, const ax_string & new_extension );
	
	static	ax_string	GetFileName	( const ax_string & path, bool with_extension );
	
	// e.g. return '.txt'
	static	ax_string	GetExt		( const ax_string	& path );
	
	static	ax_string	GetDirName	( const ax_string & path );
};


}}} //namespace


#endif
