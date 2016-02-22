// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef Config_h__
#define Config_h__

#include <limits>
#include <stdint.h>
#include <tuple>


namespace detail
{
  using TypesSize = std::tuple<uint8_t, uint16_t, uint32_t, uint32_t>;

  constexpr size_t NumberOfBits(size_t x)
  {
    return x == 0 ? 0 : 1 + NumberOfBits(x >> 1);
  }

  constexpr size_t FixZero(size_t x)
  {
    return x == 0 ? 0 : x - 1;
  }

  template<size_t S>
  struct TypeFromSize
  {
    typedef typename std::tuple_element<FixZero(NumberOfBits(S)) / 8, TypesSize>::type type;
  };
}

enum
{
  SECTOR_SIZE = 32,
  SECTOR_CAPACITY = SECTOR_SIZE * SECTOR_SIZE * SECTOR_SIZE
};

using IndexType = detail::TypeFromSize<SECTOR_CAPACITY>::type;


#endif // Config_h__

