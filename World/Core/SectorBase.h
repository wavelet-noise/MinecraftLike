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

  BlockType GetBlock(const SBPos &pos)
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

  void Foreach(const std::function<void(size_t index, BlockType &block)> &func)
  {
    if (mCountBlocks == 0)
    {
      return;
    }
    for (size_t i = 0; i < mBlocks.size(); ++i)
    {
      if (mBlocks[i] != 0)
      {
        func(i, mUniqueBlocks[mBlocks[i] - 1]);
      }
    }
  }


private:
  std::vector<uint16_t> mBlocks;
  std::vector<BlockType> mUniqueBlocks;

  size_t mCountBlocks = 0;

private:
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
      }
      unique = empty;
    }

    if (!mCountBlocks)
    {
      const size_t size = SECTOR_SIZE * SECTOR_SIZE * SECTOR_SIZE;
      mBlocks.resize(size);
    }

    ++mCountBlocks;
    mBlocks[index] = unique + 1;
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

    const size_t size_sec = SECTOR_SIZE * SECTOR_SIZE * SECTOR_SIZE;
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
      mUniqueBlocks[unique - 1] = nullptr;
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
    if (mCountBlocks == 0)
    {
      return nullptr;
    }

    if (mBlocks[index] == 0)
    {
      return nullptr;
    }

    return mUniqueBlocks[mBlocks[index] - 1];
  }

};




#endif // SectorBase_h__