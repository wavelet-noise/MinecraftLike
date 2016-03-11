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
  SectorBase()
  {
    mUniqueBlocks.emplace_back(nullptr);
  }

  BlockType GetBlock(const SBPos &pos)
  {
    if (mCountBlocks == 0)
    {
      return mUniqueBlocks[0];
    }
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

protected:
  std::vector<IndexType> mBlocks;
  std::vector<BlockType> mUniqueBlocks;

  size_t mCountBlocks = 0;

protected:
  inline void AddBlock(size_t index, BlockType block)
  {
    // Блок не должен быть пустым.
    assert(block);
    // 1. Ищем блок в списке уникальных блоков.
    // 2. Ищем место под новый блок.
    // 3. Если не нашли уникальный блок - добавляем блок в список уникальных.
    // 4. Добавляем индекс на уникальный блок.
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
      else if (i != 0)
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
        mUniqueBlocks.push_back(block);
      }
      unique = empty;
    }

    if (!mCountBlocks)
    {
      mBlocks.resize(SECTOR_CAPACITY);
    }

    ++mCountBlocks;
    mBlocks[index] = static_cast<IndexType>(unique);
  }

  inline void RemoveBlock(size_t index)
  {
    // пробегаем по всем блокам, если блоков
    // данного типа больше нет в секторе, удаляем из списка уникальных блоков.
    if (mCountBlocks == 0)
    {
      return;
    }
    auto unique = mBlocks[index];
    if (unique == 0)
    {
      return;
    }
    mBlocks[index] = 0;

    bool isUnique = true;

    const size_t size_sec = SECTOR_CAPACITY;
    for (size_t i = 0; i < size_sec; ++i)
    {
      if (unique == mBlocks[i])
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
        // TODO. Освобождать память, если capacity > size на N %.
        mUniqueBlocks.resize(mUniqueBlocks.size() - eraseCount);
        mUniqueBlocks.shrink_to_fit();
      }
    }

    --mCountBlocks;
    if (!mCountBlocks)
    {
      mBlocks.clear();
      mBlocks.shrink_to_fit();
    }
  }

  inline BlockType GetBlock(size_t index)
  {
    assert(!mBlocks.empty());
    return mUniqueBlocks[mBlocks[index]];
  }

};




#endif // SectorBase_h__