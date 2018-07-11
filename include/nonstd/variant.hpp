// Copyright 2016-2018 by Martin Moene
//
// https://github.com/martinmoene/variant-lite
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef NONSTD_VARIANT_LITE_HPP
#define NONSTD_VARIANT_LITE_HPP

#define  variant_lite_VERSION "1.0.0"

// variant-lite configuration:

#ifndef  variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO
# define variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO  0
#endif

#ifndef  variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO
# define variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO  0
#endif

// Compiler detection:

#ifdef _MSVC_LANG
# define variant_MSVC_LANG  _MSVC_LANG
#else
# define variant_MSVC_LANG  0
#endif

#define variant_CPP11_OR_GREATER  ( __cplusplus >= 201103L || variant_MSVC_LANG >= 201103L )
#define variant_CPP14_OR_GREATER  ( __cplusplus >= 201402L || variant_MSVC_LANG >= 201703L )
#define variant_CPP17_OR_GREATER  ( __cplusplus >= 201703L || variant_MSVC_LANG >= 201703L )

// use C++17 std::variant if available:

#if defined( __has_include )
# define variant_HAS_INCLUDE( arg )  __has_include( arg )
#else
# define variant_HAS_INCLUDE( arg )  0
#endif

#if variant_HAS_INCLUDE( <variant> ) && variant_CPP17_OR_GREATER

#define variant_HAVE_STD_VARIANT  1

#include <variant>

#if ! variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO
# define variant_size_V(T)  nonstd::variant_size<T>::value
#endif

#if ! variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO
# define variant_alternative_T(I,T)  typename nonstd::variant_alternative<I,T >::type
#endif

namespace nonstd {

    using std::variant;
    using std::monostate;
    using std::bad_variant_access;
    using std::variant_size;
    using std::variant_size_v;
    using std::variant_alternative;
    using std::variant_alternative_t;
    using std::hash;

    using std::in_place;
    using std::in_place_type;
    using std::in_place_index;
    using std::in_place_t;
    using std::in_place_type_t;
    using std::in_place_index_t;

    using std::visit;
    using std::holds_alternative;
    using std::get;
    using std::get_if;
    using std::operator==;
    using std::operator!=;
    using std::operator<;
    using std::operator<=;
    using std::operator>;
    using std::operator>=;
    using std::swap;

    constexpr auto variant_npos = std::variant_npos;

}

#else // C++17 std::variant

#include <cstddef>
#include <limits>
#include <new>
#include <stdexcept>
#include <utility>

// variant-lite alignment configuration:

#ifndef  variant_CONFIG_MAX_ALIGN_HACK
# define variant_CONFIG_MAX_ALIGN_HACK  0
#endif

#ifndef  variant_CONFIG_ALIGN_AS
// no default, used in #if defined()
#endif

#ifndef  variant_CONFIG_ALIGN_AS_FALLBACK
# define variant_CONFIG_ALIGN_AS_FALLBACK  double
#endif

// half-open range [lo..hi):
#define variant_BETWEEN( v, lo, hi ) ( lo <= v && v < hi )

#if defined(_MSC_VER) && !defined(__clang__)
# define variant_COMPILER_MSVC_VERSION   (_MSC_VER / 100 - 5 - (_MSC_VER < 1900))
#else
# define variant_COMPILER_MSVC_VERSION   0
#endif

#if defined __GNUC__
# define variant_COMPILER_GNUC_VERSION  __GNUC__
#else
# define variant_COMPILER_GNUC_VERSION    0
#endif

#if variant_BETWEEN(variant_COMPILER_MSVC_VERSION, 7, 14 )
# pragma warning( push )
# pragma warning( disable: 4345 )   // initialization behavior changed
#endif

// Presence of C++11 language features:

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 10
# define variant_HAVE_AUTO  1
# define variant_HAVE_NULLPTR  1
# define variant_HAVE_STATIC_ASSERT  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 12
# define variant_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG  1
# define variant_HAVE_INITIALIZER_LIST  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 14
# define variant_HAVE_ALIAS_TEMPLATE  1
# define variant_HAVE_CONSTEXPR_11  1
# define variant_HAVE_ENUM_CLASS  1
# define variant_HAVE_EXPLICIT_CONVERSION  1
# define variant_HAVE_IS_DEFAULT  1
# define variant_HAVE_IS_DELETE  1
# define variant_HAVE_NOEXCEPT  1
# define variant_HAVE_OVERRIDE  1
#endif

// Presence of C++14 language features:

#if variant_CPP14_OR_GREATER
# define variant_HAVE_CONSTEXPR_14  1
#endif

// Presence of C++17 language features:

#if variant_CPP17_OR_GREATER
# define variant_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE  1
#endif

// Presence of C++ library features:

#if variant_COMPILER_GNUC_VERSION
# define variant_HAVE_TR1_TYPE_TRAITS  1
# define variant_HAVE_TR1_ADD_POINTER  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 9
# define variant_HAVE_TYPE_TRAITS  1
# define variant_HAVE_STD_ADD_POINTER  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 11
# define variant_HAVE_ARRAY  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 12
# define variant_HAVE_CONDITIONAL  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 14 || (variant_COMPILER_MSVC_VERSION >= 9 && _HAS_CPP0X)
# define variant_HAVE_CONTAINER_DATA_METHOD  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 12
# define variant_HAVE_REMOVE_CV  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 14
# define variant_HAVE_SIZED_TYPES  1
#endif

// For the rest, consider VC14 as C++11 for variant-lite:

#if variant_COMPILER_MSVC_VERSION >= 14
# undef  variant_CPP11_OR_GREATER
# define variant_CPP11_OR_GREATER  1
#endif

// C++ feature usage:

#if variant_HAVE_CONSTEXPR_11
# define variant_constexpr constexpr
#else
# define variant_constexpr /*constexpr*/
#endif

#if variant_HAVE_CONSTEXPR_14
# define variant_constexpr14 constexpr
#else
# define variant_constexpr14 /*constexpr*/
#endif

#if variant_HAVE_NOEXCEPT
# define variant_noexcept noexcept
#else
# define variant_noexcept /*noexcept*/
#endif

#if variant_HAVE_NULLPTR
# define variant_nullptr nullptr
#else
# define variant_nullptr NULL
#endif

#if variant_HAVE_OVERRIDE
# define variant_override override
#else
# define variant_override /*override*/
#endif

// additional includes:

#if variant_HAVE_INITIALIZER_LIST
# include <initializer_list>
#endif

#if variant_HAVE_TYPE_TRAITS
# include <type_traits>
#elif variant_HAVE_TR1_TYPE_TRAITS
# include <tr1/type_traits>
#endif

//
// in_place: code duplicated in any-lite, optional-lite, variant-lite:
//

#if ! nonstd_lite_HAVE_IN_PLACE_TYPES

namespace nonstd {

namespace detail {

template< class T >
struct in_place_type_tag {};

template< std::size_t I >
struct in_place_index_tag {};

} // namespace detail

struct in_place_t {};

template< class T >
inline in_place_t in_place( detail::in_place_type_tag<T> = detail::in_place_type_tag<T>() )
{
    return in_place_t();
}

template< std::size_t I >
inline in_place_t in_place( detail::in_place_index_tag<I> = detail::in_place_index_tag<I>() )
{
    return in_place_t();
}

template< class T >
inline in_place_t in_place_type( detail::in_place_type_tag<T> = detail::in_place_type_tag<T>() )
{
    return in_place_t();
}

template< std::size_t I >
inline in_place_t in_place_index( detail::in_place_index_tag<I> = detail::in_place_index_tag<I>() )
{
    return in_place_t();
}

// mimic templated typedef:

#define nonstd_lite_in_place_type_t( T)  nonstd::in_place_t(&)( nonstd::detail::in_place_type_tag<T>  )
#define nonstd_lite_in_place_index_t(T)  nonstd::in_place_t(&)( nonstd::detail::in_place_index_tag<I> )

#define nonstd_lite_HAVE_IN_PLACE_TYPES  1

} // namespace nonstd

#endif // nonstd_lite_HAVE_IN_PLACE_TYPES

//
// variant:
//

namespace nonstd { namespace variants {

namespace detail {

// C++11 emulation:

#if variant_HAVE_STD_ADD_POINTER

using std::add_pointer;

#elif variant_HAVE_TR1_ADD_POINTER

using std::tr1::add_pointer;

#else

template< class T > struct remove_reference     { typedef T type; };
template< class T > struct remove_reference<T&> { typedef T type; };

template< class T > struct add_pointer
{
    typedef typename remove_reference<T>::type * type;
};

#endif // variant_HAVE_STD_ADD_POINTER

#if variant_HAVE_REMOVE_CV

using std::remove_cv;

#else

template< class T > struct remove_const          { typedef T type; };
template< class T > struct remove_const<const T> { typedef T type; };

template< class T > struct remove_volatile             { typedef T type; };
template< class T > struct remove_volatile<volatile T> { typedef T type; };

template< class T >
struct remove_cv
{
    typedef typename remove_volatile<typename remove_const<T>::type>::type type;
};

#endif // variant_HAVE_REMOVE_CV

#if variant_HAVE_CONDITIONAL

using std::conditional;

#else

template< bool Cond, class Then, class Else >
struct conditional;

template< class Then, class Else >
struct conditional< true , Then, Else > { typedef Then type; };

template< class Then, class Else >
struct conditional< false, Then, Else > { typedef Else type; };

#endif // variant_HAVE_CONDITIONAL

// typelist:

#if variant_CPP11_OR_GREATER

#define variant_TL1(T1) detail::typelist< T1, detail::nulltype>
#define variant_TL2(T1, ...)  detail::typelist< T1, variant_TL1(__VA_ARGS__)>
#define variant_TL3(T1, ...)  detail::typelist< T1, variant_TL2(__VA_ARGS__)>
#define variant_TL4(T1, ...)  detail::typelist< T1, variant_TL3(__VA_ARGS__)>
#define variant_TL5(T1, ...)  detail::typelist< T1, variant_TL4(__VA_ARGS__)>
#define variant_TL6(T1, ...)  detail::typelist< T1, variant_TL5(__VA_ARGS__)>
#define variant_TL7(T1, ...)  detail::typelist< T1, variant_TL6(__VA_ARGS__)>
#define variant_TL8(T1, ...)  detail::typelist< T1, variant_TL7(__VA_ARGS__)>
#define variant_TL9(T1, ...)  detail::typelist< T1, variant_TL8(__VA_ARGS__)>
#define variant_TL10(T1, ...) detail::typelist< T1, variant_TL9(__VA_ARGS__)>
#define variant_TL11(T1, ...) detail::typelist< T1, variant_TL10(__VA_ARGS__)>
#define variant_TL12(T1, ...) detail::typelist< T1, variant_TL11(__VA_ARGS__)>
#define variant_TL13(T1, ...) detail::typelist< T1, variant_TL12(__VA_ARGS__)>
#define variant_TL14(T1, ...) detail::typelist< T1, variant_TL13(__VA_ARGS__)>
#define variant_TL15(T1, ...) detail::typelist< T1, variant_TL14(__VA_ARGS__)>
#define variant_TL16(T1, ...) detail::typelist< T1, variant_TL15(__VA_ARGS__)>
#define variant_TL17(T1, ...) detail::typelist< T1, variant_TL16(__VA_ARGS__)>
#define variant_TL18(T1, ...) detail::typelist< T1, variant_TL17(__VA_ARGS__)>
#define variant_TL19(T1, ...) detail::typelist< T1, variant_TL18(__VA_ARGS__)>
#define variant_TL20(T1, ...) detail::typelist< T1, variant_TL19(__VA_ARGS__)>
#define variant_TL21(T1, ...) detail::typelist< T1, variant_TL20(__VA_ARGS__)>
#define variant_TL22(T1, ...) detail::typelist< T1, variant_TL21(__VA_ARGS__)>
#define variant_TL23(T1, ...) detail::typelist< T1, variant_TL22(__VA_ARGS__)>
#define variant_TL24(T1, ...) detail::typelist< T1, variant_TL23(__VA_ARGS__)>
#define variant_TL25(T1, ...) detail::typelist< T1, variant_TL24(__VA_ARGS__)>
#define variant_TL26(T1, ...) detail::typelist< T1, variant_TL25(__VA_ARGS__)>
#define variant_TL27(T1, ...) detail::typelist< T1, variant_TL26(__VA_ARGS__)>
#define variant_TL28(T1, ...) detail::typelist< T1, variant_TL27(__VA_ARGS__)>
#define variant_TL29(T1, ...) detail::typelist< T1, variant_TL28(__VA_ARGS__)>
#define variant_TL30(T1, ...) detail::typelist< T1, variant_TL29(__VA_ARGS__)>

#else

#define variant_TL1( T1                         ) detail::typelist< T1, detail::nulltype >
#define variant_TL2( T1, T2                     ) detail::typelist< T1, variant_TL1( T2) >
#define variant_TL3( T1, T2, T3                 ) detail::typelist< T1, variant_TL2( T2, T3 ) >
#define variant_TL4( T1, T2, T3, T4             ) detail::typelist< T1, variant_TL3( T2, T3, T4 ) >
#define variant_TL5( T1, T2, T3, T4, T5         ) detail::typelist< T1, variant_TL4( T2, T3, T4, T5 ) >
#define variant_TL6( T1, T2, T3, T4, T5, T6     ) detail::typelist< T1, variant_TL5( T2, T3, T4, T5, T6 ) >
#define variant_TL7( T1, T2, T3, T4, T5, T6, T7 ) detail::typelist< T1, variant_TL6( T2, T3, T4, T5, T6, T7 ) >

#endif // typelist
// variant parameter unused type tags:

template< class T >
struct TX : T
{
                        inline TX<T> operator+ (           ) const { return TX<T>();  }
                        inline TX<T> operator- (           ) const { return TX<T>();  }

                        inline TX<T> operator! (           ) const { return TX<T>();  }
                        inline TX<T> operator~ (           ) const { return TX<T>();  }

                        inline TX<T>*operator& (           ) const { return variant_nullptr; }

    template< class U > inline TX<T> operator* ( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator/ ( U const & ) const { return TX<T>();  }

    template< class U > inline TX<T> operator% ( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator+ ( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator- ( U const & ) const { return TX<T>();  }

    template< class U > inline TX<T> operator<<( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator>>( U const & ) const { return TX<T>();  }

                        inline bool  operator==( T const & ) const { return false; }
                        inline bool  operator< ( T const & ) const { return false; }

    template< class U > inline TX<T> operator& ( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator| ( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator^ ( U const & ) const { return TX<T>();  }

    template< class U > inline TX<T> operator&&( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator||( U const & ) const { return TX<T>();  }
};

struct S0{}; typedef TX<S0> T0;
struct S1{}; typedef TX<S1> T1;
struct S2{}; typedef TX<S2> T2;
struct S3{}; typedef TX<S3> T3;
struct S4{}; typedef TX<S4> T4;
struct S5{}; typedef TX<S5> T5;
struct S6{}; typedef TX<S6> T6;
struct S7{}; typedef TX<S7> T7;
struct S8{}; typedef TX<S8> T8;
struct S9{}; typedef TX<S9> T9;
struct S10{}; typedef TX<S10> T10;
struct S11{}; typedef TX<S11> T11;
struct S12{}; typedef TX<S12> T12;
struct S13{}; typedef TX<S13> T13;
struct S14{}; typedef TX<S14> T14;
struct S15{}; typedef TX<S15> T15;
struct S16{}; typedef TX<S16> T16;
struct S17{}; typedef TX<S17> T17;
struct S18{}; typedef TX<S18> T18;
struct S19{}; typedef TX<S19> T19;
struct S20{}; typedef TX<S20> T20;
struct S21{}; typedef TX<S21> T21;
struct S22{}; typedef TX<S22> T22;
struct S23{}; typedef TX<S23> T23;
struct S24{}; typedef TX<S24> T24;
struct S25{}; typedef TX<S25> T25;
struct S26{}; typedef TX<S26> T26;
struct S27{}; typedef TX<S27> T27;
struct S28{}; typedef TX<S28> T28;
struct S29{}; typedef TX<S29> T29;

struct nulltype{};

template< class Head, class Tail >
struct typelist
{
    typedef Head head;
    typedef Tail tail;
};

// typelist max element size:

template< class List >
struct typelist_max;

template <>
struct typelist_max< nulltype >
{
    enum { value = 0 } ;
    typedef void type;
};

template< class Head, class Tail >
struct typelist_max< typelist<Head, Tail> >
{
private:
    enum { tail_value = size_t( typelist_max<Tail>::value ) };

    typedef typename typelist_max<Tail>::type tail_type;

public:
    enum { value = (sizeof( Head ) > tail_value) ? sizeof( Head ) : std::size_t( tail_value ) } ;

    typedef typename conditional< (sizeof( Head ) > tail_value), Head, tail_type>::type type;
};

#if variant_CPP11_OR_GREATER

// typelist max alignof element type:

template< class List >
struct typelist_max_alignof;

template <>
struct typelist_max_alignof< nulltype >
{
    enum { value = 0 } ;
};

template< class Head, class Tail >
struct typelist_max_alignof< typelist<Head, Tail> >
{
private:
    enum { tail_value = size_t( typelist_max_alignof<Tail>::value ) };

public:
    enum { value = (alignof( Head ) > tail_value) ? alignof( Head ) : std::size_t( tail_value ) };
};

#endif

// typelist size (length):

template< class List >
struct typelist_size
{
   enum { value = 1 };
};

template<> struct typelist_size< T0 > { enum { value = 0 }; };
template<> struct typelist_size< T1 > { enum { value = 0 }; };
template<> struct typelist_size< T2 > { enum { value = 0 }; };
template<> struct typelist_size< T3 > { enum { value = 0 }; };
template<> struct typelist_size< T4 > { enum { value = 0 }; };
template<> struct typelist_size< T5 > { enum { value = 0 }; };
template<> struct typelist_size< T6 > { enum { value = 0 }; };
template<> struct typelist_size< T7 > { enum { value = 0 }; };
template<> struct typelist_size< T8 > { enum { value = 0 }; };
template<> struct typelist_size< T9 > { enum { value = 0 }; };
template<> struct typelist_size< T10 > { enum { value = 0 }; };
template<> struct typelist_size< T11 > { enum { value = 0 }; };
template<> struct typelist_size< T12 > { enum { value = 0 }; };
template<> struct typelist_size< T13 > { enum { value = 0 }; };
template<> struct typelist_size< T14 > { enum { value = 0 }; };
template<> struct typelist_size< T15 > { enum { value = 0 }; };
template<> struct typelist_size< T16 > { enum { value = 0 }; };
template<> struct typelist_size< T17 > { enum { value = 0 }; };
template<> struct typelist_size< T18 > { enum { value = 0 }; };
template<> struct typelist_size< T19 > { enum { value = 0 }; };
template<> struct typelist_size< T20 > { enum { value = 0 }; };
template<> struct typelist_size< T21 > { enum { value = 0 }; };
template<> struct typelist_size< T22 > { enum { value = 0 }; };
template<> struct typelist_size< T23 > { enum { value = 0 }; };
template<> struct typelist_size< T24 > { enum { value = 0 }; };
template<> struct typelist_size< T25 > { enum { value = 0 }; };
template<> struct typelist_size< T26 > { enum { value = 0 }; };
template<> struct typelist_size< T27 > { enum { value = 0 }; };
template<> struct typelist_size< T28 > { enum { value = 0 }; };
template<> struct typelist_size< T29 > { enum { value = 0 }; };

template<> struct typelist_size< nulltype > { enum { value = 0 } ; };

template< class Head, class Tail >
struct typelist_size< typelist<Head, Tail> >
{
    enum { value = typelist_size<Head>::value + typelist_size<Tail>::value };
};

// typelist index of type:

template< class List, class T >
struct typelist_index_of;

template< class T >
struct typelist_index_of< nulltype, T >
{
    enum { value = -1 };
};

template< class Tail, class T >
struct typelist_index_of< typelist<T, Tail>, T >
{
    enum { value = 0 };
};

template< class Head, class Tail, class T >
struct typelist_index_of< typelist<Head, Tail>, T >
{
private:
    enum { nextVal = typelist_index_of<Tail, T>::value };
public:
    enum { value = nextVal == -1 ? -1 : 1 + nextVal } ;
};

// typelist type at index:

template< class List, std::size_t i>
struct typelist_type_at;

template< class Head, class Tail >
struct typelist_type_at< typelist<Head, Tail>, 0 >
{
    typedef Head type;
};

template< class Head, class Tail, std::size_t i >
struct typelist_type_at< typelist<Head, Tail>, i >
{
    typedef typename typelist_type_at<Tail, i - 1>::type type;
};

#if variant_CONFIG_MAX_ALIGN_HACK

// Max align, use most restricted type for alignment:

#define variant_UNIQUE(  name )       variant_UNIQUE2( name, __LINE__ )
#define variant_UNIQUE2( name, line ) variant_UNIQUE3( name, line )
#define variant_UNIQUE3( name, line ) name ## line

#define variant_ALIGN_TYPE( type ) \
    type variant_UNIQUE( _t ); struct_t< type > variant_UNIQUE( _st )

template< class T >
struct struct_t { T _; };

union max_align_t
{
    variant_ALIGN_TYPE( char );
    variant_ALIGN_TYPE( short int );
    variant_ALIGN_TYPE( int );
    variant_ALIGN_TYPE( long int  );
    variant_ALIGN_TYPE( float  );
    variant_ALIGN_TYPE( double );
    variant_ALIGN_TYPE( long double );
    variant_ALIGN_TYPE( char * );
    variant_ALIGN_TYPE( short int * );
    variant_ALIGN_TYPE( int *  );
    variant_ALIGN_TYPE( long int * );
    variant_ALIGN_TYPE( float * );
    variant_ALIGN_TYPE( double * );
    variant_ALIGN_TYPE( long double * );
    variant_ALIGN_TYPE( void * );

#ifdef HAVE_LONG_LONG
    variant_ALIGN_TYPE( long long );
#endif

    struct Unknown;

    Unknown ( * variant_UNIQUE(_) )( Unknown );
    Unknown * Unknown::* variant_UNIQUE(_);
    Unknown ( Unknown::* variant_UNIQUE(_) )( Unknown );

    struct_t< Unknown ( * )( Unknown)         > variant_UNIQUE(_);
    struct_t< Unknown * Unknown::*            > variant_UNIQUE(_);
    struct_t< Unknown ( Unknown::* )(Unknown) > variant_UNIQUE(_);
};

#undef variant_UNIQUE
#undef variant_UNIQUE2
#undef variant_UNIQUE3

#undef variant_ALIGN_TYPE

#elif defined( variant_CONFIG_ALIGN_AS ) // variant_CONFIG_MAX_ALIGN_HACK

// Use user-specified type for alignment:

#define variant_ALIGN_AS( unused ) \
    variant_CONFIG_ALIGN_AS

#else // variant_CONFIG_MAX_ALIGN_HACK

// Determine POD type to use for alignment:

#define variant_ALIGN_AS( to_align ) \
    typename detail::type_of_size< detail::alignment_types, detail::alignment_of< to_align >::value >::type

template <typename T>
struct alignment_of;

template <typename T>
struct alignment_of_hack
{
    char c;
    T t;
    alignment_of_hack();
};

template <unsigned A, unsigned S>
struct alignment_logic
{
    enum { value = A < S ? A : S };
};

template< typename T >
struct alignment_of
{
    enum { value = alignment_logic<
        sizeof( alignment_of_hack<T> ) - sizeof(T), sizeof(T) >::value, };
};

template< typename List, size_t N >
struct type_of_size
{
    typedef typename conditional<
        N == sizeof( typename List::head ),
            typename List::head,
            typename type_of_size<typename List::tail, N >::type >::type type;
};

template< size_t N >
struct type_of_size< nulltype, N >
{
    typedef variant_CONFIG_ALIGN_AS_FALLBACK type;
};

template< typename T>
struct struct_t { T _; };

#define variant_ALIGN_TYPE( type ) \
    typelist< type , typelist< struct_t< type >

struct Unknown;

typedef
    variant_ALIGN_TYPE( char ),
    variant_ALIGN_TYPE( short ),
    variant_ALIGN_TYPE( int ),
    variant_ALIGN_TYPE( long ),
    variant_ALIGN_TYPE( float ),
    variant_ALIGN_TYPE( double ),
    variant_ALIGN_TYPE( long double ),

    variant_ALIGN_TYPE( char *),
    variant_ALIGN_TYPE( short * ),
    variant_ALIGN_TYPE( int * ),
    variant_ALIGN_TYPE( long * ),
    variant_ALIGN_TYPE( float * ),
    variant_ALIGN_TYPE( double * ),
    variant_ALIGN_TYPE( long double * ),

    variant_ALIGN_TYPE( Unknown ( * )( Unknown ) ),
    variant_ALIGN_TYPE( Unknown * Unknown::*     ),
    variant_ALIGN_TYPE( Unknown ( Unknown::* )( Unknown ) ),

    nulltype
    > > > > > > >    > > > > > > >
    > > > > > > >    > > > > > > >
    > > > > > >
    alignment_types;

#undef variant_ALIGN_TYPE

#endif // variant_CONFIG_MAX_ALIGN_HACK

template< typename T>
inline std::size_t hash( T const & v )
{
    // primes:
    unsigned const int a  = 54059;
    unsigned const int b  = 76963;
    unsigned const int h0 = 37;

    unsigned int h = h0;
    unsigned char const * s = reinterpret_cast<unsigned char const *>( &v );

    for ( std::size_t i = 0; i < sizeof(v); ++i, ++s )
    {
        h = (h * a) ^ (*s * b);
    }
    return h;
}

template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26, class T27, class T28, class T29>
struct helper
{
    typedef signed char type_index_t;
    typedef variant_TL30( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29 ) variant_types;

    template< class U >
    static U * as( void * data )
    {
        return reinterpret_cast<U*>( data );
    }

    template< class U >
    static U const * as( void const * data )
    {
        return reinterpret_cast<const U*>( data );
    }

    static type_index_t to_index_t( std::size_t index )
    {
        return static_cast<type_index_t>( index );
    }

    static void destroy( std::size_t index, void * data )
    {
        switch ( index )
        {
            case 0: as<T0>( data )->~T0(); break;
            case 1: as<T1>( data )->~T1(); break;
            case 2: as<T2>( data )->~T2(); break;
            case 3: as<T3>( data )->~T3(); break;
            case 4: as<T4>( data )->~T4(); break;
            case 5: as<T5>( data )->~T5(); break;
            case 6: as<T6>( data )->~T6(); break;
            case 7: as<T7>( data )->~T7(); break;
            case 8: as<T8>( data )->~T8(); break;
            case 9: as<T9>( data )->~T9(); break;
            case 10: as<T10>( data )->~T10(); break;
            case 11: as<T11>( data )->~T11(); break;
            case 12: as<T12>( data )->~T12(); break;
            case 13: as<T13>( data )->~T13(); break;
            case 14: as<T14>( data )->~T14(); break;
            case 15: as<T15>( data )->~T15(); break;
            case 16: as<T16>( data )->~T16(); break;
            case 17: as<T17>( data )->~T17(); break;
            case 18: as<T18>( data )->~T18(); break;
            case 19: as<T19>( data )->~T19(); break;
            case 20: as<T20>( data )->~T20(); break;
            case 21: as<T21>( data )->~T21(); break;
            case 22: as<T22>( data )->~T22(); break;
            case 23: as<T23>( data )->~T23(); break;
            case 24: as<T24>( data )->~T24(); break;
            case 25: as<T25>( data )->~T25(); break;
            case 26: as<T26>( data )->~T26(); break;
            case 27: as<T27>( data )->~T27(); break;
            case 28: as<T28>( data )->~T28(); break;
            case 29: as<T29>( data )->~T29(); break;
        }
    }

#if variant_CPP11_OR_GREATER
    template< class T, class... Args >
    static type_index_t construct_t( void * data, Args&&... args )
    {
        new( data ) T( std::forward<Args>(args)... );

        return to_index_t( detail::typelist_index_of< variant_types, T>::value );
    }

    template< std::size_t I, class... Args >
    static type_index_t construct_i( void * data, Args&&... args )
    {
        using type = typename detail::typelist_type_at< variant_types, I >::type;

        construct_t< type >( data, std::forward<Args>(args)... );

        return to_index_t( I );
    }

    static type_index_t move( std::size_t const from_index, void * from_value, void * to_value )
    {
        switch ( from_index )
        {
            case 0: new( to_value ) T0( std::forward<T0>( *as<T0>( from_value ) ) ); break;
            case 1: new( to_value ) T1( std::forward<T1>( *as<T1>( from_value ) ) ); break;
            case 2: new( to_value ) T2( std::forward<T2>( *as<T2>( from_value ) ) ); break;
            case 3: new( to_value ) T3( std::forward<T3>( *as<T3>( from_value ) ) ); break;
            case 4: new( to_value ) T4( std::forward<T4>( *as<T4>( from_value ) ) ); break;
            case 5: new( to_value ) T5( std::forward<T5>( *as<T5>( from_value ) ) ); break;
            case 6: new( to_value ) T6( std::forward<T6>( *as<T6>( from_value ) ) ); break;
            case 7: new( to_value ) T7( std::forward<T7>( *as<T7>( from_value ) ) ); break;
            case 8: new( to_value ) T8( std::forward<T8>( *as<T8>( from_value ) ) ); break;
            case 9: new( to_value ) T9( std::forward<T9>( *as<T9>( from_value ) ) ); break;
            case 10: new( to_value ) T10( std::forward<T10>( *as<T10>( from_value ) ) ); break;
            case 11: new( to_value ) T11( std::forward<T11>( *as<T11>( from_value ) ) ); break;
            case 12: new( to_value ) T12( std::forward<T12>( *as<T12>( from_value ) ) ); break;
            case 13: new( to_value ) T13( std::forward<T13>( *as<T13>( from_value ) ) ); break;
            case 14: new( to_value ) T14( std::forward<T14>( *as<T14>( from_value ) ) ); break;
            case 15: new( to_value ) T15( std::forward<T15>( *as<T15>( from_value ) ) ); break;
            case 16: new( to_value ) T16( std::forward<T16>( *as<T16>( from_value ) ) ); break;
            case 17: new( to_value ) T17( std::forward<T17>( *as<T17>( from_value ) ) ); break;
            case 18: new( to_value ) T18( std::forward<T18>( *as<T18>( from_value ) ) ); break;
            case 19: new( to_value ) T19( std::forward<T19>( *as<T19>( from_value ) ) ); break;
            case 20: new( to_value ) T20( std::forward<T20>( *as<T20>( from_value ) ) ); break;
            case 21: new( to_value ) T21( std::forward<T21>( *as<T21>( from_value ) ) ); break;
            case 22: new( to_value ) T22( std::forward<T22>( *as<T22>( from_value ) ) ); break;
            case 23: new( to_value ) T23( std::forward<T23>( *as<T23>( from_value ) ) ); break;
            case 24: new( to_value ) T24( std::forward<T24>( *as<T24>( from_value ) ) ); break;
            case 25: new( to_value ) T25( std::forward<T25>( *as<T25>( from_value ) ) ); break;
            case 26: new( to_value ) T26( std::forward<T26>( *as<T26>( from_value ) ) ); break;
            case 27: new( to_value ) T27( std::forward<T27>( *as<T27>( from_value ) ) ); break;
            case 28: new( to_value ) T28( std::forward<T28>( *as<T28>( from_value ) ) ); break;
            case 29: new( to_value ) T29( std::forward<T29>( *as<T29>( from_value ) ) ); break;
        }
        return to_index_t( from_index );
    }
#endif

    static type_index_t copy( std::size_t const from_index, const void * from_value, void * to_value )
    {
        switch ( from_index )
        {
            case 0: new( to_value ) T0( *as<T0>( from_value ) ); break;
            case 1: new( to_value ) T1( *as<T1>( from_value ) ); break;
            case 2: new( to_value ) T2( *as<T2>( from_value ) ); break;
            case 3: new( to_value ) T3( *as<T3>( from_value ) ); break;
            case 4: new( to_value ) T4( *as<T4>( from_value ) ); break;
            case 5: new( to_value ) T5( *as<T5>( from_value ) ); break;
            case 6: new( to_value ) T6( *as<T6>( from_value ) ); break;
            case 7: new( to_value ) T7( *as<T7>( from_value ) ); break;
            case 8: new( to_value ) T8( *as<T8>( from_value ) ); break;
            case 9: new( to_value ) T9( *as<T9>( from_value ) ); break;
            case 10: new( to_value ) T10( *as<T10>( from_value ) ); break;
            case 11: new( to_value ) T11( *as<T11>( from_value ) ); break;
            case 12: new( to_value ) T12( *as<T12>( from_value ) ); break;
            case 13: new( to_value ) T13( *as<T13>( from_value ) ); break;
            case 14: new( to_value ) T14( *as<T14>( from_value ) ); break;
            case 15: new( to_value ) T15( *as<T15>( from_value ) ); break;
            case 16: new( to_value ) T16( *as<T16>( from_value ) ); break;
            case 17: new( to_value ) T17( *as<T17>( from_value ) ); break;
            case 18: new( to_value ) T18( *as<T18>( from_value ) ); break;
            case 19: new( to_value ) T19( *as<T19>( from_value ) ); break;
            case 20: new( to_value ) T20( *as<T20>( from_value ) ); break;
            case 21: new( to_value ) T21( *as<T21>( from_value ) ); break;
            case 22: new( to_value ) T22( *as<T22>( from_value ) ); break;
            case 23: new( to_value ) T23( *as<T23>( from_value ) ); break;
            case 24: new( to_value ) T24( *as<T24>( from_value ) ); break;
            case 25: new( to_value ) T25( *as<T25>( from_value ) ); break;
            case 26: new( to_value ) T26( *as<T26>( from_value ) ); break;
            case 27: new( to_value ) T27( *as<T27>( from_value ) ); break;
            case 28: new( to_value ) T28( *as<T28>( from_value ) ); break;
            case 29: new( to_value ) T29( *as<T29>( from_value ) ); break;
        }
        return to_index_t( from_index );
    }
};

} // namespace detail

//
// Variant:
//

template< class T0, 
          class T1, 
          class T2, 
          class T3, 
          class T4, 
          class T5, 
          class T6, 
          class T7, 
          class T8, 
          class T9, 
          class T10, 
          class T11, 
          class T12, 
          class T13, 
          class T14, 
          class T15, 
          class T16, 
          class T17, 
          class T18, 
          class T19, 
          class T20, 
          class T21, 
          class T22, 
          class T23, 
          class T24, 
          class T25, 
          class T26, 
          class T27, 
          class T28, 
          class T29 
        >
class variant;

class monostate{};

inline variant_constexpr bool operator< ( monostate, monostate ) variant_noexcept { return false; }
inline variant_constexpr bool operator> ( monostate, monostate ) variant_noexcept { return false; }
inline variant_constexpr bool operator<=( monostate, monostate ) variant_noexcept { return true;  }
inline variant_constexpr bool operator>=( monostate, monostate ) variant_noexcept { return true;  }
inline variant_constexpr bool operator==( monostate, monostate ) variant_noexcept { return true;  }
inline variant_constexpr bool operator!=( monostate, monostate ) variant_noexcept { return false; }

// obtain the size of the variant's list of alternatives at compile time

template< class T >
struct variant_size; /* undefined */

#if variant_CPP11_OR_GREATER
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26, class T27, class T28, class T29>
struct variant_size<variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29>>
{
    enum _ { value = detail::typelist_size< variant_TL30(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29)>::value };
};
#else
template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
struct variant_size< variant<T0, T1, T2, T3, T4, T5, T6> >
{
    enum _ { value = detail::typelist_size< variant_TL7(T0, T1, T2, T3, T4, T5, T6) >::value };
};
#endif

#if variant_CPP14_OR_GREATER
template< class T >
constexpr std::size_t variant_size_v = variant_size<T>::value;
#endif

#if ! variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO
# define variant_size_V(T)  nonstd::variant_size<T>::value
#endif

// obtain the type of the alternative specified by its index, at compile time:

template< std::size_t I, class T >
struct variant_alternative; /* undefined */

#if variant_CPP11_OR_GREATER
template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26, class T27, class T28, class T29>
struct variant_alternative< I, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29>>
{
    typedef typename detail::typelist_type_at<variant_TL30(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29), I>::type type;
};
#else
template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
struct variant_alternative< I, variant<T0, T1, T2, T3, T4, T5, T6> >
{
    typedef typename detail::typelist_type_at<variant_TL7(T0, T1, T2, T3, T4, T5, T6), I>::type type;
};
#endif

#if variant_CPP11_OR_GREATER
template< std::size_t I, class T >
using variant_alternative_t = typename variant_alternative<I, T>::type;
#endif

#if ! variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO
# define variant_alternative_T(I,T)  typename nonstd::variant_alternative<I,T >::type
#endif

// NTS:implement specializes the std::uses_allocator type trait
// std::uses_allocator<nonstd::variant>

// index of the variant in the invalid state (constant)

#if variant_CPP11_OR_GREATER
variant_constexpr std::size_t variant_npos = static_cast<std::size_t>( -1 );
#else
static const std::size_t variant_npos = static_cast<std::size_t>( -1 );
#endif

class bad_variant_access : public std::exception
{
public:
#if variant_CPP11_OR_GREATER
    virtual const char* what() const variant_noexcept variant_override
#else
    virtual const char* what() const throw()
#endif
    {
        return "bad variant access";
    }
};

template<
    class T0,
    class T1 = detail::T1,
    class T2 = detail::T2,
    class T3 = detail::T3,
    class T4 = detail::T4,
    class T5 = detail::T5,
    class T6 = detail::T6,
    class T7 = detail::T7,
    class T8 = detail::T8,
    class T9 = detail::T9,
    class T10 = detail::T10,
    class T11 = detail::T11,
    class T12 = detail::T12,
    class T13 = detail::T13,
    class T14 = detail::T14,
    class T15 = detail::T15,
    class T16 = detail::T16,
    class T17 = detail::T17,
    class T18 = detail::T18,
    class T19 = detail::T19,
    class T20 = detail::T20,
    class T21 = detail::T21,
    class T22 = detail::T22,
    class T23 = detail::T23,
    class T24 = detail::T24,
    class T25 = detail::T25,
    class T26 = detail::T26,
    class T27 = detail::T27,
    class T28 = detail::T28,
    class T29 = detail::T29
>
class variant
{
    typedef detail::helper< T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29> helper_type;
    typedef variant_TL30(    T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29 ) variant_types;

public:
    variant() : type_index( 0 ) { new( ptr() ) T0(); }

    variant( T0 const & t0 ) : type_index( 0 ) { new( ptr() ) T0( t0 ); }
    variant( T1 const & t1 ) : type_index( 1 ) { new( ptr() ) T1( t1 ); }
    variant( T2 const & t2 ) : type_index( 2 ) { new( ptr() ) T2( t2 ); }
    variant( T3 const & t3 ) : type_index( 3 ) { new( ptr() ) T3( t3 ); }
    variant( T4 const & t4 ) : type_index( 4 ) { new( ptr() ) T4( t4 ); }
    variant( T5 const & t5 ) : type_index( 5 ) { new( ptr() ) T5( t5 ); }
    variant( T6 const & t6 ) : type_index( 6 ) { new( ptr() ) T6( t6 ); }
    variant( T7 const & t7 ) : type_index( 7 ) { new( ptr() ) T6( t7 ); }
    variant( T8 const & t8 ) : type_index( 8 ) { new( ptr() ) T6( t8 ); }
    variant( T9 const & t9 ) : type_index( 9 ) { new( ptr() ) T6( t9 ); }
    variant( T10 const & t10 ) : type_index( 10 ) { new( ptr() ) T6( t10 ); }
    variant( T11 const & t11 ) : type_index( 11 ) { new( ptr() ) T6( t11 ); }
    variant( T12 const & t12 ) : type_index( 12 ) { new( ptr() ) T6( t12 ); }
    variant( T13 const & t13 ) : type_index( 13 ) { new( ptr() ) T6( t13 ); }
    variant( T14 const & t14 ) : type_index( 14 ) { new( ptr() ) T6( t14 ); }
    variant( T15 const & t15 ) : type_index( 15 ) { new( ptr() ) T6( t15 ); }
    variant( T16 const & t16 ) : type_index( 16 ) { new( ptr() ) T6( t16 ); }
    variant( T17 const & t17 ) : type_index( 17 ) { new( ptr() ) T6( t17 ); }
    variant( T18 const & t18 ) : type_index( 18 ) { new( ptr() ) T6( t18 ); }
    variant( T19 const & t19 ) : type_index( 19 ) { new( ptr() ) T6( t19 ); }
    variant( T20 const & t20 ) : type_index( 20 ) { new( ptr() ) T6( t20 ); }
    variant( T21 const & t21 ) : type_index( 21 ) { new( ptr() ) T6( t21 ); }
    variant( T22 const & t22 ) : type_index( 22 ) { new( ptr() ) T6( t22 ); }
    variant( T23 const & t23 ) : type_index( 23 ) { new( ptr() ) T6( t23 ); }
    variant( T24 const & t24 ) : type_index( 24 ) { new( ptr() ) T6( t24 ); }
    variant( T25 const & t25 ) : type_index( 25 ) { new( ptr() ) T6( t25 ); }
    variant( T26 const & t26 ) : type_index( 26 ) { new( ptr() ) T6( t26 ); }
    variant( T27 const & t27 ) : type_index( 27 ) { new( ptr() ) T6( t27 ); }
    variant( T28 const & t28 ) : type_index( 28 ) { new( ptr() ) T6( t28 ); }
    variant( T29 const & t29 ) : type_index( 29 ) { new( ptr() ) T6( t29 ); }

#if variant_CPP11_OR_GREATER
    variant( T0 &&      t0 ) : type_index( 0 ) { new( ptr() ) T0( std::move( t0 ) ); }
    variant( T1 &&      t1 ) : type_index( 1 ) { new( ptr() ) T1( std::move( t1 ) ); }
    variant( T2 &&      t2 ) : type_index( 2 ) { new( ptr() ) T2( std::move( t2 ) ); }
    variant( T3 &&      t3 ) : type_index( 3 ) { new( ptr() ) T3( std::move( t3 ) ); }
    variant( T4 &&      t4 ) : type_index( 4 ) { new( ptr() ) T4( std::move( t4 ) ); }
    variant( T5 &&      t5 ) : type_index( 5 ) { new( ptr() ) T5( std::move( t5 ) ); }
    variant( T6 &&      t6 ) : type_index( 6 ) { new( ptr() ) T6( std::move( t6 ) ); }
    variant( T7 &&      t7 ) : type_index( 7 ) { new( ptr() ) T7( std::move( t7 ) ); }
    variant( T8 &&      t8 ) : type_index( 8 ) { new( ptr() ) T8( std::move( t8 ) ); }
    variant( T9 &&      t9 ) : type_index( 9 ) { new( ptr() ) T9( std::move( t9 ) ); }
    variant( T10 &&      t10 ) : type_index( 10 ) { new( ptr() ) T10( std::move( t10 ) ); }
    variant( T11 &&      t11 ) : type_index( 11 ) { new( ptr() ) T11( std::move( t11 ) ); }
    variant( T12 &&      t12 ) : type_index( 12 ) { new( ptr() ) T12( std::move( t12 ) ); }
    variant( T13 &&      t13 ) : type_index( 13 ) { new( ptr() ) T13( std::move( t13 ) ); }
    variant( T14 &&      t14 ) : type_index( 14 ) { new( ptr() ) T14( std::move( t14 ) ); }
    variant( T15 &&      t15 ) : type_index( 15 ) { new( ptr() ) T15( std::move( t15 ) ); }
    variant( T16 &&      t16 ) : type_index( 16 ) { new( ptr() ) T16( std::move( t16 ) ); }
    variant( T17 &&      t17 ) : type_index( 17 ) { new( ptr() ) T17( std::move( t17 ) ); }
    variant( T18 &&      t18 ) : type_index( 18 ) { new( ptr() ) T18( std::move( t18 ) ); }
    variant( T19 &&      t19 ) : type_index( 19 ) { new( ptr() ) T19( std::move( t19 ) ); }
    variant( T20 &&      t20 ) : type_index( 20 ) { new( ptr() ) T20( std::move( t20 ) ); }
    variant( T21 &&      t21 ) : type_index( 21 ) { new( ptr() ) T21( std::move( t21 ) ); }
    variant( T22 &&      t22 ) : type_index( 22 ) { new( ptr() ) T22( std::move( t22 ) ); }
    variant( T23 &&      t23 ) : type_index( 23 ) { new( ptr() ) T23( std::move( t23 ) ); }
    variant( T24 &&      t24 ) : type_index( 24 ) { new( ptr() ) T24( std::move( t24 ) ); }
    variant( T25 &&      t25 ) : type_index( 25 ) { new( ptr() ) T25( std::move( t25 ) ); }
    variant( T26 &&      t26 ) : type_index( 26 ) { new( ptr() ) T26( std::move( t26 ) ); }
    variant( T27 &&      t27 ) : type_index( 27 ) { new( ptr() ) T27( std::move( t27 ) ); }
    variant( T28 &&      t28 ) : type_index( 28 ) { new( ptr() ) T28( std::move( t28 ) ); }
    variant( T29 &&      t29 ) : type_index( 29 ) { new( ptr() ) T29( std::move( t29 ) ); }
#endif

    variant(variant const & rhs)
    : type_index( rhs.type_index )
    {
        (void) helper_type::copy( rhs.type_index, rhs.ptr(), ptr() );
    }

#if variant_CPP11_OR_GREATER

    variant( variant && rhs )
    : type_index( rhs.type_index )
    {
        (void) helper_type::move( rhs.type_index, rhs.ptr(), ptr() );
    }

    template< std::size_t I >
    using type_at_t = typename detail::typelist_type_at< variant_types, I >::type;

    template< class T, class... Args,
        typename = typename std::enable_if< std::is_constructible< T, Args...>::value>::type >
    explicit variant( nonstd_lite_in_place_type_t(T), Args&&... args)
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), std::forward<Args>(args)... );
    }

    template< class T, class U, class... Args,
        typename = typename std::enable_if< std::is_constructible< T, std::initializer_list<U>&, Args...>::value>::type >
    explicit variant( nonstd_lite_in_place_type_t(T), std::initializer_list<U> il, Args&&... args )
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), il, std::forward<Args>(args)... );
    }

    template< std::size_t I, class... Args,
        typename = typename std::enable_if< std::is_constructible< type_at_t<I>, Args...>::value>::type >
    explicit variant( nonstd_lite_in_place_index_t(I), Args&&... args )
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_i<I>( ptr(), std::forward<Args>(args)... );
    }

    template <size_t I, class U, class... Args,
        typename = typename std::enable_if< std::is_constructible< type_at_t<I>, std::initializer_list<U>&, Args...>::value >::type >
    explicit variant( nonstd_lite_in_place_index_t(I), std::initializer_list<U> il, Args&&... args )
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_i<I>( ptr(), il, std::forward<Args>(args)... );
    }

#endif // variant_CPP11_OR_GREATER

    ~variant()
    {
        helper_type::destroy( index(), ptr() );
    }

    variant & operator=( variant const & rhs )
    {
        return copy_assign( rhs );
    }

#if variant_CPP11_OR_GREATER

    variant & operator=( variant && rhs )
    {
        return move_assign( std::forward<variant>( rhs ) );
    }

    variant & operator=( T0 &&      t0 ) { return move_assign_value<T0,0>( std::forward<T0>( t0 ) ); }
    variant & operator=( T1 &&      t1 ) { return move_assign_value<T1,1>( std::forward<T1>( t1 ) ); }
    variant & operator=( T2 &&      t2 ) { return move_assign_value<T2,2>( std::forward<T2>( t2 ) ); }
    variant & operator=( T3 &&      t3 ) { return move_assign_value<T3,3>( std::forward<T3>( t3 ) ); }
    variant & operator=( T4 &&      t4 ) { return move_assign_value<T4,4>( std::forward<T4>( t4 ) ); }
    variant & operator=( T5 &&      t5 ) { return move_assign_value<T5,5>( std::forward<T5>( t5 ) ); }
    variant & operator=( T6 &&      t6 ) { return move_assign_value<T6,6>( std::forward<T6>( t6 ) ); }
    variant & operator=( T7 &&      t7 ) { return move_assign_value<T7,7>( std::forward<T6>( t7 ) ); }
    variant & operator=( T8 &&      t8 ) { return move_assign_value<T8,8>( std::forward<T6>( t8 ) ); }
    variant & operator=( T9 &&      t9 ) { return move_assign_value<T9,9>( std::forward<T6>( t9 ) ); }
    variant & operator=( T10 &&      t10 ) { return move_assign_value<T10,10>( std::forward<T6>( t10 ) ); }
    variant & operator=( T11 &&      t11 ) { return move_assign_value<T11,11>( std::forward<T6>( t11 ) ); }
    variant & operator=( T12 &&      t12 ) { return move_assign_value<T12,12>( std::forward<T6>( t12 ) ); }
    variant & operator=( T13 &&      t13 ) { return move_assign_value<T13,13>( std::forward<T6>( t13 ) ); }
    variant & operator=( T14 &&      t14 ) { return move_assign_value<T14,14>( std::forward<T6>( t14 ) ); }
    variant & operator=( T15 &&      t15 ) { return move_assign_value<T15,15>( std::forward<T6>( t15 ) ); }
    variant & operator=( T16 &&      t16 ) { return move_assign_value<T16,16>( std::forward<T6>( t16 ) ); }
    variant & operator=( T17 &&      t17 ) { return move_assign_value<T17,17>( std::forward<T6>( t17 ) ); }
    variant & operator=( T18 &&      t18 ) { return move_assign_value<T18,18>( std::forward<T6>( t18 ) ); }
    variant & operator=( T19 &&      t19 ) { return move_assign_value<T19,19>( std::forward<T6>( t19 ) ); }
    variant & operator=( T20 &&      t20 ) { return move_assign_value<T20,20>( std::forward<T6>( t20 ) ); }
    variant & operator=( T21 &&      t21 ) { return move_assign_value<T21,21>( std::forward<T6>( t21 ) ); }
    variant & operator=( T22 &&      t22 ) { return move_assign_value<T22,22>( std::forward<T6>( t22 ) ); }
    variant & operator=( T23 &&      t23 ) { return move_assign_value<T23,23>( std::forward<T6>( t23 ) ); }
    variant & operator=( T24 &&      t24 ) { return move_assign_value<T24,24>( std::forward<T6>( t24 ) ); }
    variant & operator=( T25 &&      t25 ) { return move_assign_value<T25,25>( std::forward<T6>( t25 ) ); }
    variant & operator=( T26 &&      t26 ) { return move_assign_value<T26,26>( std::forward<T6>( t26 ) ); }
    variant & operator=( T27 &&      t27 ) { return move_assign_value<T27,27>( std::forward<T6>( t27 ) ); }
    variant & operator=( T28 &&      t28 ) { return move_assign_value<T28,28>( std::forward<T6>( t28 ) ); }
    variant & operator=( T29 &&      t29 ) { return move_assign_value<T29,29>( std::forward<T6>( t29 ) ); }
#else
    variant & operator=( T0 const & t0 ) { return copy_assign_value<T0,0>( t0 ); }
    variant & operator=( T1 const & t1 ) { return copy_assign_value<T1,1>( t1 ); }
    variant & operator=( T2 const & t2 ) { return copy_assign_value<T2,2>( t2 ); }
    variant & operator=( T3 const & t3 ) { return copy_assign_value<T3,3>( t3 ); }
    variant & operator=( T4 const & t4 ) { return copy_assign_value<T4,4>( t4 ); }
    variant & operator=( T5 const & t5 ) { return copy_assign_value<T5,5>( t5 ); }
    variant & operator=( T6 const & t6 ) { return copy_assign_value<T6,6>( t6 ); }
    variant & operator=( T7 const & t7 ) { return copy_assign_value<T7,7>( t7 ); }
    variant & operator=( T8 const & t8 ) { return copy_assign_value<T8,8>( t8 ); }
    variant & operator=( T9 const & t9 ) { return copy_assign_value<T9,9>( t9 ); }
    variant & operator=( T10 const & t10 ) { return copy_assign_value<T10,10>( t10 ); }
    variant & operator=( T11 const & t11 ) { return copy_assign_value<T11,11>( t11 ); }
    variant & operator=( T12 const & t12 ) { return copy_assign_value<T12,12>( t12 ); }
    variant & operator=( T13 const & t13 ) { return copy_assign_value<T13,13>( t13 ); }
    variant & operator=( T14 const & t14 ) { return copy_assign_value<T14,14>( t14 ); }
    variant & operator=( T15 const & t15 ) { return copy_assign_value<T15,15>( t15 ); }
    variant & operator=( T16 const & t16 ) { return copy_assign_value<T16,16>( t16 ); }
    variant & operator=( T17 const & t17 ) { return copy_assign_value<T17,17>( t17 ); }
    variant & operator=( T18 const & t18 ) { return copy_assign_value<T18,18>( t18 ); }
    variant & operator=( T19 const & t19 ) { return copy_assign_value<T19,19>( t19 ); }
    variant & operator=( T20 const & t20 ) { return copy_assign_value<T20,20>( t20 ); }
    variant & operator=( T21 const & t21 ) { return copy_assign_value<T21,21>( t21 ); }
    variant & operator=( T22 const & t22 ) { return copy_assign_value<T22,22>( t22 ); }
    variant & operator=( T23 const & t23 ) { return copy_assign_value<T23,23>( t23 ); }
    variant & operator=( T24 const & t24 ) { return copy_assign_value<T24,24>( t24 ); }
    variant & operator=( T25 const & t25 ) { return copy_assign_value<T25,25>( t25 ); }
    variant & operator=( T26 const & t26 ) { return copy_assign_value<T26,26>( t26 ); }
    variant & operator=( T27 const & t27 ) { return copy_assign_value<T27,27>( t27 ); }
    variant & operator=( T28 const & t28 ) { return copy_assign_value<T28,28>( t28 ); }
    variant & operator=( T29 const & t29 ) { return copy_assign_value<T29,29>( t29 ); }
#endif

    std::size_t index() const
    {
        return variant_npos_internal() == type_index ? variant_npos : type_index;
    }

    bool valueless_by_exception() const
    {
        return type_index == variant_npos_internal();
    }

#if variant_CPP11_OR_GREATER
    template< class T, class... Args,
        typename = typename std::enable_if< std::is_constructible< T, Args...>::value>::type >
    void emplace( Args&&... args )
    {
        helper_type::destroy( type_index, ptr() );
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), std::forward<Args>(args)... );
    }

    template< class T, class U, class... Args,
        typename = typename std::enable_if< std::is_constructible< T, std::initializer_list<U>&, Args...>::value>::type >
    void emplace( std::initializer_list<U> il, Args&&... args )
    {
        helper_type::destroy( type_index, ptr() );
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), il, std::forward<Args>(args)... );
    }

    template< size_t I, class... Args,
        typename = typename std::enable_if< std::is_constructible< type_at_t<I>, Args...>::value>::type >
    void emplace( Args&&... args )
    {
        this->template emplace< type_at_t<I> >( std::forward<Args>(args)... );
    }

    template< size_t I, class U, class... Args,
        typename = typename std::enable_if< std::is_constructible< type_at_t<I>, std::initializer_list<U>&, Args...>::value >::type >
    void emplace( std::initializer_list<U> il, Args&&... args )
    {
        this->template emplace< type_at_t<I> >( il, std::forward<Args>(args)... );
    }

#endif // variant_CPP11_OR_GREATER

    void swap( variant & rhs ) variant_noexcept
    {
        if ( valueless_by_exception() && rhs.valueless_by_exception() )
        {
            // no effect
        }
        else if ( index() == rhs.index() )
        {
            this->swap_value( index(), rhs );
        }
        else
        {
            variant tmp( *this );
            *this = rhs;
            rhs = tmp;
        }
    }

    //
    // non-standard:
    //

    template< class T >
    variant_constexpr std::size_t index_of() const variant_noexcept
    {
        return detail::typelist_index_of<variant_types, typename detail::remove_cv<T>::type >::value;
    }

    template< class T >
    T & get()
    {
        const std::size_t i = index_of<T>();

        if ( i != index() || i == max_index() )
        {
            throw bad_variant_access();
        }

        return *as<T>();
    }

    template< class T >
    T const & get() const
    {
        const std::size_t i = index_of<T>();

        if ( i != index() || i == max_index() )
        {
            throw bad_variant_access();
        }

        return *as<const T>();
    }

    template< std::size_t I >
    typename variant_alternative< I, variant >::type &
    get()
    {
        return this->template get< typename detail::typelist_type_at< variant_types, I >::type >();
    }

    template< std::size_t I >
    typename variant_alternative< I, variant >::type const &
    get() const
    {
        return this->template get< typename detail::typelist_type_at< variant_types, I >::type >();
    }

private:
    typedef typename helper_type::type_index_t type_index_t;

    void * ptr() variant_noexcept
    {
        return &data;
    }

    void const * ptr() const variant_noexcept
    {
        return &data;
    }

    template< class U >
    U * as()
    {
        return reinterpret_cast<U*>( ptr() );
    }

    template< class U >
    U const * as() const
    {
        return reinterpret_cast<U const *>( ptr() );
    }

    variant_constexpr std::size_t max_index() const variant_noexcept
    {
        return data_size;
    }

    variant_constexpr type_index_t variant_npos_internal() const variant_noexcept
    {
        return static_cast<type_index_t>( -1 );
    }

    variant & copy_assign( variant const & rhs )
    {
        if ( valueless_by_exception() && rhs.valueless_by_exception() )
        {
            // no effect
        }
        else if ( ! valueless_by_exception() && rhs.valueless_by_exception() )
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
        }
        else if ( index() == rhs.index() )
        {
            type_index = helper_type::copy( rhs.type_index, rhs.ptr(), ptr() );
        }
        else
        {
            // alas exception safety with pre-C++11 needs an extra copy:

            variant tmp( rhs );
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
#if variant_CPP11_OR_GREATER
            type_index = helper_type::move( rhs.type_index, tmp.ptr(), ptr() );
#else
            type_index = helper_type::copy( rhs.type_index, tmp.ptr(), ptr() );
#endif
        }
        return *this;
    }

#if variant_CPP11_OR_GREATER

    variant & move_assign( variant && rhs )
    {
        if ( valueless_by_exception() && rhs.valueless_by_exception() )
        {
            // no effect
        }
        else if ( ! valueless_by_exception() && rhs.valueless_by_exception() )
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
        }
        else if ( index() == rhs.index() )
        {
            type_index = helper_type::move( rhs.type_index, rhs.ptr(), ptr() );
        }
        else
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
            type_index = helper_type::move( rhs.type_index, rhs.ptr(), ptr() );
        }
        return *this;
    }

    template< class T, std::size_t I >
    variant & move_assign_value( T && value )
    {
        if( index() == I )
        {
            *as<T>() = std::forward<T>( value );
        }
        else
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
            new( ptr() ) T( std::forward<T>( value ) );
            type_index = I;
        }
        return *this;
    }
#else
    template< class T, std::size_t I >
    variant & copy_assign_value( T const & value )
    {
        if( index() == I )
        {
            *as<T>() = value;
        }
        else
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
            new( ptr() ) T( value );
            type_index = I;
        }
        return *this;
    }

#endif // variant_CPP11_OR_GREATER

    void swap_value( std::size_t index, variant & rhs )
    {
        using std::swap;
        switch( index )
        {
            case 0: swap( this->get<0>(), rhs.get<0>() ); break;
            case 1: swap( this->get<1>(), rhs.get<1>() ); break;
            case 2: swap( this->get<2>(), rhs.get<2>() ); break;
            case 3: swap( this->get<3>(), rhs.get<3>() ); break;
            case 4: swap( this->get<4>(), rhs.get<4>() ); break;
            case 5: swap( this->get<5>(), rhs.get<5>() ); break;
            case 6: swap( this->get<6>(), rhs.get<6>() ); break;
            case 7: swap( this->get<7>(), rhs.get<7>() ); break;
            case 8: swap( this->get<8>(), rhs.get<8>() ); break;
            case 9: swap( this->get<9>(), rhs.get<9>() ); break;
            case 10: swap( this->get<10>(), rhs.get<10>() ); break;
            case 11: swap( this->get<11>(), rhs.get<11>() ); break;
            case 12: swap( this->get<12>(), rhs.get<12>() ); break;
            case 13: swap( this->get<13>(), rhs.get<13>() ); break;
            case 14: swap( this->get<14>(), rhs.get<14>() ); break;
            case 15: swap( this->get<15>(), rhs.get<15>() ); break;
            case 16: swap( this->get<16>(), rhs.get<16>() ); break;
            case 17: swap( this->get<17>(), rhs.get<17>() ); break;
            case 18: swap( this->get<18>(), rhs.get<18>() ); break;
            case 19: swap( this->get<19>(), rhs.get<19>() ); break;
            case 20: swap( this->get<20>(), rhs.get<20>() ); break;
            case 21: swap( this->get<21>(), rhs.get<21>() ); break;
            case 22: swap( this->get<22>(), rhs.get<22>() ); break;
            case 23: swap( this->get<23>(), rhs.get<23>() ); break;
            case 24: swap( this->get<24>(), rhs.get<24>() ); break;
            case 25: swap( this->get<25>(), rhs.get<25>() ); break;
            case 26: swap( this->get<26>(), rhs.get<26>() ); break;
            case 27: swap( this->get<27>(), rhs.get<27>() ); break;
            case 28: swap( this->get<28>(), rhs.get<28>() ); break;
            case 29: swap( this->get<29>(), rhs.get<29>() ); break;
        }
    }

private:
    enum { data_size  = detail::typelist_max< variant_types >::value };

#if variant_CPP11_OR_GREATER

    enum { data_align = detail::typelist_max_alignof< variant_types >::value };

    using aligned_storage_t = typename std::aligned_storage< data_size, data_align >::type;
    aligned_storage_t data;

#elif variant_CONFIG_MAX_ALIGN_HACK

    typedef union { unsigned char data[ data_size ]; } aligned_storage_t;

    detail::max_align_t hack;
    aligned_storage_t data;

#else
    typedef typename detail::typelist_max< variant_types >::type max_type;

    typedef variant_ALIGN_AS( max_type ) align_as_type;

    typedef union { align_as_type data[ 1 + ( data_size - 1 ) / sizeof(align_as_type) ]; } aligned_storage_t;
    aligned_storage_t data;

// #   undef variant_ALIGN_AS

#endif // variant_CONFIG_MAX_ALIGN_HACK

    type_index_t type_index;
};

template <class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool holds_alternative( variant<T0, T1, T2, T3, T4, T5, T6> const & v ) variant_noexcept
{
    return v.index() == v.template index_of<T>();
}

template< class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline R & get( variant<T0, T1, T2, T3, T4, T5, T6> & v, nonstd_lite_in_place_type_t(R) = in_place<R> )
{
    return v.template get<R>();
}

template< class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6  >
inline R const & get( variant<T0, T1, T2, T3, T4, T5, T6> const & v, nonstd_lite_in_place_type_t(R) = in_place<R> )
{
    return v.template get<R>();
}

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline typename variant_alternative< I, variant<T0, T1, T2, T3, T4, T5, T6> >::type &
get( variant<T0, T1, T2, T3, T4, T5, T6> & v, nonstd_lite_in_place_index_t(I) = in_place<I> )
{
    if ( I != v.index() )
    {
        throw bad_variant_access();
    }

    return v.template get<I>();
}

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline typename variant_alternative< I, variant<T0, T1, T2, T3, T4, T5, T6> >::type const &
get( variant<T0, T1, T2, T3, T4, T5, T6> const & v, nonstd_lite_in_place_index_t(I) = in_place<I> )
{
    if ( I != v.index() )
    {
        throw bad_variant_access();
    }

    return v.template get<I>();
}

template< class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline typename detail::add_pointer<T>::type
get_if( variant<T0, T1, T2, T3, T4, T5, T6> * pv, nonstd_lite_in_place_type_t(T) = in_place<T> )
{
    return ( pv->index() == pv->template index_of<T>() ) ? &get<T>( *pv ) : variant_nullptr;
}

template< class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline typename detail::add_pointer<const T>::type
get_if( variant<T0, T1, T2, T3, T4, T5, T6> const * pv, nonstd_lite_in_place_type_t(T) = in_place<T>)
{
    return ( pv->index() == pv->template index_of<T>() ) ? &get<T>( *pv ) : variant_nullptr;
}

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline typename detail::add_pointer< typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6> >::type >::type
get_if( variant<T0, T1, T2, T3, T4, T5, T6> * pv, nonstd_lite_in_place_index_t(I) = in_place<I> )
{
    return ( pv->index() == I ) ? &get<I>( *pv ) : variant_nullptr;
}

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline typename detail::add_pointer< const typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6> >::type >::type
get_if( variant<T0, T1, T2, T3, T4, T5, T6> const * pv, nonstd_lite_in_place_index_t(I) = in_place<I> )
{
    return ( pv->index() == I ) ? &get<I>( *pv )  : variant_nullptr;
}

template< class T0, class T1, class T2, class T3, class T4, class T5 >
inline void swap(
    variant<T0, T1, T2, T3, T4, T5> & a,
    variant<T0, T1, T2, T3, T4, T5> & b ) variant_noexcept
{
    a.swap( b );
}

// template <class Visitor, class... Variants>
// visit( Visitor&& vis, Variants&&... vars );

// The following visit is restricted  with respect to the standard.
// It uses the common idiom is to return anrhs variant:

template< class Visitor, class Variant >
inline Variant visit( Visitor const & vis, Variant const & v )
{
    if ( v.valueless_by_exception() )
    {
        throw bad_variant_access();
    }

    switch( v.index() )
    {
        case 0: return vis( get<0>( v ) );
        case 1: return vis( get<1>( v ) );
        case 2: return vis( get<2>( v ) );
        case 3: return vis( get<3>( v ) );
        case 4: return vis( get<4>( v ) );
        case 5: return vis( get<5>( v ) );
        case 6: return vis( get<6>( v ) );
        default: return Variant();
    }
}

namespace detail {

template< class Variant >
struct Comparator
{
    static inline bool equal( Variant const & v, Variant const & w )
    {
        switch( v.index() )
        {
            case 0: return get<0>( v ) == get<0>( w );
            case 1: return get<1>( v ) == get<1>( w );
            case 2: return get<2>( v ) == get<2>( w );
            case 3: return get<3>( v ) == get<3>( w );
            case 4: return get<4>( v ) == get<4>( w );
            case 5: return get<5>( v ) == get<5>( w );
            case 6: return get<6>( v ) == get<6>( w );
            default: return false;
        }
    }

    static inline bool less_than( Variant const & v, Variant const & w )
    {
        switch( v.index() )
        {
            case 0: return get<0>( v ) < get<0>( w );
            case 1: return get<1>( v ) < get<1>( w );
            case 2: return get<2>( v ) < get<2>( w );
            case 3: return get<3>( v ) < get<3>( w );
            case 4: return get<4>( v ) < get<4>( w );
            case 5: return get<5>( v ) < get<5>( w );
            case 6: return get<6>( v ) < get<6>( w );
            default: return false;
        }
    }
};

} //namespace detail

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool operator==(
    variant<T0, T1, T2, T3, T4, T5, T6> const & v,
    variant<T0, T1, T2, T3, T4, T5, T6> const & w )
{
    if      ( v.index() != w.index()     ) return false;
    else if ( v.valueless_by_exception() ) return true;
    else                                   return detail::Comparator< variant<T0, T1, T2, T3, T4, T5, T6> >::equal( v, w );
}

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool operator!=(
    variant<T0, T1, T2, T3, T4, T5, T6> const & v,
    variant<T0, T1, T2, T3, T4, T5, T6> const & w )
{
    return ! ( v == w );
}

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool operator<(
    variant<T0, T1, T2, T3, T4, T5, T6> const & v,
    variant<T0, T1, T2, T3, T4, T5, T6> const & w )
{
    if      ( w.valueless_by_exception() ) return false;
    else if ( v.valueless_by_exception() ) return true;
    else if ( v.index() < w.index()      ) return true;
    else if ( v.index() > w.index()      ) return false;
    else                                   return detail::Comparator< variant<T0, T1, T2, T3, T4, T5, T6> >::less_than( v, w );
}

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool operator>(
    variant<T0, T1, T2, T3, T4, T5, T6> const & v,
    variant<T0, T1, T2, T3, T4, T5, T6> const & w )
{
    return w < v;
}

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool operator<=(
    variant<T0, T1, T2, T3, T4, T5, T6> const & v,
    variant<T0, T1, T2, T3, T4, T5, T6> const & w )
{
    return ! ( v > w );
}

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool operator>=(
    variant<T0, T1, T2, T3, T4, T5, T6> const & v,
    variant<T0, T1, T2, T3, T4, T5, T6> const & w )
{
    return ! ( v < w );
}

} // namespace variants

using namespace variants;

} // namespace nonstd

#if variant_CPP11_OR_GREATER

// specialize the std::hash algorithm:

namespace std {

template<>
struct hash< nonstd::monostate >
{
    std::size_t operator()( nonstd::monostate ) const variant_noexcept
    {
        return 42;
    }
};

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
struct hash< nonstd::variant<T0, T1, T2, T3, T4, T5, T6> >
{
    std::size_t operator()( nonstd::variant<T0, T1, T2, T3, T4, T5, T6> const & v ) const variant_noexcept
    {
        return nonstd::variants::detail::hash( v );
    }
};

} //namespace std

#endif // variant_CPP11_OR_GREATER

#if variant_BETWEEN(variant_COMPILER_MSVC_VERSION, 10, 14 )
# pragma warning( pop )
#endif

#endif // have C++17 std::variant

#endif // NONSTD_VARIANT_LITE_HPP
