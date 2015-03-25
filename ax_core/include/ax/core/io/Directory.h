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

	static	ArrayObj< String >	GetFileSystemEntries	( const String & path, bool sub_directory, bool hidden_file, const String & file_ext = ax_txt("") );

	static	ArrayObj< String >	GetFiles				( const String & path, bool sub_directory, bool hidden_file, const String & file_ext = ax_txt("") );
	static	ArrayObj< String >	GetDirectories			( const String & path, bool sub_directory, bool hidden_file );
	
	static	bool	Exists			( const String & path );
	static	void	Create			( const String & path, bool sub_directory = true );
	static	void	Remove			( const String & path, bool sub_directory );
	static	void	RemoveIfExists	( const String & path, bool sub_directory );
	
};

}}} //namespace



#endif
