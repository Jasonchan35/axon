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
	static	String	ChangeExtension	( const String & path, const String & new_extension );
	
	static	String	GetFileName		( const String & path, bool with_extension );
	
	// e.g. return '.txt'
	static	String	GetExtension	( const String	& path );
	
	static	String	GetDirectoryName( const String & path );
};


}}} //namespace


#endif
