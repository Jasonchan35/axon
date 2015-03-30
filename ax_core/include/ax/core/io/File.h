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

#include "../base/Obj.h"
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

typedef	int64_t	FileSize;


struct File : public StaticClass {

	static	bool 		Exists 				( const ax_string & filename );
	static	void		Remove				( const ax_string & filename );

	static	void		Rename				( const ax_string & old_name, const ax_string & new_name );

	static	String		ReadText			( const ax_string & filename );
	static	void		ReadTextToBuffer	( MutString  & buf, const ax_string & filename );

	static	StringA		ReadUtf8			( const ax_string & filename );
	static	void		ReadUtf8ToBuffer	( MutStringA & buf, const ax_string & filename );

	static	void		WriteText			( const ax_string & filename, const ax_string	 & text );
	static	void		WriteUtf8			( const ax_string & filename, const StringA  	 & text );
	static	void		WriteUtf8			( const ax_string & filename, const MutStringA   & text );
};


}}} //namespace

#endif
