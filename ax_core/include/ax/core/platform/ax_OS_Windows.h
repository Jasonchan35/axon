#pragma once
#ifndef __ax_os_win_h__
#define __ax_os_win_h__

//! \ingroup base_platform
//@{
#ifdef ax_OS_Windows

#ifndef UNICODE 
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX // no min and max macro in windows.h

#include <winsock2.h> //winsock2 must include before windows.h to avoid winsock1 define
#include <windows.h>
#include <Mmsystem.h>
#include <time.h>
#include <conio.h>
#include <shellapi.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>


/*
#include "../../external/glew/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
*/

#define	ax_USE_OpenGL        1
#define	ax_USE_OpenGL_ShadingProgram		1


#ifdef ax_COMPILER_VC
	#pragma comment( lib, "ws2_32.lib" )
#endif //ax_COMPILER_VC

#undef	min
#undef	max

#ifdef	_DEBUG
	#define ax_RELEASE_ASSERT(_Expression) \
		{ if(!(_Expression) ) 	{ _wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__); } }
	#define ax_ASSERT(_Expression)	ax_RELEASE_ASSERT(_Expression)
#else
	#define	ax_ASSERT	 //nothing
	#define	ax_RELEASE_ASSERT(_Expression)  \
		{ if(!(_Expression) ) { MessageBoxA( nullptr, #_Expression, nullptr, MB_ICONSTOP ); abort(); } }
#endif

#define ax_EXE_SUFFIX ".exe"
#define	ax_SO_SUFFIX  ".dll"

#endif//ax_OS_Windows

//@}
#endif //__ax_os_win_h__
