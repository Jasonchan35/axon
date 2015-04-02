//
//  FileStream.cpp
//  ax_core
//
//  Created by Jason Chan on 12/2/14.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include <ax/core/io/FileStream.h>
#include <ax/core/string/UtfConverter.h>

#if ax_OS_Unix
	#include <sys/stat.h>
#endif

namespace ax {
namespace System {
namespace IO {

ax_ImplObject( FileStream );

#if 0
#pragma mark ================= Common ====================
#endif

FileStream::~FileStream() {
	close();
}

void FileStream::_handleCheck() const {
	if( ! isOpened() ) throw Err_File_Handle();
}

String		FileStream::readAllText() {
#if ax_char_define_type == 'a'
	return readAllUtf8();
#else

	TempString	buf;
	readAllTextToBuffer( buf );
	return buf.to_string();

#endif
}

StringA		FileStream::readAllUtf8() {
	_handleCheck();
	
	FileSize cur = getPos();
	FileSize file_size = getFileSize();
	
	ax_int remain;
	if( ! ax_safe_assign( remain, file_size - cur ) ) throw Err_Undefined();
	if( remain < 0 ) throw Err_Undefined();

	try{
		StringA::Buffer	buf( remain );
				
		readBytes( buf.data, remain );
		
		return StringA( buf );

	}catch(...) {
		throw;
	}
}

void FileStream::readAllUtf8ToBuffer	( MutStringA & buf ) {
	_handleCheck();
	
	FileSize cur = getPos();
	FileSize file_size = getFileSize();
	
	ax_int size;
	if( ! ax_safe_assign( size, file_size ) ) throw Err_Undefined();

	try{
		buf.resize( size );
				
		setPos( 0 );
		readBytes( buf.dataPtr(), size );
		setPos( cur );

	}catch(...) {
		buf.clear();
		throw;
	}
}

void FileStream::readAllTextToBuffer	( MutString & buf ) {
#if ax_char_define_type == 'a'
	readAllUtf8ToBuffer( buf );
#else
	TempStringA	tmp;
	readAllUtf8ToBuffer( tmp );
	buf.assignUtf( tmp );
#endif
}

void FileStream::writeText ( const String & buf ) {
#if ax_char_define_type == 'a'
	writeUtf8( buf );
#else
	TempStringA tmp;
	tmp.assignUtf( buf );
	writeBytes( tmp.c_str(), tmp.size() );
#endif
}

void FileStream::writeUtf8 ( const StringA & buf ) {
	writeBytes( buf.c_str(), buf.size() );
}

void FileStream::writeUtf8 ( const MutStringA & buf ) {
	writeBytes( buf.c_str(), buf.size() );
}

void FileStream::readAllBytesToBuffer	( ByteArray & buf ) {
	FileSize cur = getPos();
	
	FileSize	filesize = getFileSize();
	ax_int size;
	if( ! ax_safe_assign( size, filesize ) ) throw Err_Undefined();
	
	buf.resize( size );
	
	try{
		setPos( 0 );
		readBytes( buf.dataPtr(), size );
		setPos( cur );
	}catch(...){
		buf.clear();
		throw;
	}
}

#if 0
#pragma mark ================= Unix ====================
#endif
#ifdef ax_OS_Unix

FileStream::FileStream() {
	h_ = -1;
}

bool FileStream::isOpened() const {
	return h_ != -1;
}

void FileStream::close() {
	if( isOpened() ) {
		int ret = ::close( h_ );
		assert( ret == 0 );
		h_ = -1;
		ret = 0; // just used the var to avoid the warrning
	}
}

void FileStream::flush () {
	int b = fsync( h_ );
	if( b != 0 ) throw Err_Undefined();
}

Time::Timestamp	FileStream::creationTime	( ) {
	_handleCheck();
	 struct stat info;
	if( 0 != fstat( h_, &info ) ) throw Err_Undefined();
	Time::Timestamp t;
	t.set_timespec( info.st_birthtimespec );
	return t;
}

Time::Timestamp	FileStream::lastAccessTime 	() {
	_handleCheck();
	struct stat info;
	if( 0 != fstat( h_, &info ) ) throw Err_Undefined();
	Time::Timestamp t;
	t.set_timespec( info.st_atimespec );
	return t;
}

Time::Timestamp	FileStream::lastWriteTime	( ) {
	_handleCheck();
	 struct stat info;
	if( 0 != fstat( h_, &info ) ) throw Err_Undefined();
	Time::Timestamp t;
	t.set_timespec( info.st_mtimespec );
	return t;
}

Time::Timestamp	FileStream::lastStatusChangeTime ( ) {
	_handleCheck();
	 struct stat info;
	if( 0 != fstat( h_, &info ) ) throw Err_Undefined();
	Time::Timestamp t;
	t.set_timespec( info.st_ctimespec );
	return t;
}

void FileStream::setPos		( FileSize  n ) {
	_handleCheck();
	off_t ret = lseek( h_, n, SEEK_SET );
	if( ret == -1 ) throw Err_Undefined();
}

void FileStream::advPos		( FileSize  n ) {
	_handleCheck();
	off_t ret = lseek( h_, n, SEEK_CUR );
	if( ret == -1 ) throw Err_Undefined();
}

void FileStream::setPosEnd	( FileSize  n ) {
	_handleCheck();
	off_t ret = lseek( h_, n, SEEK_END );
	if( ret == -1 ) throw Err_Undefined();
}

FileSize FileStream::getPos () {
	_handleCheck();
	FileSize n = 0;
	off_t ret = lseek( h_, n, SEEK_SET );
	if( ret == -1 ) throw Err_Undefined();
	return FileSize( ret );
}

FileSize FileStream::getFileSize() {
	_handleCheck();
	FileSize out = 0;
	
	off_t curr, r; 
	
	curr = lseek( h_, 0, SEEK_CUR );	if( curr == -1 ) throw Err_Undefined();
	r = lseek( h_, 0, SEEK_END );		if( r == -1 ) throw Err_Undefined();
	out = r;
	r = lseek( h_, curr, SEEK_SET );	if( r == -1 ) throw Err_Undefined();
	
	return FileSize( out );
}

void FileStream::setFileSize( FileSize newSize ) {
	_handleCheck();
	
	FileSize	oldPos = getPos();
	
	off_t o = newSize;
	if( 0 != ftruncate( h_, o ) ) throw Err_Undefined();
	
	if( oldPos < newSize ) setPos( oldPos );
}

void FileStream::readBytes ( void* buf, ax_int byteSize ) {
	_handleCheck();
	
	if( byteSize == 0 ) return;
	auto ret = ::read( h_, buf, byteSize );
	if( ret == -1 ) throw Err_Undefined();
}

void FileStream::writeBytes		( const void* buf, ax_int byteSize ) {
	_handleCheck();
	
	if( byteSize == 0 ) return;
	auto ret = ::write( h_, buf, byteSize );
	if( ret == -1 ) throw Err_Undefined();
}

//----- open file
void FileStream::_os_open( const String & filename, int access_flag ) {
	close();
#if axOS_Linux
	access_flag |= O_LARGEFILE;
#endif
	TempStringA	filenameA;
	filenameA.assignUtf( filename );

	h_ = ::open( filenameA.c_str(), access_flag, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
	if( h_ == -1 ) {
		switch( errno ) {
			case EACCES: throw Err_File_AccessDenied();
			case EEXIST: throw Err_File_AlreadyExists();
			case ENFILE: throw Err_File_OpenTooManyFiles();
			case EMFILE: throw Err_File_OpenTooManyFiles();
			default:	 throw Err_File_OpenError();
		}
	}
}

void FileStream::open ( const String & filename, FileMode mode, FileAccess access, FileShareMode share ) {
	int access_flag = 0;
	
	switch( mode ) {
		case FileMode::CreateNew: 		access_flag |= O_CREAT | O_EXCL;	break;
		case FileMode::OpenExists:	break;
		case FileMode::OpenOrCreate: 	access_flag |= O_CREAT;				break;
	}

	switch( access ) {
		case FileAccess::Read:		access_flag |= O_RDONLY;	break;
		case FileAccess::ReadWrite:	access_flag |= O_RDWR;		break;
		case FileAccess::WriteOnly:	access_flag |= O_WRONLY;	break;
	}

	_os_open( filename, access_flag );
}

bool	FileStream::_os_lock( int flags ) {
	_handleCheck();

	int rc;
	/* keep trying if flock() gets interrupted (by a signal) */
	while ((rc = flock( h_, flags)) < 0 && errno == EINTR)
		continue;

	if (rc == -1) return false;
	return true;
}

void	FileStream::lock( bool exclusive ) {
	if( exclusive ) {
		if( ! _os_lock( LOCK_EX ) ) throw Err_File_LockError();
	}else{
		if( ! _os_lock( LOCK_SH ) ) throw Err_File_LockError();
	}
}

bool	FileStream::trylock( bool exclusive ) {
	if( exclusive ) {
		return _os_lock( LOCK_EX | LOCK_NB );
	}else{
		return _os_lock( LOCK_SH | LOCK_NB );
	}
}

void	FileStream::unlock() {
	_os_lock( LOCK_UN );
}

#endif //axOS_Unix

#if 0
#pragma mark ================= Windows ====================
#endif
#ifdef ax_OS_Windows

FileStream::FileStream() {
	h_ = INVALID_HANDLE_VALUE;
}

bool FileStream::isOpened() const {
	return h_ != INVALID_HANDLE_VALUE;
}

void FileStream::close() {
	if( isOpened() ) {
		BOOL ret = ::CloseHandle( h_ );
		assert( ret );
		h_ = INVALID_HANDLE_VALUE;
	}
}

void FileStream::flush () {
	BOOL b = FlushFileBuffers( h_ );
	if( ! b ) throw ax_exception_unknown();
}

axTimeStamp	FileStream::lastAccessTime 	() {
	_handleCheck();

	axTimeStamp  t;
	BY_HANDLE_FILE_INFORMATION info;
	if( ! GetFileInformationByHandle( h_, &info ) ) throw ax_exception_unknown();
	t.setFILETIME( info.ftLastAccessTime );
	return 0;
}

axTimeStamp	FileStream::lastWriteTime	() {
	_handleCheck();

	axTimeStamp t;
	BY_HANDLE_FILE_INFORMATION info;
	if( ! GetFileInformationByHandle( h_, &info ) ) throw ax_exception_unknown();
	t.setFILETIME( info.ftLastWriteTime );
	return t;
}

FileSize FileStream::getPos() {
	_handleCheck();

	 FileSize n;
	LONG low, hi = 0;
	low = SetFilePointer( h_, 0, &hi, FILE_CURRENT );
	int64_t	i64 = low | ( (int64_t)hi << 32 );
	return ax_safe_assign( n, i64 );
}

void FileStream::setPos( FileSize n ) {
	_handleCheck();

	int64_t	i64;
	if( ! ax_safe_assign( i64, n ) ) throw ax_exception_unknown();
	LONG hi = i64 >> 32;
	SetFilePointer( h_, (LONG)i64, &hi, FILE_BEGIN );
}

void FileStream::advPos( FileSize n ) {
	_handleCheck();

	int64_t	i64;
	if( ! ax_safe_assign( i64, n ) ) throw ax_exception_unknown();
	LONG hi = i64 >> 32;
	SetFilePointer( h_, (LONG)i64, &hi, FILE_CURRENT );
}

void FileStream::setPosEnd( FileSize n ) {
	_handleCheck();

	int64_t	i64;
	if( ! ax_safe_assign( i64, n ) ) throw Err_Undefined();
	LONG hi = i64 >> 32;
	SetFilePointer( h_, (LONG)i64, &hi, FILE_END );
}

FileSize FileStream::getFileSize() {
	_handleCheck();

	FileSize n;
	LARGE_INTEGER s;
	BOOL ret = GetFileSizeEx( h_, &s );
	if( ! ret ) return -1;
	if( ! ax_safe_assign( n, s.QuadPart ) )  throw ax_exception_unknown();
	return n;
}

void	FileStream::setFileSize		( FileSize newSize ) {
	_handleCheck();

	FileSize	oldPos = getPos();
	setPos( newSize );
	SetEndOfFile( h_ );

	if( oldPos < newSize ) setPos( oldPos );
}

void FileStream::readMem( void *buf, axSize byteSize ) {
	_handleCheck();

	if( byteSize == 0 ) return;
	DWORD	n;
	if( ! ax_safe_assign( n, byteSize ) ) throw Err_Undefined();
	DWORD	result = 0;
	BOOL ret = ::ReadFile( h_, buf, n, &result, nullptr );
	if( !ret ) {
		DWORD e = GetLastError();
		ax_log_win32_error("FileStream read file", e);
		switch( e ) {
			case ERROR_LOCK_VIOLATION: throw axFile::exception_lock_error();
		}
		throw Err_Undefined();
	}
}

void FileStream::writeMem(const void *buf, axSize byteSize) {
	_handleCheck();

	if( byteSize == 0 ) return;
	DWORD	n;
	if( ! ax_safe_assign( n, byteSize ) ) throw Err_Undefined();
	DWORD	result = 0;
	BOOL ret = ::WriteFile( h_, buf, n, &result, nullptr );
	if( !ret ) {
		throw Err_Undefined();
	}
}

void FileStream::_os_open( const wchar_t* filename, DWORD access_flag, DWORD create_flag ) {
}

void FileStream::open	( const char* filename, axFileMode mode, axFileAccess access, axFileShareMode share ) {
	DWORD	create_flag	= 0;
	DWORD	access_flag = 0;
	DWORD	share_flag	= 0;

	switch( mode ) {
		case axFileMode::CreateNew: 		create_flag |= CREATE_NEW;			break;
		case axFileMode::OpenExists:		create_flag |= OPEN_EXISTING;		break;
		case axFileMode::OpenOrCreate:		create_flag |= CREATE_ALWAYS;		break;
	}

	switch( access ) {
		case axFileAccess::Read:			access_flag |= GENERIC_READ;					break;
		case axFileAccess::ReadWrite:		access_flag |= GENERIC_READ | GENERIC_WRITE;	break;
		case axFileAccess::WriteOnly:		access_flag |= GENERIC_WRITE;					break;
	}

	switch( share ) {
		case axFileShareMode::Read:			share_flag |= FILE_SHARE_READ;						break;
		case axFileShareMode::ReadWrite:	share_flag |= FILE_SHARE_READ | FILE_SHARE_WRITE;	break;
		case axFileShareMode::Write:		share_flag |= FILE_SHARE_WRITE;						break;
	}
 
	close();

	axTempStringW	wstr( filename );

	h_ = ::CreateFile( wstr, access_flag, share_flag, nullptr, create_flag, FILE_ATTRIBUTE_NORMAL, nullptr );
	if( h_ == INVALID_HANDLE_VALUE ) {
		DWORD err = GetLastError();
//		ax_log_win32_error( "File_open", err );
		switch( err ) {
			case ERROR_FILE_EXISTS: throw axFile::exception_already_exists();
		}
		throw Err_Undefined();
	}

}

bool FileStream::_os_lock( DWORD flags ) {
	_handleCheck();

    OVERLAPPED offset;
	ax_memset( offset, 0 );

	const DWORD len = 0xffffffff;
	return 0 != LockFileEx( h_, flags, 0, len, len, &offset );
}

void FileStream::lock( bool exclusive ) {
	if( exclusive ) {
		if( ! _os_lock( LOCKFILE_EXCLUSIVE_LOCK ) ) throw axFile::exception_lock_error();
	}else{
		if( ! _os_lock( 0 ) ) throw axFile::exception_lock_error();
	}
}

bool FileStream::trylock( bool exclusive ) {
	if( exclusive ) {
	    return _os_lock( LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY );
	}else{
	    return _os_lock( LOCKFILE_FAIL_IMMEDIATELY );
	}
}

void FileStream::unlock() {
	_handleCheck();

    DWORD len = 0xffffffff;
	OVERLAPPED offset;
	ax_memset( offset, 0 );

	if (!UnlockFileEx( h_, 0, len, len, &offset)) {
		throw axFile::exception_lock_error();
	}
}

#endif //ax_OS_Windows



}}} //namespace