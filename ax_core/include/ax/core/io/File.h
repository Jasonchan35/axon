//
//  File.h
//  ax_core
//
//  Created by Jason Chan on 12/2/14.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_File_h
#define ax_core_File_h

#include "../String/String.h"
#include "../Array/Array.h"

#include "../Object.h"
#include "../Time/TimeStamp.h"

namespace ax {
namespace System {
namespace IO {

enum class FileMode {
	CreateNew,
	OpenExists,
	OpenOrCreate,
};

enum class FileAccess {
	Read,
	ReadWrite,
	WriteOnly,
};

enum class FileShareMode {
	None,
	Read,
	Write,
	ReadWrite,
};

typedef	Int64	FileSize;


struct File : public StaticClass {

	static	String		ReadAllText			( const String & filename );
	static	void		ReadAllTextToBuffer	( String  & buf, const String & filename );

	static	StringA		ReadAllUtf8			( const String & filename );
	static	void		ReadAllUtf8ToBuffer	( IStringA & buf, const String & filename );

	static	void		WriteAllText		( const String & filename, const String  & text );
	static	void		WriteAllUtf8		( const String & filename, const IStringA & text );
};


}}} //namespace

#endif
