#pragma once
#include <list>
#include <memory>
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
	static size_t Idfor()
	{
		static size_t result(Nextid());
		return result;
	}

	virtual size_t GetId() const = 0;

	virtual bool IsEquals(const Order &rhs) = 0;
	virtual glm::vec3 GetPos() const = 0;
	virtual std::string to_string() const;
	virtual float Tiring() const;
	virtual void Perform(const GameObjectParams & params, PGameObject performer, float work = 0) = 0;

	void Take();
	virtual void Done();
	virtual void Drop();
	void Cancel(std::string __reason = "");

	bool IsTaken() const
	{
		return mTaken;
	}

	bool IsDone() const
	{
		return mDone;
	}

	bool IsCanceled() const
	{
		return mCanceled;
	}

private:
	static size_t Nextid()
	{
		static size_t next_id(0);
		return next_id++;
	}

	bool mTaken = false;
	bool mDone = false;
	bool mCanceled = false;
	std::string reason;
};

template<typename T>
struct NumberedOrder : Order
{
	size_t GetId() const override final
	{
		return Idfor<T>();
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