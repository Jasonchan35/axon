//
//  platform.h
//  ax_core
//
//  Created by Jason on 2015-03-14.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_platform_h
#define ax_core_platform_h

#define ax_DEBUG_Enumerating	0


#ifdef	_MSC_VER
	#include "ax_COMPILER_VC.h"
#endif

#ifdef __GNUC__
	#include "ax_COMPILER_GCC.h"
#endif



//======== CPU =============
// check CPU define
#if ax_CPU_x86_64 + ax_CPU_x86 + ax_CPU_PowerPC + ax_CPU_ARM!= 1
	#error CPU should be specified
#endif

#if ax_CPU_x86_64
	#define ax_CPU_LP64				1
	#define ax_CPU_LITTLE_ENDIAN	1
	#define ax_CPU_SUPPORT_MEMORY_MISALIGNED	64	
#endif

#if ax_CPU_x86
	#define ax_CPU_LP32				1
	#define ax_CPU_LITTLE_ENDIAN	1
	#define ax_CPU_SUPPORT_MEMORY_MISALIGNED	64
#endif

#if ax_CPU_PowerPC
	#define ax_CPU_LP32				1
	#define ax_CPU_BIG_ENDIAN		1
	#define ax_CPU_SUPPORT_MEMORY_MISALIGNED	64
#endif

#if ax_CPU_ARM
	#define ax_CPU_LP32				1
	#define ax_CPU_SUPPORT_MEMORY_MISALIGNED	8
#endif

#if ax_CPU_LP32 + ax_CPU_LP64 != 1
	#error CPU bits should be specified
#endif


#if ax_CPU_BIG_ENDIAN + ax_CPU_LITTLE_ENDIAN != 1
	#error CPU uint8_t order should be specified
#endif

//======== OS ===============
#if   ax_OS_WIN32   + ax_OS_WIN64 + ax_OS_WINCE \
		+ ax_OS_FreeBSD + ax_OS_Linux + ax_OS_Solaris \
		+ ax_OS_MacOSX  + ax_OS_iOS + ax_OS_MinGW != 1
	#error OS should be specified
#endif

#if ax_OS_Linux
	#include "ax_OS_Linux.h"
#endif

#if ax_OS_iOS
    #include "ax_OS_iOS.h"
#endif

#if ax_OS_MacOSX
	#include "ax_OS_MacOSX.h"
#endif

#if ax_OS_FreeBSD
        #include "ax_OS_FreeBSD.h"
#endif

#if ax_OS_FreeBSD || ax_OS_Linux || ax_OS_Solaris || ax_OS_MacOSX || ax_OS_iOS
	#define	ax_OS_Unix		1
	#define	ax_USE_PTHREAD	1
	#include "ax_OS_UNIX.h"
#endif

#if ax_OS_WIN32 || ax_OS_WIN64 || ax_OS_WINCE || ax_OS_MinGW
	#define ax_OS_Windows	1
    #include "ax_OS_Windows.h"
#endif



//-- C headers ----
#include <math.h>
#include <unistd.h>

//--- C++ headers ------
#include <new>
#include <memory>
#include <exception>
#include <atomic>
#include <iostream>
#include <cstdint>
#include <cassert>
#include <type_traits>


//-------------------
#include "ax_macro.h"
#include "ax_basic_types.h"
#include "ax_safe_assign.h"
#include "ax_utils.h"
#include "ax_atomic.h"

#endif
