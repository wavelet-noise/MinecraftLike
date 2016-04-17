#pragma once
#include <list>
#include <memory>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <Core\GameObject.h>

using POrder = std::shared_ptr<class Order>;

class Order : public std::enable_shared_from_this<Order>
{
public:
	virtual ~Order() {}

	template <typename T_>
	inline static size_t Idfor()
	{
		static size_t result(Nextid());
		return result;
	}

	virtual size_t GetId() const = 0;

	virtual bool IsEquals(const Order &rhs) = 0;
	virtual std::string to_string() const;

	void Take();
	void Done();

	inline bool IsTaken() const
	{
		return mTaken;
	}

	inline bool IsDone() const
	{
		return mDone;
	}

private:
	static size_t Nextid()
	{
		static size_t next_id(0);
		return next_id++;
	}

	bool mTaken = false;
	bool mDone = false;
};

template<typename T>
struct NumberedOrder : public Order
{
	virtual size_t GetId() const
	{
		return Idfor<T>();
	}
};

struct OrderDig : public NumberedOrder<OrderDig>
{
	OrderDig(glm::vec3 v);
	std::string to_string() const override;
	glm::vec3 pos;

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderDig &>(rhs);
		
		return o.pos == pos;
	}
};

struct OrderGet : public NumberedOrder<OrderGet>
{
	OrderGet(glm::vec3 v, PGameObject i);
	std::string to_string() const override;
	glm::vec3 pos;
	PGameObject item;

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderGet &>(rhs);

		return o.pos == pos && item == item;
	}
};

struct OrderWalk : public NumberedOrder<OrderWalk>
{
	OrderWalk(glm::vec3 v);
	std::string to_string() const override;
	glm::vec3 pos;
	PGameObject item;

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderWalk &>(rhs);

		return o.pos == pos;
	}
};

struct OrderWander : public NumberedOrder<OrderWander>
{
	OrderWander(glm::vec3 v);
	std::string to_string() const override;
	glm::vec3 pos;
	PGameObject item;

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderWander &>(rhs);

		return o.pos == pos;
	}
};

class OrderBus
{
public:
	std::list<POrder> orders, orders_taken;

	void IssueOrder(POrder ord);
	void Clear();

	void Update();

	static OrderBus &Get()
	{
		static OrderBus instance;
		return instance;
	}
};