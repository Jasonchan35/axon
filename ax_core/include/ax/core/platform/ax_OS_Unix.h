#pragma once
#ifndef __ax_os_unix_h__
#define __ax_os_unix_h__

//! \ingroup base_platform
//@{


#ifdef ax_OS_UNIX

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <wctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <dirent.h>

#define ax_EXE_SUFFIX ""
#define	ax_SO_SUFFIX  ".so"

#define ax_RELEASE_ASSERT(_Expression) \
	{ if(!(_Expression) ) { printf("%s:%u: failed assertion `%s'\n", __FILE__, __LINE__, #_Expression); abort(); }}

#ifdef	_DEBUG
	#define ax_ASSERT(_Expression) ax_RELEASE_ASSERT( _Expression )
#else
	#define	ax_ASSERT(_Expression)	//nothing
#endif

#endif //ax_OS_UNIX

//@}
#endif //__ax_os_unix_h__

