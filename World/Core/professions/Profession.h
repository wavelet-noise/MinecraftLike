#pragma once
#include "Core/GameObject.h"
#include "Core/orders/OrderBus.h"

using PProfession = std::shared_ptr<class Profession>;

enum class ProfessionLevel
{
	Incapable = 0,
	Incompetent = 1,
	Beginner = 2,
	Novice = 3,
	Amateur = 4,
	Practitioner = 5,
	Professional = 6,
	Expert = 7,
	Master = 8,
	High_Master = 9,
	Grand_Master = 10,
	Legendary = 11,
	MAX = 12
};

class Profession
{
public:
	Profession();

	virtual size_t GetId() const = 0;

	bool IsEquals(const Profession &rhs) const;

	std::string ProfessionLevelString(ProfessionLevel pl) const;

	virtual ~Profession();

	template <typename T_>
	static size_t Idfor()
	{
		static size_t result(Nextid());
		return result;
	}

	virtual std::string Name() = 0;
	virtual std::string Description() = 0;

	virtual void Perform(const GameObjectParams & params, PGameObject performer) = 0;
	virtual bool CanPeformOrder(POrder p) = 0;

	virtual float GetBaseCost() const = 0;

	float GetCost() const;

	ProfessionLevel GetLevel() const;
	void SetLevel(ProfessionLevel);

	virtual PProfession Clone() = 0;

	bool GetActive() const;
	void SetActive(bool);

private:
	static size_t Nextid()
	{
		static size_t next_id(1);
		return next_id++;
	}

	float cost = 0;
	ProfessionLevel pl = ProfessionLevel::Incapable;
	bool active = true;
};

template<typename T>
struct NumberedProfession : Profession
{
	size_t GetId() const override final
	{
		return Idfor<T>();
	}
};

#define REGISTER_PROFESSION(type) REGISTER_ELEMENT(type, ProfessionFactory::Get(), StringIntern(#type))

struct ProfessionFactory : public boost::noncopyable
{
	using FactoryType = TemplateFactory<StringIntern, Profession>;
	static FactoryType &Get();
};
