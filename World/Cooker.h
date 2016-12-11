#pragma once
#include "Profession.h"
class Cooker : public NumberedProfession<Cooker>
{
public:
	bool CanPeformOrder(POrder p) override;
	std::string Name() override;
	std::string Description() override;
	void Perform(const GameObjectParams& params, PGameObject performer) override;
	PProfession Clone() override;
	float GetBaseCost() const override;
};

REGISTER_PROFESSION(Cooker)