////////////////////////////////////////////////////////////
//
// LouEngine - Multiplatform Game Engine Project
// Copyright (C) 2016-2017 Jesus Lou (loucocito@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once

// Class that encapsulates a string ID, computed using
// the MurmurHash3 algorithm.
// The class can be implicitly constructed from strings
// although you may have to use explicit constructor
// when passing a pointer as CStrID argument.

// If the string passed to the constructor is a
// compile-time literal, the ID is resolved at compile-time.

// Each CStrID is 4 bytes in size.

#if defined(_DEBUG) || defined(PLATFORM_WINDOWS)
  #define STR_ID_DEBUG_STRINGS
#endif

class CStrID {
public:
  // Helper to disambiguate
  struct TConstCharWrapper;

  // Null id, equal to CStrID( "" )
  // (internally 0, false when used as boolean)
  CStrID( );
  
  // Construct/assign from a char array
  template <unsigned Len>
  CStrID( const char (&str)[Len] );
  template <unsigned Len>
  CStrID & operator=( const char (&str)[Len] );

  // Construct/assign from a char pointer
  // (wrapper disambiguates against compile-time version,
  // but forces you to use the explicit ctor when passed as argument)
  CStrID( const TConstCharWrapper & str );
  CStrID & operator=( const TConstCharWrapper & str );

  CStrID& operator=( const CStrID & s );
  bool operator==( const CStrID & s ) const;
  bool operator!=( const CStrID & s ) const;
  bool operator<( const CStrID & s ) const;
  bool operator>( const CStrID & s ) const;

  bool operator==( unsigned int v ) const;
  bool operator!=( unsigned int v ) const;
  bool operator<( unsigned int v ) const;
  bool operator>( unsigned int v ) const;

  ///////////////////////////////////////////
  ///////////////////////////////////////////
  ///////////////////////////////////////////
  ///////////////////////////////////////////
  // Users should not need to know
  // what lies beyond this line
  
  struct TConstCharWrapper {
    inline TConstCharWrapper( const char * s ): str( s ) { }
    const char * str;
  };

private:
  unsigned id;

  ///////////////////////////////////////////
  // Constants

  static const unsigned K0 = 0x85ebca6b;
  static const unsigned K1 = 0xc2b2ae35;
  static const unsigned K2 = 0xcc9e2d51;
  static const unsigned K3 = 0x1b873593;
  static const unsigned K4 = 0xe6546b64;


  ///////////////////////////////////////////
  // Bit shift/mix helpers

  static inline unsigned rotl32( unsigned x, unsigned r ) {
    return (x << r) | (x >> (32 - r));
  }

  static inline unsigned fmix32_sub( unsigned h, unsigned s ) {
    return ( h ^ ( h >> s ) );
  }

  static inline unsigned fmix32( unsigned h ) {
    return fmix32_sub( fmix32_sub( fmix32_sub( h, 16 ) * K0, 13 ) * K1, 16 );
  }


  ///////////////////////////////////////////
  // Compile-time version (recursive)
  // Some of what would normally be functions are structs with static member functions
  // (workaround, as c++ forbids partial specialization for plain functions)
  // NOTE: these functions may be called during run-time if called on a run-time filled
  // character array, hence the extra ifs, to halt on null character.

  struct TRet {
    unsigned l, h;
    inline TRet( unsigned al, unsigned ah ): l( al ), h ( ah ) { }
  };

  static inline unsigned murmur3_32_subHK( unsigned h, unsigned k ) {
    return rotl32( h ^ ( rotl32( k * K2, 15 ) * K3 ), 13 ) * 5 + K4;
  }
  
  template<unsigned Len, unsigned BytesLeft>
  struct Tmurmur3_32R { static inline TRet value( const TRet & r, const char (&str)[Len] ) {
    static const unsigned i0 = Len -1 -BytesLeft;
    if( !str[i0] )
      return r;
    if( !str[i0+1] )
      return TRet( r.l + 1, r.h ^ ( rotl32( str[i0] * K2, 15 ) * K3 ) );
    if( !str[i0+2] )
      return TRet( r.l + 2, r.h ^ ( rotl32( ( (str[i0]) | (str[i0+1] << 8) ) * K2, 15 ) * K3 ) );
    if( !str[i0+3] )
      return TRet( r.l + 3, r.h ^ ( rotl32( ( (str[i0]) | (str[i0+1] << 8) | (str[i0+2] << 16) ) * K2, 15 ) * K3 ) );
    return Tmurmur3_32R<Len, BytesLeft-4>::value( TRet( r.l + 4, murmur3_32_subHK( r.h, (str[i0]) | (str[i0+1] << 8) | (str[i0+2] << 16) | (str[i0+3] << 24) ) ), str );
  } };

  template<unsigned Len>
  struct Tmurmur3_32R<Len, 0> { static inline TRet value( const TRet & r, const char (&str)[Len] ) {
    return r;
  } };

  template<unsigned Len>
  struct Tmurmur3_32R<Len, 1> { static inline TRet value( const TRet & r, const char (&str)[Len] ) {
    static const unsigned i0 = Len - 2;
    if( !str[i0] )
      return r;
    return TRet( r.l + 1, r.h ^ ( rotl32( str[i0] * K2, 15 ) * K3 ) );
  } };

  template<unsigned Len>
  struct Tmurmur3_32R<Len, 2> { static inline TRet value( const TRet & r, const char (&str)[Len] ) {
    static const unsigned i0 = Len - 3;
    if( !str[i0] )
      return r;
    if( !str[i0+1] )
      return TRet( r.l + 1, r.h ^ ( rotl32( str[i0] * K2, 15 ) * K3 ) );
    return TRet( r.l + 2, r.h ^ ( rotl32( ( (str[i0]) | (str[i0+1] << 8) ) * K2, 15 ) * K3 ) );
  } };

  template<unsigned Len>
  struct Tmurmur3_32R<Len, 3> { static inline TRet value( const TRet & r, const char (&str)[Len] ) {
    static const unsigned i0 = Len - 4;
    if( !str[i0] )
      return r;
    if( !str[i0+1] )
      return TRet( r.l + 1, r.h ^ ( rotl32( str[i0] * K2, 15 ) * K3 ) );
    if( !str[i0+2] )
      return TRet( r.l + 2, r.h ^ ( rotl32( ( (str[i0]) | (str[i0+1] << 8) ) * K2, 15 ) * K3 ) );
    return TRet( r.l + 3, r.h ^ ( rotl32( ( (str[i0]) | (str[i0+1] << 8) | (str[i0+2] << 16) ) * K2, 15 ) * K3 ) );
  } };

  template<unsigned Len>
  static inline unsigned murmur3_32( const char (&str)[Len], unsigned seed = 0 ) {
    const TRet r( Tmurmur3_32R<Len, Len-1>::value( TRet( 0, seed ), str ) );
    return fmix32( r.h ^ r.l );
  }


  ///////////////////////////////////////////
  // Run-time version (iterative)

  static inline unsigned strlen( const char * str ) { const char * s = str; while( *s ) s++; return (unsigned)(s - str); }
  
  static unsigned runtime_murmur3_32( const char * str, unsigned len, unsigned seed = 0 );
};

inline CStrID::CStrID( ) 
: id( 0 )
{
}

template <unsigned Len>
inline CStrID::CStrID( const char (&str)[Len] ) {
  *this = str;
}

template <unsigned Len>
inline CStrID & CStrID::operator=( const char (&str)[Len] ) {
  id = murmur3_32( str );
#ifdef STR_ID_DEBUG_STRINGS
  registerDebugInternal( str, strlen( str ) );
#endif
  return *this;
}

inline CStrID::CStrID( const TConstCharWrapper & str ) {
  *this = str;
}

inline CStrID & CStrID::operator=( const TConstCharWrapper & str ) {
  if( !str.str ) {
    id = 0;
    return *this;
  }
  const unsigned l = strlen( str.str );
  id = runtime_murmur3_32( str.str, l );
  return *this;
}

inline CStrID& CStrID::operator=( const CStrID & s ) {
  id = s.id;
  return *this;
}

inline bool CStrID::operator==( const CStrID & s ) const {
  return id == s.id;
}

inline bool CStrID::operator!=( const CStrID & s ) const {
  return id != s.id;
}

inline bool CStrID::operator<( const CStrID & s ) const {
  return id < s.id;
}

inline bool CStrID::operator>( const CStrID & s ) const {
  return id > s.id;
}


inline bool CStrID::operator==( unsigned int v ) const {
  return id == v;
}

inline bool CStrID::operator!=( unsigned int v ) const {
  return id != v;
}

inline bool CStrID::operator<( unsigned int v ) const {
  return id < v;
}

inline bool CStrID::operator>( unsigned int v ) const {
  return id > v;
}
