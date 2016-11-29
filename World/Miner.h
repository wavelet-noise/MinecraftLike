#pragma once
#include "Profession.h"

class Miner : public NumberedProfession<Miner>
{
public:
	bool CanPeformOrder(POrder p) override;
	std::string Name();
	std::string Description();
	void Perform(const GameObjectParams& params, PGameObject performer) override;
	PProfession Clone() override;
};

