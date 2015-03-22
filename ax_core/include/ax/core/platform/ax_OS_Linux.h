#ifndef __ax_os_linux_h__
#define __ax_os_linux_h__

#ifdef ax_OS_Linux

#if ax_OS_Android
    #include "ax_os_android.h"
#else
	#define	ax_GCC_Atomic		1
#endif

#endif //ax_OS_Linux
#endif //__ax_os_linux_h__
