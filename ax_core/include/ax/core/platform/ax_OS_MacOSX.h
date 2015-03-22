#ifndef __ax_os_mac_h__
#define __ax_os_mac_h__

//! \ingroup base_platform
//@{
#ifdef ax_OS_MacOSX

//#include "../../external/glew/glew.h"

#if __OBJC__

#import <Foundation/Foundation.h>
#import <CoreServices/CoreServices.h>
#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>

//#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>

//#import <libkern/OSAtomic.h>
//#import <sys/sysctl.h>

#endif // __OBJC__


//#include <dirent.h>
//#include <iostream>

#endif//ax_OS_MacOSX

//@}
#endif //__ax_os_mac_h__
