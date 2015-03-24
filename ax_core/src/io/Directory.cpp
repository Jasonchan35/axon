//
//  Directory.cpp
//  ax_core
//
//  Created by Jason Chan on 12/2/14.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#include <ax/core/io/Directory.h>
#include <ax/core/io/Path.h>

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

bool	Directory::Exists	( const IString & path ) {
	TempStringA	path8;
	path8.assignUtf( path );

	struct stat s;	
	if( 0 != ::stat( path8.c_str(), &s ) ) return false;
	return ( s.st_mode & S_IFDIR ) != 0;
}


void	OS_Directory_Create	( const IString & path )	   {
	TempStringA	path8;
	path8.assignUtf( path );

	auto ret = ::mkdir( path8.c_str(), 0755 );
	if( ret != 0 ) {
		if( errno == EEXIST ) return; //already exists
		throw Exception::ErrUndefined();
	}	
}

void	 OS_Directory_Remove ( const IString & path )    {
	TempStringA	path8;
	path8.assignUtf( path );

	auto ret = ::rmdir( path8.c_str() );
	if( ret != 0 ) {
		throw Exception::ErrUndefined();
	}
}


String	Directory::GetCurrent() {
	char  tmp[ Path::k_max_char + 1 ];
	if( ! ::getcwd( tmp, Path::k_max_char ) ) {
		throw Exception::ErrDirectory();
	}
	tmp[ Path::k_max_char ] = 0;
	
	return String::FromUtf_c_str( tmp );
}

void	Directory::SetCurrent( const IString & path ) {
	TempStringA tmp;
	tmp.assignUtf( path );
	::chdir( tmp.c_str() );
}

static void OS_Directory_GetFileSystemEntries (	IArray< String > & result, const IString & path, bool recursively,
												bool need_file, const IString* file_ext, bool need_dir, bool dotFile )
{

	TempStringA		path_utf8;
	path_utf8.assignUtf( path );

//	DIR*			dir_;
	struct dirent	entry;
	struct dirent*	r = nullptr;
	
	auto h = ::opendir( path_utf8.c_str() );
	if( ! h ) {
		ax_dump( path );
		throw Exception::ErrDirectory();
	}

	try{
		String	name;
	
		for(;;) {
			if( 0 != ::readdir_r( h, & entry, &r ) ) {
				throw Exception::ErrDirectory();
			}
				
			if( ! r ) break;
			
			if( ! dotFile ) {
				if( ax_str_has_prefix( r->d_name, ".") ) {
					continue;
				}
			}
			
			if( ax_strcmp( r->d_name, "."  ) == 0 ) continue;
			if( ax_strcmp( r->d_name, ".." ) == 0 ) continue;

			bool isDir = ( r->d_type & DT_DIR );
			if( isDir ) {
				if( need_dir || recursively ) {
					ax_format_to_string( name, "{?}/{?}", path, String::FromUtf_c_str( r->d_name ) );
					if( need_dir ) result.append( name );
					if( recursively ) OS_Directory_GetFileSystemEntries( result, name, true, need_file, file_ext, need_dir, dotFile );
				}
			}else{
				if( ! need_file ) continue;
				
				ax_format_to_string( name, "{?}/{?}", path, String::FromUtf_c_str( r->d_name ) );
				
				if( file_ext ) {
					auto ext = Path::GetExt( name );
					if( ! file_ext->equals( ext, true ) ) continue;
				}
				
				result.append( name );
			}
		}
	}catch(...){
		::closedir( h );
		throw;
	}
}

#endif // Unix


#if 0
#pragma mark ================= Windows ====================
#endif
#ifdef ax_OS_Windows

static void	OS_Directoy_Remove	( const IString & path ) {
	TempStringW	path_;
	path_.appendUtf( path );
	
	if( 0 != ::_wrmdir( path_.c_str() ) ) {
		throw Exception::ErrUndefined();
	}
}

static	OS_Directory_Create	( const IString & path ) {
	TempStringW	path_;
	path_.appendUtf( path );
	
	if( 0 != ::_wmkdir( path_.c_str() ) ) {
		throw Exception::ErrUndefined();
	}
}

bool	Directory::Exists		( const IString & path ) {
	TempStringW	path_;
	path_.appendUtf( path );
	
	struct stat s;
	if( 0 != ::_wstat( path_.c_str(), &s ) ) return false;
	return ( s.st_mode & S_IFDIR ) != 0;
}

#endif // windows

#if 0
#pragma mark ================= Common ====================
#endif


ax_Obj< Array< String > >	Directory::GetFiles			( const IString & path, bool recursively, bool dotFile )	{
	auto result = ax_new_obj( Array< String > );
	OS_Directory_GetFileSystemEntries( *result, path, recursively, true, nullptr, false, dotFile );
	return result;
}

ax_Obj< Array< String > >	Directory::GetFilesWithExt	( const IString & path, const IString & file_ext, bool recursively, bool dotFile )	{
	auto result = ax_new_obj( Array< String > );
	OS_Directory_GetFileSystemEntries( *result, path, recursively, true, &file_ext, false, dotFile );
	return result;
}

ax_Obj< Array< String > >	Directory::GetDirs			( const IString & path, bool recursively, bool dotFile )	{
	auto result = ax_new_obj( Array< String > );
	OS_Directory_GetFileSystemEntries( *result, path, recursively, false, nullptr, true, dotFile );
	return result;
}

ax_Obj< Array< String > >	Directory::GetFileSystemEntries	( const IString & path, bool recursively, bool dotFile )	{
	auto result = ax_new_obj( Array< String > );
	OS_Directory_GetFileSystemEntries( *result, path, recursively, true, nullptr, true, dotFile );
	return result;
}

void	Directory::RemoveIfExists	( const IString & path, bool recursively ) {
	if( Directory::Exists( path ) ) {
		Directory::Remove( path, recursively );
	}
}

void	Directory::Remove ( const IString & path, bool recursively ) {
	if( ! recursively ) return OS_Directory_Remove( path );

	auto dirList = GetDirs( path, false, true );
	ax_foreach( auto & p, *dirList ) {
		Directory::Remove( p, recursively );
	}
	
	auto fileList = GetFiles( path, false, true );
	ax_foreach( auto & p, *fileList ) {
		File::Remove( p );
	}
	

	OS_Directory_Remove( path );
}

void Directory::Create( const IString & path, bool recursive ) {
	if( ! recursive ) {
		OS_Directory_Create( path );
	}

	const ax_char* p	= path.c_str();
	const ax_char* s 	= p;
	const ax_char* sp	= Path::k_seperators;
	const ax_char* end	= p + path.size();

	TempString token, tmp;
	
	for( ;; ) {
		const ax_char *e = ax_strchr_list( s, sp );
		if( !e ) { 
			e = end;
		}

		ax_int len = e-s;
		if( len > 0 ) {
			token.assign( s, len );
			
			if( s != p ) { //skip the 1st 1
				tmp.append( ax_str("/") );
			}
			
			tmp.append( token );
			OS_Directory_Create( tmp );
			//ax_log("p {?}", tmp );			
		}
				
		if( e >= end ) break;
		s = e+1;
	}
}

	

}}} //namespace
