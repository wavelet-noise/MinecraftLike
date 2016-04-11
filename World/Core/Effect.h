#pragma once
#include <Core\GameObject.h>
#include <Core\PositionAgent.h>
#include <Core\TemplateFactory.h>

struct Effect
{
	virtual void Apply(PGameObject go) = 0;
	virtual void Remove(PGameObject go) = 0;
};

#define REGISTER_EFFECT(type) REGISTER_ELEMENT(type, EffectFactory::Get(), StringIntern(#type))

struct EffectFactory : public boost::noncopyable
{
	using FactoryType = TemplateFactory<StringIntern, Effect>;
	static FactoryType &Get()
	{
		static FactoryType ef;
		return ef;
	}
};

struct MoralEffect : public Effect
{
	float morale = 0;

	// Унаследовано через Effect
	virtual void Apply(PGameObject go) override
	{
		if (auto m = go->GetAgent<Morale>())
		{
			m->morale += morale;
		}
	}

	virtual void Remove(PGameObject go) override
	{
		if (auto m = go->GetAgent<Morale>())
		{
			m->morale -= morale;
		}
	}
};

REGISTER_EFFECT(MoralEffect)