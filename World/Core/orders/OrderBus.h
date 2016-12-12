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

	enum class State
	{
		Initial,
		Builded,
		Performing,
		Done,
		Dropped
	};

	virtual size_t GetId() const = 0;

	virtual bool IsEquals(const Order &rhs) = 0;

	virtual glm::vec3 GetPos() const = 0;
	virtual std::string to_string() const;

	virtual float Tiring() const;
	virtual void Update(const GameObjectParams & params, PGameObject performer, float work = 0);

	virtual void SetState(State __state);
	virtual State GetState() const;

	void Take();
	virtual void Done();
	virtual void Drop();
	void Cancel(std::string __reason = "");

	bool IsTaken() const;
	bool IsDone() const;
	bool IsCanceled() const;

	virtual void Perform(const GameObjectParams & params, PGameObject performer, float work = 0) = 0;
	virtual void Rebuild(const GameObjectParams & params, PGameObject performer);
	virtual void Approach(const GameObjectParams & params, PGameObject performer);

private:
	static size_t Nextid();

	bool mTaken = false;
	bool mDone = false;
	bool mCanceled = false;
	std::string reason;

	std::list<glm::vec3> path;

protected:
	State state = State::Initial;
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

	static OrderBus& Get()
	{
		static OrderBus instance;
		return instance;
	}
};