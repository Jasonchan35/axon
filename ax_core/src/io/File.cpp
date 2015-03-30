//
//  File.cpp
//  ax_core
//
//  Created by Jason Chan on 12/2/14.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include <ax/core/io/File.h>
#include <ax/core/io/FileStream.h>

#if ax_OS_Unix
	#include <dirent.h>
	#include <sys/stat.h>
#endif


namespace ax {
namespace System {
namespace IO {


#if 0
#pragma mark ================= Unix ====================
#endif

#if ax_OS_Unix

void File::Remove	( const ax_string & filename ) {
	TempStringA	tmp;
	tmp.assignUtf( filename );

	auto ret = ::remove( tmp.c_str() );
	if( ret != 0 ) {
		throw Err_Undefined();
	}
}

bool File::Exists ( const ax_string & filename ) {
	TempStringA	tmp;
	tmp.assignUtf( filename );

	struct stat s;
	if( 0 != ::stat( tmp.c_str(), &s ) ) return false;
	return ( s.st_mode & S_IFDIR ) == 0;
}

void File::Rename	( const ax_string & old_name, const ax_string & new_name ) {
	TempStringA	old_name_, new_name_;
	old_name_.assignUtf( old_name );
	new_name_.assignUtf( new_name );

	auto ret = ::rename( old_name_.c_str(), new_name_.c_str() );
	if( ret != 0 ) {
		throw Err_Undefined();
	}
}

#endif


#if 0
#pragma mark ================= Common ====================
#endif

String	File::ReadText			( const ax_string & filename ) {
	FileStream	s;
	s.openRead( filename );
	return s.readAllText();
}

void	File::ReadTextToBuffer	( MutString & buf, const ax_string & filename ) {
	FileStream	s;
	s.openRead( filename );
	s.readAllTextToBuffer( buf );
}

StringA	File::ReadUtf8			( const ax_string & filename ) {
	FileStream	s;
	s.openRead( filename );
	return s.readAllUtf8();
}

void	File::ReadUtf8ToBuffer	( MutStringA & buf, const ax_string & filename ) {
	FileStream	s;
	s.openRead( filename );
	s.readAllUtf8ToBuffer( buf );
}

void	File::WriteText		( const ax_string & filename, const ax_string & text ) {
	FileStream	s;
	s.openWrite( filename, true );
	s.writeText( text );
}

void	File::WriteUtf8		( const ax_string & filename, const StringA & text ) {
	FileStream	s;
	s.openWrite( filename, true );
	s.writeUtf8( text );
}

void	File::WriteUtf8		( const ax_string & filename, const MutStringA & text ) {
	FileStream	s;
	s.openWrite( filename, true );
	s.writeUtf8( text );
}


}}} //namespace