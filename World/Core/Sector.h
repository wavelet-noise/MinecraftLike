#pragma once
#include <array>
#include <GL\glew.h>
#include <glm/glm.hpp>

#include "../tools/CoordSystem.h"
#include "GameObject.h"
#include "Config.h"
#include "DB.h"

#include <boost\serialization\serialization.hpp>
#include <list>
#include "SectorBase.h"

class Creature;

class Sector : public SectorBase<PGameObject>
{
public:
  Sector(const SPos &position);
  ~Sector();

  const SPos &GetPos() const;

  // ���������� ���� � �������� �������.
  // ������������ ���� ����� ������.
  void SetBlock(const SBPos &pos, PGameObject block);

  void Spawn(const SBPos &position, PGameObject creature);
  void Place(const SBPos &position, PGameObject creature);
  void Repace(const SBPos &position, PGameObject creature);

  void SayChanged();

  void Update(class World *world, float dt);

  void Draw(class Tessellator *tess);

  void SetSlise(int s);

private:
  SPos mPos;

  class Tessellator *mTessellator = nullptr;

  std::list<PGameObject> creatures;
  std::list<std::tuple<PGameObject, glm::vec3>> items;

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