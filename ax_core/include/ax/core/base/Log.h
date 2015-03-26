//
//  Log.h
//  ax_core
//
//  Created by Jason on 2015-03-19.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_Log_h
#define ax_core_Log_h

#include "../string/StringFormat.h"

#define ax_log_(...)	ax::System::Logger::LogFormat< ax_char >( __VA_ARGS__ )

#define	ax_log_macro1( fmt ) 		ax_log_( ax_txt(fmt) )
#define	ax_log_macro2( fmt, ... )	ax_log_( ax_txt(fmt), __VA_ARGS__ )
#define	ax_log_macro3( fmt, ... )	ax_log_( ax_txt(fmt), __VA_ARGS__ )
#define	ax_log_macro4( fmt, ... )	ax_log_( ax_txt(fmt), __VA_ARGS__ )
#define	ax_log_macro5( fmt, ... )	ax_log_( ax_txt(fmt), __VA_ARGS__ )
#define	ax_log_macro6( fmt, ... )	ax_log_( ax_txt(fmt), __VA_ARGS__ )
#define	ax_log_macro7( fmt, ... )	ax_log_( ax_txt(fmt), __VA_ARGS__ )
#define	ax_log_macro8( fmt, ... )	ax_log_( ax_txt(fmt), __VA_ARGS__ )
#define	ax_log_macro9( fmt, ... )	ax_log_( ax_txt(fmt), __VA_ARGS__ )

#define ax_log(...)		ax_MACRO_DISPATCHER( ax_log_macro,	__VA_ARGS__ )(__VA_ARGS__)


#define	ax_std_dump_macro1( a0 ) 				{ std::wcout << #a0 <<"="<< (a0) << '\n'; }
#define	ax_std_dump_macro2( a0,a1 ) 			{ std::wcout << #a0 <<"="<< (a0) << ", " << #a1 <<"="<< (a1) << '\n'; }
#define	ax_std_dump_macro3( a0,a1,a2 ) 			{ std::wcout << #a0 <<"="<< (a0) << ", " << #a1 <<"="<< (a1) << #a2 <<"="<< (a2) << '\n'; }
#define	ax_std_dump_macro4( a0,a1,a2,a3 ) 		{ std::wcout << #a0 <<"="<< (a0) << ", " << #a1 <<"="<< (a1) << #a2 <<"="<< (a2) << #a3 <<"="<< (a3) << '\n'; }
#define	ax_std_dump_macro5( a0,a1,a2,a3,a4 ) 	{ std::wcout << #a0 <<"="<< (a0) << ", " << #a1 <<"="<< (a1) << #a2 <<"="<< (a2) << #a3 <<"="<< (a3) << #a4 <<"="<< (a4) << '\n'; }
#define	ax_std_dump_macro6( a0,a1,a2,a3,a4,a5 )	{ std::wcout << #a0 <<"="<< (a0) << ", " << #a1 <<"="<< (a1) << #a2 <<"="<< (a2) << #a3 <<"="<< (a3) << #a4 <<"="<< (a4) << #a5 <<"="<< (a5) << '\n'; }

#define ax_std_dump(...) 	ax_MACRO_DISPATCHER( ax_std_dump_macro,	__VA_ARGS__ )(__VA_ARGS__)
//-------

#define	ax_dump_macro1( a0 )					{ ax_log( "{?}={?}\t\t[#{?}:{?}]", 												ax_txt(#a0),(a0), 																							ax__FILE_BASENAME__, ax__LINE__ ); }
#define	ax_dump_macro2( a0,a1 )					{ ax_log( "{?}={?}, {?}={?}\t\t[#{?}:{?}]", 									ax_txt(#a0),(a0), ax_txt(#a1),(a1), 																		ax__FILE_BASENAME__, ax__LINE__ ); }
#define	ax_dump_macro3( a0,a1,a2 )				{ ax_log( "{?}={?}, {?}={?}, {?}={?}\t\t[#{?}:{?}]", 							ax_txt(#a0),(a0), ax_txt(#a1),(a1), ax_txt(#a2),(a2), 														ax__FILE_BASENAME__, ax__LINE__ ); }
#define	ax_dump_macro4( a0,a1,a2,a3 )			{ ax_log( "{?}={?}, {?}={?}, {?}={?}, {?}={?}\t\t[#{?}:{?}]",					ax_txt(#a0),(a0), ax_txt(#a1),(a1), ax_txt(#a2),(a2), ax_txt(#a3),(a3), 									ax__FILE_BASENAME__, ax__LINE__ ); }
#define	ax_dump_macro5( a0,a1,a2,a3,a4 )		{ ax_log( "{?}={?}, {?}={?}, {?}={?}, {?}={?}, {?}={?}\t\t[#{?}:{?}]", 			ax_txt(#a0),(a0), ax_txt(#a1),(a1), ax_txt(#a2),(a2), ax_txt(#a3),(a3), ax_txt(#a4),(a4), 					ax__FILE_BASENAME__, ax__LINE__ ); }
#define	ax_dump_macro6( a0,a1,a2,a3,a4,a5 )		{ ax_log( "{?}={?}, {?}={?}, {?}={?}, {?}={?}, {?}={?}, {?}={?}\t\t[#{?}:{?}]",	ax_txt(#a0),(a0), ax_txt(#a1),(a1), ax_txt(#a2),(a2), ax_txt(#a3),(a3), ax_txt(#a4),(a4), ax_txt(#a5),(a5), ax__FILE_BASENAME__, ax__LINE__ ); }

#define ax_dump(...)			ax_MACRO_DISPATCHER( ax_dump_macro,	__VA_ARGS__ )(__VA_ARGS__)

#define ax_dump_func_name()		{ std::wcout << "fn " << ax_PRETTY_FUNC_NAME <<"\n"; }

namespace ax {
namespace System {

class Logger : public NonCopyable {
public:

	template< typename T > inline
	static void 	Log( const StringX<T> & msg ) {
//		std::cout <<  msg.c_str() << "\n";
	
		#if   ax_char_define_type == 'a'
			std::cout << msg.c_str() << "\n";
			
		#elif ax_char_define_type == 'w'
			std::wcout << msg.c_str() << "\n";
			
		#else
			TempStringW	tmp;
			tmp.assignUtf( msg );
			std::wcout << tmp.c_str() << "\n";
		
		#endif
	}

	ax_TemplateFunc_Params1( template< typename T >, static void, LogFormat,		   const StringX<T> & /*fmt*/,               StringFormatArg_<T> );
							 template< typename T >  static	void  LogFormat_Params	(  const StringX<T> &   fmt,   const Params< StringFormatArg_<T> > & args ) {
								Log( StringFormat_<T>::ToString_Params( fmt, args ) );
							 }

};

}} //namespace


#endif
