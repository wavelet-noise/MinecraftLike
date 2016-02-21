// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef SectorBase_h__
#define SectorBase_h__


#include <stdint.h>
#include <vector>
#include <limits>


template<class BlockType>
class SectorBase
{
public:

  BlockType &GetBlock(const SBPos &pos)
  {
    return GetBlock(cs::SBtoI(pos));
  }

  void SetBlock(const SBPos &pos, BlockType block)
  {
    if (block)
    {
      AddBlock(cs::SBtoI(pos), block);
    }
    else
    {
      RemoveBlock(cs::SBtoI(pos));
    }
  }

private:
  std::vector<uint8_t> mBlocks8;
  std::vector<uint16_t> mBlocks16;

  size_t mCountBlocks = 0;

  std::vector<BlockType> mUniqueBlocks;

private:
  inline size_t GetUnique(size_t index)
  {
    const auto size_max = std::numeric_limits<uint8_t>::max() - 1;
    return (mUniqueBlocks.size() <= size_max) ? mBlocks8[index] : mBlocks16[index];
  }

  inline void SetUnique(size_t index, size_t unique)
  {
    const auto size_max = std::numeric_limits<uint8_t>::max() - 1;
    if (mUniqueBlocks.size() <= size_max)
    {
      mBlocks8[index] = static_cast<uint8_t>(unique);
    }
    else
    {
      mBlocks16[index] = static_cast<uint16_t>(unique);
    }
  }

  /// ¬ыделить пам€ть под блоки.
  inline void Create()
  {
    assert(mCountBlocks == 0);
    const size_t size_max = std::numeric_limits<uint8_t>::max() - 1;
    const size_t size_sec = SECTOR_SIZE * SECTOR_SIZE * SECTOR_SIZE;
    if (mUniqueBlocks.size() <= size_max)
    {
      mBlocks8.resize(size_sec);
    }
    else
    {
      mBlocks16.resize(size_sec);
    }
  }

  /// ќчистить пам€ть под блоки.
  inline void Clear()
  {
    assert(mCountBlocks == 0);
    const size_t size_max = std::numeric_limits<uint8_t>::max() - 1;
    if (mUniqueBlocks.size() <= size_max)
    {
      mBlocks8.clear();
      mBlocks8.shrink_to_fit();
    }
    else
    {
      mBlocks16.clear();
      mBlocks16.shrink_to_fit();
    }
  }

  void CheckResize(size_t oldSize)
  {
    // TODO. Ќеобходимо перейти на другую индексаци€ блоков.
    size_t currSize = mUniqueBlocks.size();

    const auto size_max = std::numeric_limits<uint8_t>::max() - 1;
    if (oldSize <= size_max && currSize > size_max)
    {

    }
    if (currSize <= size_max && oldSize > size_max)
    {

    }
  }

  inline void AddBlock(size_t index, BlockType block)
  {
    // Ѕлок не должен быть пустым.
    assert(block);
    // 1. »щем блок в списке уникальных блоков.
    // 2. »щем место под новый блок.
    // 3. ≈сли не нашли уникальный блок - добавл€ем блок в список уникальных.
    // 4. ƒобавл€ем индекс на уникальный блок.
    bool findedEmpty = false;
    bool findedUnique = false;
    size_t empty;
    size_t unique;
    for (size_t i = 0; i < mUniqueBlocks.size(); ++i)
    {
      if (mUniqueBlocks[i])
      {
        if (mUniqueBlocks[i] == block)
        {
          unique = i;
          findedUnique = true;
        }
      }
      else
      {
        empty = i;
        findedEmpty = true;
      }
    }

    if (!findedUnique)
    {
      if (!findedEmpty)
      {
        empty = mUniqueBlocks.size();
        mUniqueBlocks.resize(empty + 1, block);
        CheckResize(empty);
      }
      unique = empty;
    }

    if (!mCountBlocks)
    {
      Create();
    }
    ++mCountBlocks;
    SetUnique(index, unique + 1);
  }

  inline void RemoveBlock(size_t index)
  {
    // пробегаем по всем блокам, если блоков
    // данного типа больше нет в секторе, удал€ем из списка уникальных блоков.
    if (mCountBlocks == 0)
    {
      return;
    }
    auto unique = GetUnique(index);
    SetUnique(index, 0);

    bool isUnique = true;

    const size_t size_sec = SECTOR_SIZE * SECTOR_SIZE * SECTOR_SIZE;
    for (size_t i = 0; i < size_sec; ++i)
    {
      if (unique == GetUnique(i))
      {
        isUnique = false;
        break;
      }
    }

    if (isUnique)
    {
      mUniqueBlocks[unique] = nullptr;
      auto oldSize = mUniqueBlocks.size();
      size_t eraseCount = 0;
      for (auto &it = mUniqueBlocks.rbegin(); it != mUniqueBlocks.rend(); ++it)
      {
        if (*it)
        {
          break;
        }
        ++eraseCount;
      }
      if (eraseCount)
      {
        mUniqueBlocks.resize(mUniqueBlocks.size() - eraseCount);
        CheckResize(oldSize);
      }
    }

    --mCountBlocks;
    if (!mCountBlocks)
    {
      Clear();
    }
  }

  inline BlockType &GetBlock(size_t index)
  {
    if (mCountBlocks == 0)
    {
      return nullptr;
    }

    return mUniqueBlocks[GetUnique(index)];
  }

};




#endif // SectorBase_h__