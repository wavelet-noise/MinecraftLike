// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef Sector_h__
#define Sector_h__

#include <array>
#include <glm/glm.hpp>

#include "../tools/CoordSystem.h"
#include "Block.h"
#include "Config.h"
#include "DB.h"

#include <boost\serialization\serialization.hpp>
#include <list>
#include "SectorBase.h"


class Sector : public SectorBase<PBlock>
{
public:
  Sector(const SPos &position);
  ~Sector();

  const SPos &GetPos() const;

  /// Установить блок в заданную позицию.
  /// Существующий блок будет удален.
  void SetBlock(const SBPos &pos, PBlock block);

  void Update(class World *world);

  void Draw(class Tessellator *tess);

private:
  SPos mPos;

  class Tessellator *mTessellator = nullptr;

//   friend class boost::serialization::access;
// 
//   template<class Archive>
//   void save(Archive &ar, const unsigned int) const
//   {
//     for (int i = 0; i < SECTOR_CAPACITY; ++i)
//     {
//       ar << std::string(mBlocks[i]->GetId());
//     }
//   }
// 
//   template<class Archive>
//   void load(Archive &ar, const unsigned int)
//   {
//     for (int i = 0; i < SECTOR_CAPACITY; ++i)
//     {
//       std::string b;
//       ar >> b;
// 
//       mBlocks[i] = DB::Get().Create(StringIntern(b));
//     }
//   }
// 
//   BOOST_SERIALIZATION_SPLIT_MEMBER()

};

// namespace boost {
//   namespace serialization {
//     template<class Archive>
//     inline void save_construct_data(Archive &ar, const Sector *t, const unsigned int)
//     {
//       ar << t->GetPos();
//     }
// 
//     template<class Archive>
//     inline void load_construct_data(Archive &ar, Sector *t, const unsigned int)
//     {
//       glm::vec3 spos;
//       ar >> spos;
// 
//       new (t) Sector(spos);
//     }
//   }
// }


#endif // Sector_h__