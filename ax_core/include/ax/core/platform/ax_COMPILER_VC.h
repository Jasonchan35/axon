#ifndef ax_core_Compiler_VisualC_h
#define ax_core_Compiler_VisualC_h

#ifdef _MSC_VER

#include <cstdint>

#define	ax_COMPILER_VC	1

#define ax_DLL_EXPORT		_declspec (dllexport)
#define ax_DLL_IMPORT		_declspec (dllimport)


#define	ax_FUNC_NAME			__FUNCTION__
#define ax_PRETTY_FUNC_NAME		__FUNCSIG__

#define ax_DEPRECATED( f )		__declspec( deprecated ) f
#define ax_PACKED_ALIGN( f )	__declspec(align(1)) f

#ifdef _DEBUG
	#define	ax_ALWAYS_INLINE( f )	f
#else
	#define	ax_ALWAYS_INLINE( f )	__forceinline f
#endif

#define ax_THREAD_LOCAL	__declspec( thread )

#if _MSC_VER < 1600
	#define	nullptr	NULL
#endif

//cpu
#ifdef _M_X64
	#define ax_CPU_x86_64      1
#elif defined( _M_PPC )
	#define ax_CPU_Powerpc     1
#else
	#define ax_CPU_x86         1
#endif

#ifdef _M_IX86_FP
	#define ax_CPU_SSE2		  1
#endif

//os

#ifdef _WIN64
	#define ax_OS_WIN64     1
	#define ax_OS_Win64     1

#elif defined _WIN32
	#define ax_OS_WIN32     1
	#define ax_OS_Win32     1

#elif defined _WINCE
	#define ax_OS_WinCE     1
	#define ax_OS_WinCE     1

#endif



#endif //_MSC_VER
#endif //ax_core_Compiler_VisualC_h
