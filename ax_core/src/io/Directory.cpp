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

bool	Directory::Exists	( const String & path ) {
	TempStringA	path8;
	path8.assignUtf( path );

	struct stat s;	
	if( 0 != ::stat( path8.c_str(), &s ) ) return false;
	return ( s.st_mode & S_IFDIR ) != 0;
}


void	OS_Directory_Create	( const String & path )	   {
	TempStringA	path8;
	path8.assignUtf( path );

	auto ret = ::mkdir( path8.c_str(), 0755 );
	if( ret != 0 ) {
		if( errno == EEXIST ) return; //already exists
		throw Err_Undefined();
	}	
}

void	 OS_Directory_Remove ( const String & path )    {
	TempStringA	path8;
	path8.assignUtf( path );

	auto ret = ::rmdir( path8.c_str() );
	if( ret != 0 ) {
		throw Err_Undefined();
	}
}


String	Directory::GetCurrent() {
	char  tmp[ Path::k_max_char + 1 ];
	if( ! ::getcwd( tmp, Path::k_max_char ) ) {
		throw Err_Directory();
	}
	tmp[ Path::k_max_char ] = 0;
	
	return String::CloneUtf_c_str( tmp );
}

void	Directory::SetCurrent( const String & path ) {
	TempStringA tmp;
	tmp.assignUtf( path );
	::chdir( tmp.c_str() );
}

void OS_Directory_GetFileSystemEntries ( Array< String > & result, const String & path, bool sub_directory, bool hidden_file, const String & file_ext, bool need_file, bool need_dir ) {
	TempStringA		path_utf8;
	path_utf8.assignUtf( path );

	struct dirent	entry;
	struct dirent*	r = nullptr;
	
	auto h = ::opendir( path_utf8.c_str() );
	if( ! h ) {
		ax_dump( path );
		throw Err_Directory();
	}

	try{
		for(;;) {
			if( 0 != ::readdir_r( h, & entry, &r ) ) {
				throw Err_Directory();
			}
				
			if( ! r ) break;
			
			if( ! hidden_file ) {
				if( ax_str_has_prefix( r->d_name, ".") ) {
					continue;
				}
			}
			
			if( ax_strcmp( r->d_name, "."  ) == 0 ) continue;
			if( ax_strcmp( r->d_name, ".." ) == 0 ) continue;

			bool isDir = ( r->d_type & DT_DIR );
			if( isDir ) {
				if( need_dir || sub_directory ) {
					auto name = ax_format( ax_txt("{?}/{?}"), path, String::MakeExternal_c_str( r->d_name ) );
					if( need_dir ) {
						result.append( name );
					}
					if( sub_directory ) {
						OS_Directory_GetFileSystemEntries( result, name, sub_directory, hidden_file, file_ext, need_file, need_dir );
					}
				}
			}else{
				if( need_file ) {					
					auto name = ax_format( ax_txt("{?}/{?}"), path, String::MakeExternal_c_str( r->d_name ) );
					
					if( file_ext.size() > 0 ) {
						auto ext = Path::GetExtension( name );
						if( ! file_ext.equals( ext, true ) ) continue;
					}
					
					result.append( name );
				}
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
		throw Err_Undefined();
	}
}

static	OS_Directory_Create	( const IString & path ) {
	TempStringW	path_;
	path_.appendUtf( path );
	
	if( 0 != ::_wmkdir( path_.c_str() ) ) {
		throw Err_Undefined();
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

ArrayObj< String > 	Directory_GetFileSystemEntries ( const String & path, bool sub_directory, bool hidden_file, const String & file_ext, bool need_file, bool need_dir ) {
	auto result = ax_new_obj( Array_< String > );
	OS_Directory_GetFileSystemEntries( *result, path, sub_directory, hidden_file, file_ext, need_file, need_dir );
	return result;
}

ArrayObj< String >	Directory::GetFileSystemEntries	( const String & path, bool sub_directory, bool hidden_file, const String & file_ext ) {
	return Directory_GetFileSystemEntries( path, sub_directory, hidden_file, file_ext, true, true );
}

ArrayObj< String >	Directory::GetFiles			( const String & path, bool sub_directory, bool hidden_file, const String & file_ext  )	{
	return Directory_GetFileSystemEntries( path, sub_directory, hidden_file, file_ext, true, false );
}

ArrayObj< String >	Directory::GetDirectories	( const String & path, bool sub_directory, bool hidden_file ) {
	return Directory_GetFileSystemEntries( path, sub_directory, hidden_file, ax_txt(""), false, true );
}

void	Directory::RemoveIfExists	( const String & path, bool sub_directory ) {
	if( Directory::Exists( path ) ) {
		Directory::Remove( path, sub_directory );
	}
}

void	Directory::Remove ( const String & path, bool sub_directory ) {
	if( ! sub_directory ) return OS_Directory_Remove( path );

	auto dirList = GetDirectories( path, false, true );
	ax_foreach( & p, *dirList ) {
		Directory::Remove( p, sub_directory );
	}
	
	auto fileList = GetFiles( path, false, true );
	ax_foreach( & p, *fileList ) {
		File::Remove( p );
	}

	OS_Directory_Remove( path );
}

void Directory::Create( const String & path, bool sub_directory ) {
	if( ! sub_directory ) {
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
				tmp.append( ax_txt("/") );
			}
			
			tmp.append( token );
			OS_Directory_Create( tmp.to_string() );
			//ax_log("p {?}", tmp );			
		}
				
		if( e >= end ) break;
		s = e+1;
	}
}

	

}}} //namespace
