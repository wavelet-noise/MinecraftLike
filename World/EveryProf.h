#pragma once
#include "Profession.h"

class EveryProf : public NumberedProfession<EveryProf>
{
public:
	bool CanPeformOrder(POrder p) override;
	std::string Name() override;
	std::string Description() override;
	void Perform(const GameObjectParams& params, PGameObject performer) override;
	PProfession Clone() override;
};

