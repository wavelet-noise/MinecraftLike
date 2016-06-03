#pragma once
#include <list>
#include <memory>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <Core\GameObject.h>
#include <tools\CoordSystem.h>
#include <Core\Recipe.h>

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
	virtual glm::vec3 GetPos() const = 0;
	virtual std::string to_string() const;
	virtual float Tiring() const;
	virtual void Perform(const GameObjectParams & params, PGameObject performer);

	void Take();
	void Done();
	void Drop();

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

	virtual float Tiring() const override
	{
		return 0.3f;
	}

	virtual glm::vec3 GetPos() const
	{
		return pos;
	}

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderDig &>(rhs);
		
		return o.pos == pos;
	}

	void Perform(const GameObjectParams & params, PGameObject performer) override;
};

struct OrderGet : public NumberedOrder<OrderGet>
{
	OrderGet(glm::vec3 v, PGameObject i);
	std::string to_string() const override;
	glm::vec3 pos;
	PGameObject item;

	virtual float Tiring() const override
	{
		return 0.05f;
	}

	virtual glm::vec3 GetPos() const
	{
		return pos;
	}

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderGet &>(rhs);

		return o.pos == pos && item == item;
	}
};

struct OrderPlace : public NumberedOrder<OrderPlace>
{
	OrderPlace(WBPos v, PGameObject i);
	std::string to_string() const override;
	WBPos pos;
	PGameObject item;

	virtual float Tiring() const override
	{
		return 0.05f;
	}

	virtual glm::vec3 GetPos() const
	{
		return pos;
	}

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderPlace &>(rhs);

		return o.pos == pos && item == item;
	}
};

struct OrderDrop : public NumberedOrder<OrderDrop>
{
	OrderDrop(WBPos v, PGameObject i, int c);
	std::string to_string() const override;
	WBPos pos;
	PGameObject item;
	int count;

	virtual glm::vec3 GetPos() const
	{
		return pos;
	}

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderDrop &>(rhs);

		return o.pos == pos && item == item && count == o.count;
	}
};

struct OrderEat : public NumberedOrder<OrderEat>
{
	OrderEat(WBPos v, PGameObject i);
	std::string to_string() const override;
	WBPos pos;
	PGameObject item;

	virtual glm::vec3 GetPos() const
	{
		return pos;
	}

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderEat &>(rhs);

		return o.pos == pos && item == item;
	}
};

struct OrderWalk : public NumberedOrder<OrderWalk>
{
	OrderWalk(glm::vec3 v) : pos(v)
	{
	}

	std::string to_string() const override;

	glm::vec3 pos;
	PGameObject item;

	virtual glm::vec3 GetPos() const
	{
		return pos;
	}

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
	OrderWander(glm::vec3 v) : pos(v)
	{
	}

	std::string to_string() const override;

	glm::vec3 pos;
	PGameObject item;

	virtual glm::vec3 GetPos() const
	{
		return pos;
	}

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderWander &>(rhs);

		return o.pos == pos;
	}
};

struct OrderTalk : public NumberedOrder<OrderTalk>
{
	OrderTalk(PGameObject p) : person(p) 
	{
	}

	std::string to_string() const override;

	PGameObject person;
	float duration = 10.f;

	virtual glm::vec3 GetPos() const;

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderWander &>(rhs);

		return person == person;
	}

	void Perform(const GameObjectParams & params, PGameObject performer) override;
};

struct OrderCraft : public NumberedOrder<OrderCraft>
{
	OrderCraft(glm::vec3 v, PRecipe r, int c = 1) : pos(v), item(r), count(c)
	{
	}

	std::string to_string() const override;

	glm::vec3 pos;
	PRecipe item;
	int count = 1;

	virtual float Tiring() const override
	{
		return 0.1f;
	}

	virtual glm::vec3 GetPos() const
	{
		return pos;
	}

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderCraft &>(rhs);

		return o.pos == pos;
	}
};

struct OrderSleep : public NumberedOrder<OrderSleep>
{
	OrderSleep(glm::vec3 v) : pos(v){}

	std::string to_string() const override;

	glm::vec3 pos;

	virtual glm::vec3 GetPos() const
	{
		return pos;
	}

	virtual bool IsEquals(const Order &rhs) override
	{
		if (rhs.GetId() != GetId())
			return false;

		const auto &o = static_cast<const OrderSleep &>(rhs);

		return o.pos == pos;
	}
};

class OrderBus
{
public:
	std::list<POrder> orders, orders_taken, delayed_orders;

	void IssueDelayedOrder(POrder ord);

	void IssueOrder(POrder ord);
	void Clear();

	void Update(float dt);

	static OrderBus &Get()
	{
		static OrderBus instance;
		return instance;
	}
};