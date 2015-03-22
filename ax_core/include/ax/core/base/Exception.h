//
//  Exception.h
//  ax_core
//
//  Created by Jason on 2015-03-15.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_Exception_h
#define ax_core_Exception_h

#include "../platform/platform.h"

namespace ax {
namespace System {

class IException 				: public std::exception {};
class Err_Undefined 			: public IException {};
class Err_NotEnoughMemory 		: public IException {};
class Err_BufferOverlapped		: public IException {};

class Err_Array_OutOfRange 		: public IException {};

class Err_StringUTF				: public IException {};

class Err_StringFormat  		: public IException {};

class Err_StringFormat_MissingCloseCurlyBracket : public IException {
public:
	Err_StringFormat_MissingCloseCurlyBracket( ax_int bracketStart ) {
		
	}
};



class Err_Enumerating 			: public IException {};

class Err_Dict_DuplicateKey 	: public IException {};
class Err_Dict_NoSuchKey		: public IException {};


class Err_Directory 			: public IException {};

// File
class Err_File 					: public IException {};

class Err_File_NotFound			: public Err_File {};
class Err_File_Handle			: public Err_File {};
class Err_File_LockError		: public Err_File {};
class Err_File_AccessDenied		: public Err_File {};
class Err_File_AlreadyExists	: public Err_File {};
class Err_File_OpenTooManyFiles	: public Err_File {};
class Err_File_OpenError		: public Err_File {};

}} //namespace


#endif
