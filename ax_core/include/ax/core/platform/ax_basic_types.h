//
//  basic_types.h
//  ax_core
//
//  Created by Jason on 2015-03-15.
//  Copyright (c) 2015 Awenix. All rights reserved.
//

#ifndef ax_core_basic_types_h
#define ax_core_basic_types_h

namespace ax {
namespace System {

class StaticClass {
private:
	StaticClass(); // cannot create instance !!!
};

class NonCopyable {
public:
	NonCopyable()	{}

// C+11 delete function
//	NonCopyable 	( const NonCopyable &s ) = delete; //!< not allow by default
//	void operator=	( const NonCopyable &s ) = delete; //!< not allow by default

private:
	NonCopyable 	( const NonCopyable &s ); //!< not allow by default
	void operator=	( const NonCopyable &s ); //!< not allow by default
};
	
template< typename T > class ToStringReq_;

}} //namespace

#define	ax_params			ax::System::Params
#define	ax_ptr				ax::System::Ptr

#define ax_Locked			ax::System::Locked

#define ax_Type				ax::System::Type
#define ax_TypeCode			ax::System::TypeCode
#define ax_TypeInfo			ax::System::TypeInfo

#if ax_CPU_LP64
	typedef	int64_t		ax_int;
	typedef uint64_t	ax_uint;
#elif ax_CPU_LP32
	typedef	int32_t		ax_int;
	typedef	uint32_t	ax_uint;
#else
	#error
#endif

typedef uint32_t	ax_unichar;

#define ax_char_define_type 'a'  // a,u,w

#define ax_sz8(sz)		(u8##sz)
#define ax_sz16(sz)		( u##sz)
#define ax_sz32(sz)		( U##sz)
#define ax_szW(sz)		( L##sz)

#if ax_char_define_type == 'a' // char
	#define ax_sz(sz)	ax_sz8(sz)
	#define ax_str(sz)	ax_str8(sz)
	typedef	char		ax_char;
	
#elif ax_char_define_type == 'u'
	#define ax_sz(sz)	ax_sz16(sz)
	#define ax_str(sz)	ax_str16(sz)
	typedef	char16_t	ax_char;

#elif ax_char_define_type == 'U'
	#define ax_sz(sz)	ax_sz32(sz)
	#define ax_str(sz)	ax_str32(sz)
	typedef	char32_t	ax_char;

#elif ax_char_define_type == 'w'
	#define ax_sz(sz)	ax_szW(sz)
	#define ax_str(sz)	ax_strW(sz)
	typedef	wchar_t		ax_char;

#else
	#error
#endif



// ax_TYPE_LIST_ITEM( NAME, CPP_NAME )

#define	ax_TYPE_LIST_all_bool	\
	ax_TYPE_LIST_ITEM( bool,	bool )		\
//------

#define	ax_TYPE_LIST_all_size_t		\
	ax_TYPE_LIST_ITEM( size_t,	size_t ) \

#define	ax_TYPE_LIST_all_ssize_t	\
	ax_TYPE_LIST_ITEM( ssize_t,	ssize_t ) \

#define	ax_TYPE_LIST_all_char	\
	ax_TYPE_LIST_ITEM( char8,	char )		\
	ax_TYPE_LIST_ITEM( char16,	char16_t )	\
	ax_TYPE_LIST_ITEM( char32,	char32_t )	\
	ax_TYPE_LIST_ITEM( wchar,	wchar_t  )	\
//----

#define	ax_TYPE_LIST_all_int	\
	ax_TYPE_LIST_ITEM( int8,	int8_t   )	\
	ax_TYPE_LIST_ITEM( int16,	int16_t  )	\
	ax_TYPE_LIST_ITEM( int32,	int32_t  )	\
	ax_TYPE_LIST_ITEM( int64,	int64_t  )	\
//----

#define	ax_TYPE_LIST_all_uint	\
	ax_TYPE_LIST_ITEM( uint8,	uint8_t  )	\
	ax_TYPE_LIST_ITEM( uint16,	uint16_t )	\
	ax_TYPE_LIST_ITEM( uint32,	uint32_t )	\
	ax_TYPE_LIST_ITEM( uint64,	uint64_t )	\
//----

#define	ax_TYPE_LIST_all_float	\
	ax_TYPE_LIST_ITEM( float,	float	 )	\
	ax_TYPE_LIST_ITEM( double,	double	 )	\
//----

// -- value min/max ----
template< typename T > 			constexpr T		ax_type_min();
template< typename T > 			constexpr T		ax_type_max();

template< typename T > inline	constexpr T		ax_type_min( const T& v ) { return ax_type_min<T>(); }
template< typename T > inline	constexpr T		ax_type_max( const T& v ) { return ax_type_max<T>(); }

template<> inline constexpr	uint8_t		ax_type_min<uint8_t>()		{ return 0; }
template<> inline constexpr	uint8_t		ax_type_max<uint8_t>()		{ return 0xff; }

template<> inline constexpr	uint16_t	ax_type_min<uint16_t>()		{ return 0; }
template<> inline constexpr	uint16_t	ax_type_max<uint16_t>()		{ return 0xffff; }

template<> inline constexpr	uint32_t	ax_type_min<uint32_t>()		{ return 0; }
template<> inline constexpr	uint32_t	ax_type_max<uint32_t>()		{ return 0xffffffffU; }

template<> inline constexpr	uint64_t	ax_type_min<uint64_t>()		{ return 0; }
template<> inline constexpr	uint64_t	ax_type_max<uint64_t>()		{ return 0xffffffffffffffffULL; }

template<> inline constexpr	int8_t		ax_type_min<int8_t>()		{ return (-0x7f-1); }
template<> inline constexpr	int8_t		ax_type_max<int8_t>()		{ return 0x7f; }

template<> inline constexpr	int16_t		ax_type_min<int16_t>()		{ return (-0x7fff-1); }
template<> inline constexpr	int16_t		ax_type_max<int16_t>()		{ return 0x7fff; }

template<> inline constexpr	int32_t		ax_type_min<int32_t>()		{ return (-0x7fffffff-1); }
template<> inline constexpr	int32_t		ax_type_max<int32_t>()		{ return 0x7fffffff; }

template<> inline constexpr	int64_t		ax_type_min<int64_t>()		{ return (-0x7fffffffffffffffLL-1); }
template<> inline constexpr	int64_t		ax_type_max<int64_t>()		{ return 0x7fffffffffffffffLL; }

template<> inline constexpr	bool		ax_type_min<bool>()			{ return false; }
template<> inline constexpr	bool		ax_type_max<bool>()			{ return true; }


template<> inline constexpr	float		ax_type_min<float>  ()		{ return FLT_MIN; }
template<> inline constexpr	float		ax_type_max<float>	()		{ return FLT_MAX; }
template<> inline constexpr	double		ax_type_min<double>	()		{ return DBL_MIN; }
template<> inline constexpr	double		ax_type_max<double>	()		{ return DBL_MAX; }

template< typename T > 				bool	ax_less_than0			( T value );
template< typename T > constexpr	T 		ax_type_default_value	();
template< typename T > constexpr	ax_int	ax_type_fp_precision	();
template< typename T > constexpr	bool	ax_type_is_pod			();


template< typename A, typename B > inline constexpr bool ax_type_is_base_of	() 	{ return std::is_base_of<A,B>(); }
template< typename A, typename B > inline constexpr bool ax_type_is_same	()	{ return std::is_same<A,B>(); }

template< typename T > inline constexpr bool	ax_type_is_pod				() 	{ return std::is_pod<T>(); }

template< typename T > inline constexpr bool 	ax_type_is_integral			() 	{ return std::is_integral<T>(); }
template< typename T > inline constexpr bool 	ax_type_is_floating_point	() 	{ return std::is_floating_point<T>(); }

template< typename T > inline constexpr bool 	ax_type_is_signed			() 	{ return std::is_signed<T>(); }
template< typename T > inline constexpr bool 	ax_type_is_unsigned			() 	{ return std::is_unsigned<T>(); }

template< typename T > inline constexpr ax_int	ax_type_fp_precision		( const T & v ) { return ax_type_fp_precision<T>(); }

template< typename T > 	struct	ax_type_gc_trace : public std::integral_constant< bool, T::ax_type_on_gc_trace::value > {};

template< typename T > 	struct	ax_type_gc_trace< T& > 					: public std::true_type {};
//template< typename T > 	struct	ax_type_gc_trace< T& const > 			: public std::true_type {};
//template< typename T > 	struct	ax_type_gc_trace< T& volatile > 		: public std::true_type {};
//template< typename T > 	struct	ax_type_gc_trace< T& const volatile > 	: public std::true_type {};

template< typename T > 	struct	ax_type_gc_trace< T* > 					: public std::true_type {};
template< typename T > 	struct	ax_type_gc_trace< T* const > 			: public std::true_type {};
template< typename T > 	struct	ax_type_gc_trace< T* volatile > 		: public std::true_type {};
template< typename T > 	struct	ax_type_gc_trace< T* const volatile > 	: public std::true_type {};


#define	ax_TYPE_LIST_ITEM( NAME, T )	\
	template<> 	struct	ax_type_gc_trace< T > : public std::false_type {}; \
	template<> 	inline constexpr	T 		ax_type_default_value<T>()				{ return 0; } \
	template<>	inline constexpr	ax_int	ax_type_fp_precision<T>()				{ return 0; } \
				inline 				bool	ax_less_than0( T value )				{ return false; } \
//-------
	ax_TYPE_LIST_all_uint
#undef ax_TYPE_LIST_ITEM

#define	ax_TYPE_LIST_ITEM( NAME, T )	\
	template<> 	struct	ax_type_gc_trace< T > : public std::false_type {}; \
	template<> 	inline constexpr	T 		ax_type_default_value<T>()				{ return 0; } \
	template<>	inline constexpr	ax_int	ax_type_fp_precision<T>()				{ return 0; } \
				inline 				bool	ax_less_than0( T value )				{ return value < 0; } \
//-------
	ax_TYPE_LIST_all_int
#undef ax_TYPE_LIST_ITEM

#define	ax_TYPE_LIST_ITEM( NAME, T )	\
	template<> 	struct	ax_type_gc_trace< T > : public std::false_type {}; \
	template<> 	inline constexpr	T 		ax_type_default_value<T>()				{ return 0; } \
				inline 				bool	ax_less_than0( T value )				{ return value < 0; } \
//-------
	ax_TYPE_LIST_all_float
#undef ax_TYPE_LIST_ITEM

#define	ax_TYPE_LIST_ITEM( NAME, T )	\
	template<> 	struct	ax_type_gc_trace< T > : public std::false_type {}; \
	template<> 	inline constexpr	T 		ax_type_default_value<T>()				{ return 0; } \
	template<>	inline constexpr	ax_int	ax_type_fp_precision<T>()				{ return 0; } \
				inline 				bool	ax_less_than0( T value )				{ return value < 0; } \
//-------
	ax_TYPE_LIST_all_char
#undef ax_TYPE_LIST_ITEM

template<>	inline	constexpr 	ax_int	ax_type_fp_precision<float>()	{ return 6; } \
template<>	inline 	constexpr	ax_int	ax_type_fp_precision<double>()	{ return 15; } \


#endif