#pragma once
#include "Profession.h"
class Medic : public NumberedProfession<Medic>
{
public:
	bool CanPeformOrder(POrder p) override;
	std::string Name();
	std::string Description();
	void Perform(const GameObjectParams& params, PGameObject performer) override;
	PProfession Clone() override;
	float GetBaseCost() const;
};

REGISTER_PROFESSION(Medic)
