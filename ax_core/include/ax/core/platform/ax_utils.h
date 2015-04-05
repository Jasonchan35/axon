//
//  ax_utils.h
//  ax_core
//
//  Created by Jason on 2014-11-29.
//  Copyright (c) 2014 Awenix. All rights reserved.
//

#ifndef ax_core_ax_utils_h
#define ax_core_ax_utils_h

template< typename T > inline T* ax_const_cast( const T* v ) { return const_cast<T*>(v); }
template< typename T > inline T& ax_const_cast( const T& v ) { return const_cast<T&>(v); }


#define ax_offsetof_macro(st, m) \
	reinterpret_cast<char*>( &( reinterpret_cast<st*>(nullptr)->*m ) - reinterpret_cast<char*>(nullptr) )

template< typename Class, typename Member > inline
size_t	ax_offsetof( Member Class::*PtrToMember ) {
	auto c = reinterpret_cast<Class*>(nullptr);
	auto m = & ( c->*PtrToMember );
	return reinterpret_cast<char*>( m ) - reinterpret_cast<char*>(nullptr);
}

template< typename Class, typename Member > inline
Class&	ax_member_owner( Member Class::*PtrToMember, Member* member_instance ) {
	auto p = reinterpret_cast<char*>( member_instance ) - ax_offsetof( PtrToMember );
	return *reinterpret_cast<Class*>( p );
}

//---------------------

template< typename T >	inline
ax_int	ax_hash_of( const T & v ) {
	return v.GetHash();
}

template<> inline ax_int 	ax_hash_of( const bool & b ) { return b ? 37 : 73; } // pick 2 prime numbers ?

#define ax_TYPE_LIST_ITEM( NAME, T ) \
	template<> inline ax_int	ax_hash_of( const T & v )	{ return static_cast< ax_int >(v); } \
//----
	ax_TYPE_LIST_all_int
	ax_TYPE_LIST_all_uint
	ax_TYPE_LIST_all_char
#undef ax_TYPE_LIST_ITEM


//---------------------

template< typename T > inline void	ax_swap			( T & a, T & b )	{ T tmp( ax_move(a) ); a = ax_move(b); b = ax_move(tmp); }
template< typename T > inline T 	ax_sign			( const T & a )					{ if( a == 0 ) return 0; return ax_less_than0(a) ? -1: 1; }

template< typename T > inline T		ax_div			( const T & a, const T & b )	{ return ( b==0 ) ? 0 : a/b; }
template< typename T > inline void	ax_div_it		( 	    T & a, const T & b )	{    a = ( b==0 ) ? 0 : a/b; }

template< typename T > inline T		ax_abs			( const T & a )					{ return a>0 ? a : -a ; }
template< typename T > inline void	ax_abs_it		(       T & a )					{ a = ax_abs(a); }

template< typename T > inline T		ax_min			( const T & a, const T & b )	{ return (a<b)?a:b; }
template< typename T > inline T		ax_max			( const T & a, const T & b )	{ return (a>b)?a:b; }

template< typename T > inline void	ax_min_it		(		T & a, const T & b )	{ a = ax_min(a,b); }
template< typename T > inline void	ax_max_it		( 		T & a, const T & b )	{ a = ax_max(a,b); }

//! x clamped to the range [a,b]
template< typename T > inline T		ax_clamp		( const T & x, const T & a, const T & b )	{ if( x < a ) return a; return x > b ? b : x; }
template< typename T > inline T		ax_clamp01		( const T & x )								{ return ax_clamp(x, (T)0, (T)1); }

template< typename T > inline void	ax_clamp_it		(		T & x, const T & a, const T & b )	{ x = ax_clamp(x,a,b); }
template< typename T > inline void	ax_clamp01_it	(		T & x )								{ ax_clamp_it(x, (T)0, (T)1); }

template< typename T > inline int	ax_next_pow2	( const T & v )	{ v--; v |= v >> 1; v |= v >> 2; v |= v >> 4;	v |= v >> 8; v |= v >> 16; v++;	return v; }
template< typename T > inline bool	ax_is_pow2		( const T & v )	{ return !(v & (v - 1)) && v ; }

//----- float ----
template < typename T > inline T	ax_inf	()				{ return std::numeric_limits<T>::infinity(); }
template < typename T > inline T	ax_isinf( const T & v )	{ return std::numeric_limits<T>::has_infinity && v == ax_inf<T>(); }

template< typename T >	T	ax_epsilon();
template<>	inline double	ax_epsilon<double>() { return 1.0e-12; }
template<>	inline float	ax_epsilon<float> () { return 1.0e-5f; }

const  double ax_math_PI = 3.14159265358979323846;

//Splits a floating-point value into fractional and integer parts
inline float  ax_modf		( float  n, float  *i ) { return std::modff( n, i ); }
inline double ax_modf		( double n, double *i ) { return std::modf ( n, i ); }

inline float  ax_sqrt		( float  n )	{ return std::sqrtf( n ); }
inline double ax_sqrt		( double n )	{ return std::sqrt ( n ); }

//! reciprocal square root
inline float  ax_rsqrt		( float  n )	{ return 1.0f/std::sqrtf( n ); }
inline double ax_rsqrt		( double n )	{ return 1.0 /std::sqrt ( n ); }

inline float  ax_ceil		( float  a )	{ return std::ceilf(a); }
inline double ax_ceil		( double a )	{ return std::ceil (a); }

inline float  ax_floor		( float  a )	{ return std::floorf(a); }
inline double ax_floor		( double a )	{ return std::floor (a); }

//get remainder
inline float  ax_fmod	( float  a, float  b ) { return std::fmodf(a,b); }
inline double ax_fmod	( double a, double b ) { return std::fmod (a,b); }

inline float  ax_cos	( float  a ) { return std::cosf(a); }
inline double ax_cos	( double a ) { return std::cos (a); }

inline float  ax_sin	( float  a ) { return std::sinf(a); }
inline double ax_sin	( double a ) { return std::sin (a); }

inline float  ax_atan2	( float  a, float  b ) { return std::atan2f(a,b); }
inline double ax_atan2	( double a, double b ) { return std::atan2 (a,b); }

#if ax_COMPILER_VC
	inline float  ax_trunc	( float  n )	{ float  i; return std::modff( n, &i ); }
	inline double ax_trunc	( double n )	{ double i; return std::modf ( n, &i ); }

	inline float  ax_round	( float  a )	{ return ax_trunc ( a > 0 ? a+0.5f : a-0.5f ); }
	inline double ax_round	( double a )	{ return ax_trunc ( a > 0 ? a+0.5  : a-0.5  ); }
#else
	inline float  ax_trunc	( float  n )	{ return std::truncf(n); }
	inline double ax_trunc	( double n )	{ return std::trunc (n); }

	inline float  ax_round	( float  a )	{ return std::roundf(a); }
	inline double ax_round	( double a )	{ return std::round (a); }
#endif

inline ax_int   ax_trunc_to_int	( float  a )	{ return static_cast<ax_int>( ax_trunc(a) ); }
inline ax_int	ax_trunc_to_int	( double a )	{ return static_cast<ax_int>( ax_trunc(a) ); }

inline ax_int   ax_round_to_int	( float  a )	{ return static_cast<ax_int>( ax_round(a) ); }
inline ax_int	ax_round_to_int	( double a )	{ return static_cast<ax_int>( ax_round(a) ); }

inline ax_int   ax_ceil_to_int	( float  a )	{ return static_cast<ax_int>( ax_ceil (a) ); }
inline ax_int	ax_ceil_to_int	( double a )	{ return static_cast<ax_int>( ax_ceil (a) ); }
inline ax_int   ax_floor_to_int	( float  a )	{ return static_cast<ax_int>( ax_floor(a) ); }
inline ax_int	ax_floor_to_int	( double a )	{ return static_cast<ax_int>( ax_floor(a) ); }


template<class T> inline void ax_round_it	( T & v ) { v = ax_round(v); }
template<class T> inline void ax_floor_it	( T & v ) { v = ax_floor(v); }
template<class T> inline void ax_ceil_it 	( T & v ) { v = ax_ceil (v); }

//! Equivalent
template<class T> inline bool ax_math_equals( const T & a, const T & b, const T & ep=ax_epsilon<T>() )  { return ( ax_abs(a-b) < ep) ; }


//! linear interpolation out = a+w*(b-a)

template<class T> T inline ax_int_lerp( const T & a, const T & b, float  w ) { return static_cast<T>( ax_round( a+w*( b - a) ) ); }
template<class T> T inline ax_int_lerp( const T & a, const T & b, double w ) { return static_cast<T>( ax_round( a+w*( b - a) ) ); }

#define ax_TYPE_LIST_ITEM( NAME, T ) \
	inline T ax_lerp( const T & a, const T & b, float  w ) { return ax_int_lerp(a,b,w); } \
	inline T ax_lerp( const T & a, const T & b, double w ) { return ax_int_lerp(a,b,w); } \
//--------
	ax_TYPE_LIST_all_int
	ax_TYPE_LIST_all_uint
#undef ax_TYPE_LIST_ITEM

inline float	ax_lerp( float    a, float    b, float  w ) { return (1-w)*a + w*b; }
inline double	ax_lerp( double   a, double   b, double w ) { return (1-w)*a + w*b; }

inline float	ax_step( float 	a, float  x ) 	{ return x>=a ? 1.0f : 0.0f; }
inline double	ax_step( double a, double x ) 	{ return x>=a ? 1.0  : 0.0;  }

inline float	ax_ease_in ( float  a ) { return a*a; }	
inline double	ax_ease_in ( double a ) { return a*a; }	
inline float	ax_ease_out( float  a ) { a=1-a; return 1-a*a; }	
inline double	ax_ease_out( double a ) { a=1-a; return 1-a*a; }	
	
inline float	ax_smooth_step( float  w ) { return (-2.0f*w*w*w + 3.0f*w*w); }
inline double	ax_smooth_step( double w ) { return (-2.0 *w*w*w + 3.0 *w*w); }

template<class T> inline T ax_smooth_step( const T &a, const T &b, float  w ) { return ax_lerp(a,b,ax_smooth_step(w) ); }
template<class T> inline T ax_smooth_step( const T &a, const T &b, double w ) { return ax_lerp(a,b,ax_smooth_step(w) ); }

//! align multiple
template< typename T> inline 
T _ax_align_multiple_int( T n, T a ) { 
	return n + a -(n%a);
}

inline int8_t	ax_align_multiple( int8_t  n, int8_t  a ) { return _ax_align_multiple_int(n,a); }
inline int16_t	ax_align_multiple( int16_t n, int16_t a ) { return _ax_align_multiple_int(n,a); }
inline int32_t	ax_align_multiple( int32_t n, int32_t a ) { return _ax_align_multiple_int(n,a); }
inline int64_t	ax_align_multiple( int64_t n, int64_t a ) { return _ax_align_multiple_int(n,a); }

template< typename T> inline 
T _ax_align_multiple_uint( T n, T a ) {  
	T r = n % a;
	if( n > 0 ) { 
		return n + ax_abs(a) -r; 
	}else{ 
		return n - ax_abs(a) -r; 
	} 
}

inline uint8_t	ax_align_multiple( uint8_t  n, uint8_t  a ) { return _ax_align_multiple_uint(n,a); }
inline uint16_t	ax_align_multiple( uint16_t n, uint16_t a ) { return _ax_align_multiple_uint(n,a); }
inline uint32_t	ax_align_multiple( uint32_t n, uint32_t a ) { return _ax_align_multiple_uint(n,a); }
inline uint64_t	ax_align_multiple( uint64_t n, uint64_t a ) { return _ax_align_multiple_uint(n,a); }

//! floating-point align
template< typename T> inline
T  _ax_align_multiple_float( T   n, T   a ) {
	T i = ax_floor( n / a ) * a;
	if( i == n ) return i;
	return (n > 0) ? i+a : i-a;
}

inline float  ax_align_multiple( float  n, float  a ) { return _ax_align_multiple_float(n,a); }
inline double ax_align_multiple( double n, double a ) { return _ax_align_multiple_float(n,a); }

template < typename T, typename W > inline
T ax_bezier( T p0, T p1, T p2, T p3, W w) {
	W iw  = 1-w;
	W iw2 = iw*iw;
	W w2  = w*w;
	
	return	p0 * iw * iw2
		  + p1 * 3  * iw2 * w
		  + p2 * 3  * iw  * w2
		  + p3 * w2 * w;
}

#endif
