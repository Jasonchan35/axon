//
//  Type.h
//  ax_core
//
//  Created by Jason Chan on Mar 18, 2015.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_Type_h
#define ax_core_Type_h

#include "../Thread/SpinLock.h"
#include "../string/String.h"

namespace ax {
namespace System {

enum class TypeCode {
	k_null,
	k_bool,
	k_int8,
	k_int16,
	k_int32,
	k_int64,
	k_uint8,
	k_uint16,
	k_uint32,
	k_uint64,
	k_char8,
	k_char16,
	k_char32,
	k_wchar,
	k_float,
	k_double,
	k_Struct,
	k_Object,
	k_Interface,
};

class TypeInfo {
public:
	TypeInfo( const String & name, TypeCode typeCode, const TypeInfo* baseType );

	const	String &		name		() const { return _name; }
	const	TypeInfo*		baseType	() const { return _baseType; }
			TypeCode		typeCode	() const { return _typeCode; }

	ax_ALWAYS_INLINE(	bool	isTypeOf	( const TypeInfo & rhs ) const	) {
		if( this == &rhs ) return true;
		return hasBaseType( rhs );
	}
	
	ax_ALWAYS_INLINE(	bool	hasBaseType	( const TypeInfo & rhs ) const 	) {
		auto t = _baseType;
		while( t ) {
			if( t == &rhs ) return true;
			t = t->_baseType;
		}
		return false;
	}
	
private:
	String					_name;
	TypeCode				_typeCode;
	Ptr< const TypeInfo >	_baseType;
};

class Type {
public:
	Type( const TypeInfo & info ) : _info( info ) {}
	const TypeInfo & _info;
	
	void OnStringReq( ax_ToStringReq & req ) const {
//		req << _info.name();
	}
};

}} //namespace

template< typename T > 		 ax_TypeCode	ax_get_typecode();

template< typename T > inline
const ax_TypeInfo &	ax_get_typeinfo() {
	return T::ax_typeinfo;
}

template< typename T > inline
ax::System::Type	ax_typeof() {
	return ax::System::Type( T::ax_typeinfo );
}

template< typename T > inline
ax::System::Type	ax_typeof( const T & p ) {
	return ax_typeof<T>();
}

#define ax_TYPE_LIST_ITEM( NAME, T ) \
	extern	ax_TypeInfo		ax_TypeInfo_##NAME; \
	template<> inline const ax_TypeInfo & ax_get_typeinfo<T>() { return ax_TypeInfo_##NAME; } \
	template<> inline 		ax_TypeCode	  ax_get_typecode<T>() { return ax_TypeCode::k_##NAME; } \
//----
	ax_TYPE_LIST_all_int
	ax_TYPE_LIST_all_uint
	ax_TYPE_LIST_all_char
	ax_TYPE_LIST_all_bool
#undef ax_TYPE_LIST_ITEM


#endif


