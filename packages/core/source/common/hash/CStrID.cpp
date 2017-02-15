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

#include <hash/CStrID.h>

unsigned CStrID::runtime_murmur3_32( const char * str, unsigned len, unsigned seed ) {
  const unsigned * data = (const unsigned *)str;

  unsigned h = seed;
  unsigned k;

  const unsigned c1 = 0xcc9e2d51;
  const unsigned c2 = 0x1b873593;

  // Body
  unsigned nbytes = len;
  while( nbytes > 3 ) {
#if defined( PLATFORM_IS_BIG_ENDIAN )
    str = (const char*)data++;
    k = str[0] | (str[1] << 8) | (str[2] << 16) | (str[3] << 24);
#else
    k = *data++;
#endif
    k = rotl32( k * c1, 15 ) * c2;
    h = rotl32( h ^ k, 13 ) * 5 + 0xe6546b64;
    nbytes -= 4;
  }

  // Tail
  str = (const char *)data;
  k = 0;
  switch( nbytes ) {
    case 3: k ^= str[2] << 16;
    case 2: k ^= str[1] << 8;
    case 1: k ^= str[0];
            h ^= rotl32( k * c1, 15 ) * c2;
  };
  
  return fmix32( h ^ len );
} 
