#pragma once
#include "Core/GameObject.h"
#include "Core/orders/OrderBus.h"

using PProfession = std::shared_ptr<class Profession>;

class Profession
{
public:
	Profession();

	virtual size_t GetId() const = 0;

	bool IsEquals(const Profession &rhs);

	virtual ~Profession();

	template <typename T_>
	static size_t Idfor()
	{
		static size_t result(Nextid());
		return result;
	}

	virtual void Perform(const GameObjectParams & params, PGameObject performer) = 0;
	virtual std::string Name() = 0;
	virtual std::string Description() = 0;
	virtual bool CanPeformOrder(POrder p) = 0;

	virtual PProfession Clone() = 0;

private:
	static size_t Nextid()
	{
		static size_t next_id(0);
		return next_id++;
	}
};

template<typename T>
struct NumberedProfession : Profession
{
	size_t GetId() const override final
	{
		return Idfor<T>();
	}

	std::string Name() override;
	std::string Description() override;
};

template <typename T>
std::string NumberedProfession<T>::Name()
{
	return T::Name();
}

template <typename T>
std::string NumberedProfession<T>::Description()
{
	return T::Description();
}
