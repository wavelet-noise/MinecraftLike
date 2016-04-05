#pragma once
#include <vector>
#include <memory>
#include <glm\glm.hpp>

class Order
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

private:
	static size_t Nextid()
	{
		static size_t next_id(0);
		return next_id++;
	}
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
	glm::vec3 pos;

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderDig &>(rhs);
		
		return o.pos == pos;
	}
};

class OrderBus
{
public:
	std::vector<std::shared_ptr<Order>> orders;

	void IssueOrder(std::shared_ptr<Order> ord);

	static OrderBus &Get()
	{
		static OrderBus instance;
		return instance;
	}
};