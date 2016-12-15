#pragma once
#include "Profession.h"
class Gatherer : public NumberedProfession<Gatherer>
{
public:
	bool CanPeformOrder(POrder p) override;
	std::string Name();
	std::string Description();
	void Perform(const GameObjectParams& params, PGameObject performer) override;
	PProfession Clone() override;
	float GetBaseCost() const;
};

REGISTER_PROFESSION(Gatherer)

