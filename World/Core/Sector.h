#pragma once
#include <array>
#include <GL\glew.h>
#include <glm/glm.hpp>

#include "../tools/CoordSystem.h"
#include "GameObject.h"
#include "Config.h"
#include "DB.h"

#include <list>
#include "SectorBase.h"

class Creature;

class Sector : public SectorBase<PGameObject>
{
public:
	Sector(const SPos &position);
	Sector();
	~Sector();

	const SPos &GetPos() const;

	// ���������� ���� � �������� �������.
	// ������������ ���� ����� ������.
	void SetBlock(const SBPos &pos, PGameObject block);
	void Spawn(const SBPos &position, PGameObject creature);
	void Place(const SBPos &position, PGameObject creature);
	void Repace(const SBPos &position, PGameObject creature);
	std::list<PGameObject> GetCreatures();

	void SayChanged();

	void Update(class World *world, float dt);

	void Draw(class Tessellator *tess);

	void SetSlise(int s);

	void BinLoad(std::istream& stream) const;
	void BinSave(std::ostream& stream);

private:
	SPos mPos;

	class Tessellator *mTessellator = nullptr;

	std::list<PGameObject> creatures;
	std::list<std::tuple<PGameObject, glm::vec3>> items;
	using ActiveStruct = std::tuple<std::weak_ptr<GameObject>, SBPos>;
	std::list<ActiveStruct> mActive; //only dynamic too
};

//namespace boost {
//  namespace serialization {
//    template<class Archive>
//    inline void save_construct_data(Archive &ar, const Sector *t, const unsigned int)
//    {
//      ar << t->GetPos();
//    }
//
//    template<class Archive>
//    inline void load_construct_data(Archive &ar, Sector *t, const unsigned int)
//    {
//      glm::vec3 spos;
//      ar >> spos;
//
//      new (t) Sector(spos);
//    }
//  }
//}