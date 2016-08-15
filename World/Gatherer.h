#pragma once
#include "Profession.h"
class Gatherer : public NumberedProfession<Gatherer>
{
public:
	bool CanPeformOrder(POrder p) override;
	static std::string Name();
	static std::string Description();
	void Perform(const GameObjectParams& params, PGameObject performer) override;
	PProfession Clone() override;
};

