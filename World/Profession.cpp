#include "Profession.h"

Profession::Profession()
{
}


bool Profession::IsEquals(const Profession& rhs)
{
	if (rhs.GetId() != GetId())
		return false;

	return true;
}

Profession::~Profession()
{
}
