#pragma once
#include <Core\Sector.h>
class GenStrategy
{
public:
	virtual ~GenStrategy()
	{
	}

	virtual void Generate(Sector &s) = 0;
	virtual void WorldPass(World &w) = 0;
};

