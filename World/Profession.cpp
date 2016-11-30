#include "Profession.h"
#include "Localize.h"

Profession::Profession()
{
}


bool Profession::IsEquals(const Profession& rhs) const
{
	if (rhs.GetId() != GetId())
		return false;

	return true;
}

std::string Profession::ProfessionLevelString(ProfessionLevel pl) const
{
	switch (pl)
	{
	case ProfessionLevel::Incapable:
		return translate("Incapable");
	case ProfessionLevel::Incompetent:
		return translate("Incompetent");
	case ProfessionLevel::Beginner:
		return translate("Beginner");
	case ProfessionLevel::Novice:
		return translate("Novice");
	case ProfessionLevel::Amateur:
		return translate("Amateur");
	case ProfessionLevel::Practitioner:
		return translate("Practitioner");
	case ProfessionLevel::Professional:
		return translate("Professional");
	case ProfessionLevel::Expert:
		return translate("Expert");
	case ProfessionLevel::Master:
		return translate("Master");
	case ProfessionLevel::High_Master:
		return translate("High Master");
	case ProfessionLevel::Grand_Master:
		return translate("Grand Master");
	case ProfessionLevel::Legendary:
		return translate("Legendary");
	case ProfessionLevel::MAX:
		return translate("MAX");
	default:
		return "ProfessionLevel_ERROR";
	}
}

Profession::~Profession()
{
}

ProfessionLevel Profession::GetLevel() const
{
	return pl;
}

void Profession::SetLevel(ProfessionLevel _pl)
{
	pl = _pl;
}

bool Profession::GetActive() const
{
	return active;
}

void Profession::SetActive(bool a)
{
	active = a;
}

float Profession::GetCost() const
{
	if (pl == ProfessionLevel::Amateur)
		return 0;

	return GetBaseCost() * glm::mix(1.f, 2.f, static_cast<float>(ProfessionLevel::MAX) / static_cast<float>(pl));
}

ProfessionFactory::FactoryType &ProfessionFactory::Get()
{
	static FactoryType object;
	return object;
}
