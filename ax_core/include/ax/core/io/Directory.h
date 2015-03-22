//
//  Directory.h
//  ax_core
//
//  Created by Jason Chan on 12/2/14.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_Directory_h
#define ax_core_Directory_h

#include "../array/Array.h"

namespace ax {
namespace System {
namespace IO {

struct Directory : StaticClass {

	static	String	GetCurrent();
	static	void	SetCurrent( const String & path );

	static	Array< String >	GetFiles				( const String & path, bool  recursively = false );
	static	Array< String >	GetFilesWithExtension	( const String & path, const String & file_ext, bool recursively = false );
	static	Array< String >	GetDirectories			( const String & path, bool  recursively = false );
	static	Array< String >	GetFileSystemEntries	( const String & path, bool  recursively = false );
	
	static	Array< String > & GetFileSystemEntries (  Array< String > & buf, const String & path, bool recursively = false, bool need_file=true, bool need_dir=true );

};

}}} //namespace



#endif
