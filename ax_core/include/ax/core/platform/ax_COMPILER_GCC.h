//
//  ax_COMPILER_GCC.h
//  ax_core
//
//  Created by Jason on 2014-11-27.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_ax_COMPILER_GCC_h
#define ax_core_ax_COMPILER_GCC_h

#ifdef __clang__ 
	#define	ax_COMPILER_CLANG	1
#endif 

#ifdef __GNUC__
	#define	ax_COMPILER_GCC	1
#endif 

#if ax_COMPILER_CLANG | ax_COMPILER_GCC


#if __GNUC__ >= 4
	#define	ax_DLL_EXPORT			__attribute__ ((visibility ("default")))
	#define ax_DLL_IMPORT			__attribute__ ((visibility ("hidden")))
#else
	#define	ax_DLL_EXPORT
	#define ax_DLL_IMPORT
#endif

#define nullptr	NULL

#define	ax_FUNC_NAME			__FUNCTION__
#define ax_PRETTY_FUNC_NAME		__PRETTY_FUNCTION__

#define ax_DEPRECATED( f )		f __attribute__( (deprecated) )
#define ax_PACKED_ALIGN( f )	f __attribute__( (packed) )

#ifdef _DEBUG
	#define	ax_ALWAYS_INLINE( f )	f
#else
	#define	ax_ALWAYS_INLINE( f )	f __attribute__( (always_inline) )
#endif


#define ax_THREAD_LOCAL	__thread


#if defined(__x86_64__)
	#define ax_CPU_x86_64      1
	
#elif defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)
	#define ax_CPU_x86         1
	
#elif defined(__POWERPC__)
	#define ax_CPU_PowerPC     1
	
#elif defined(__arm__)
	#define ax_CPU_ARM			1
	
	#ifdef __ARMEL__
		#define ax_CPU_LITTLE_ENDIAN	1
	#elif defined( __ARMEB__ )
		#define ax_CPU_BIG_ENDIAN	1
	#endif
	
	#ifdef	__ARM_NEON__
		#define ax_CPU_ARM_NEON
	#endif
	
#endif


#ifdef __SSE__
	#define ax_CPU_SSE1			1
#endif

#ifdef __SSE2__
	#define ax_CPU_SSE2			1
#endif

//os

#ifdef __linux
	#define ax_OS_Linux        1

	#ifdef __ANDROID__
		#define ax_OS_Android		1
	#else
		#define ax_X11             1
	#endif

#elif defined (__FreeBSD__)
	#define ax_OS_FreeBSD		1
	#define ax_X11				1

#elif defined (__APPLE__) && defined(__MACH__)
	#include <TargetConditionals.h>
	#if (TARGET_OF_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
		#define ax_OS_iOS		1
	#else
		#define ax_OS_MacOSX		1
	#endif

#elif defined (__sun)
	#define ax_OS_Solaris		1
	#define ax_X11				1

#elif defined(__MINGW32__)
	#define ax_OS_MinGW			1
#endif



#endif //ax_COMPILER_CLANG | ax_COMPILER_GCC

#endif
