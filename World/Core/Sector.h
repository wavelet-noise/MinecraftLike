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

class Sector;
class Creature;
using PSector = std::shared_ptr<Sector>;

class Sector : public SectorBase<PGameObject>
{
public:
	Sector(const SPos &position);
	Sector();
	~Sector();

	const SPos &GetPos() const;

	// Установить блок в заданную позицию.
	// Существующий блок будет удален.
	void SetBlock(const SBPos &pos, PGameObject block);
	void Spawn(const SBPos &position, PGameObject creature);
	void Place(const SBPos &position, PGameObject creature);
	void Repace(const SBPos &position, PGameObject creature);
	std::list<PGameObject> GetCreatures();

	void SayChanged();

	void Update(class World *world, float dt);

	void Draw(class Tessellator *tess);

	void SetSlise(int s);

	void BinLoad(std::istream& val);
	void BinSave(std::ostream& val) const;

private:
	SPos mPos;

	class Tessellator *mTessellator = nullptr;

	std::list<PGameObject> creatures;
	std::list<std::tuple<PGameObject, glm::vec3>> items;
	using ActiveStruct = std::tuple<std::weak_ptr<GameObject>, SBPos>;
	std::list<ActiveStruct> mActive; //only dynamic too
};