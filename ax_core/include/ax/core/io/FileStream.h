//
//  FileStream.h
//  ax_core
//
//  Created by Jason Chan on 12/2/14.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_FileStream_h
#define ax_core_FileStream_h

#include "File.h"

namespace ax {
namespace System {
namespace IO {

class FileStream : public Object {
	ax_DefObject( FileStream, Object )
public:

	void		open			( const String & filename, FileMode mode, FileAccess access, FileShareMode share );
	
	void		openRead		( const String & filename )	{	open( filename, FileMode::OpenExists, 	FileAccess::Read, 	   FileShareMode::Read ); }
	void		openAppend		( const String & filename )	{	open( filename, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShareMode::Read ); setPosEnd(0);	}
	
	void		openWrite		( const String & filename, bool truncate )	{
		open( filename, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShareMode::Read );
		if( truncate ) setFileSize(0);
	}
	
	Time::Timestamp	creationTime 			();
	Time::Timestamp	lastAccessTime 			();
	Time::Timestamp	lastWriteTime			();
	Time::Timestamp lastStatusChangeTime	();

	void		close			();
	bool		isOpened		() const;
	
	void		lock			( bool exclusive );
	bool		trylock			( bool exclusive );
	void		unlock			();

	void		setPos			( FileSize n );
	void		advPos			( FileSize n ); //!< advance position
	void		setPosEnd		( FileSize n ); //!< set position from end
	
	FileSize	getPos			();
	FileSize	getFileSize		();

	void		setFileSize		( FileSize n );
	
//------	
	void		readBytes		(       void* buf, ax_int byte_size );
	void		writeBytes		( const void* buf, ax_int byte_size );
		
	void		writeText		( const String  & s );
	void		writeUtf8		( const StringA & s );
		
	//! read all bytes to end of file
	ax_Obj< ByteArray >	reallAllBytes();
			void		readAllBytesToBuffer( ByteArray    & buf );
	
	void		readAllTextToBuffer	( MutString    & buf );
	void		readAllUtf8ToBuffer	( MutStringA   & buf );
	
	String		readAllText();
	StringA		readAllUtf8();
	
	void		flush			();

	FileStream();
	~FileStream();
	
	
private:
	void		_handleCheck	() const;

#if ax_OS_Win
	HANDLE	h_;
	void _os_open	( const String & filename, DWORD access_flag, DWORD create_flag );
	bool _os_lock	( DWORD flags );
public:
	HANDLE	getHANDLE	()	{ return h_; }
#endif
	
#if ax_OS_Unix
	int		h_;
	void _os_open	( const String & filename, int access_flag );
	bool _os_lock	( int flags );
public:
	int		getFileDescriptor()	{ return h_; }
#endif
	
};

}}} //namespace


#endif
