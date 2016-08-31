



#pragma once
#ifndef Agent_h__
#define Agent_h__

#include "TemplateFactory.h"
#include <boost\noncopyable.hpp>
#include "..\tools\StringIntern.h"
#include <rapidjson\document.h>
#include "GameObjectParams.h"
#include <memory>
#include <type_traits>

namespace boost{
	namespace archive{
		class binary_oarchive;
	}
}

class GameObject;

using PAgent = std::shared_ptr<class Agent>;

template<class T, class... Args>
inline std::shared_ptr<T> MakeAgent(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

struct AgSync
{
	float elapsed;
	bool first;
	bool executed;
};

#ifdef _MSC_VER > 0
#define AM_NOVTABLE __declspec(novtable)
#elif
#define AM_NOVTABLE /*__declspec(novtable)*/
#endif

// Агент.
// Тип агента задается потомками на этапе компиляции и не меняется. Аналогичен типу объекта.
// Имя агента задается потомками в конструкторе. Может использоваться для идентификации
// экземпляров агентов. Также может отсутствовать.
// У агентов, которые обязательно присутствуют в игровом объекте, имя должно отсутствовать.
AM_NOVTABLE class Agent
{
public:
	Agent() = default;
	virtual ~Agent() = default;

	virtual PAgent Clone(GameObject *parent, const std::string &name = "") = 0;

	// максимальный период вызова метода Update в секундах
	// 0 - не ограничено
	virtual float GetFreq() const
	{
		return 1.0f;
	}

	virtual void __Update(const GameObjectParams &params) = 0;
	virtual void __AfterUpdate() = 0;

	virtual void Update(const GameObjectParams &params);

	template <typename T>
	static AgSync & GetSync()
	{
		static AgSync ag;
		return ag;
	}

	virtual void Requirements();

	// client/server paralell
	// выполняется 1 раз для каждого агента каждого игрового объекта, хранящегося в базе данных, после полной загрузки последней
	virtual void Afterload(GameObject * parent);

	// client
	// рисует gui этого агента для переданного в параметрах блока. Должен вызываться каждый кадр, когда требуется отрисовка окна
	virtual void DrawGui(float gt);

	// client/server syncronize
	virtual void Interact(const InteractParams &params);

	virtual void OnDestroy(const GameObjectParams &params);

	virtual void OnCreate(const GameObjectParams & params);

	virtual void OnAdjacentChanged(const GameObjectParams & params);

	virtual StringIntern GetFullName() const = 0;

	// client/server paralell
	virtual void JsonLoad(const rapidjson::Value &val);

	void save(boost::archive::binary_oarchive& ar, const unsigned) const;

	void load(boost::archive::binary_oarchive& ar, const unsigned);

protected:
	GameObject *mParent;

	const StringIntern mTypeName;
	StringIntern mAgentName;
	StringIntern mFullName;
};

static void __req_helper()
{
}

template <typename Last>
static void __req_helper(const Last &last)
{
	__deserialize(last.first, last.second);
}

template <typename First, typename... Rest>
static void __req_helper(const First &first, const Rest&... rest)
{
	__deserialize(first.first, first.second);
	deserialize(rest...);
}

#define APPLY0(t, dummy)
#define APPLY1(t, a) t(a)
#define APPLY2(t, a, b) t(a) t(b)
#define APPLY3(t, a, b, c) t(a) t(b) t(c)
#define APPLY4(t, a, b, c, d) t(a) t(b) t(c) t(d)
#define APPLY5(t, a, b, c, d, e) t(a) t(b) t(c) t(d) t(e)
#define APPLY6(t, a, b, c, d, e, f) t(a) t(b) t(c) t(d) t(e) t(f)
#define APPLY7(t, a, b, c, d, e, f, g) t(a) t(b) t(c) t(d) t(e) t(f) t(g)
#define APPLY8(t, a, b, c, d, e, f, g, h) t(a) t(b) t(c) t(d) t(e) t(f) t(g) t(h)

#define NUM_ARGS_H1(dummy, x8, x7, x6, x5, x4, x3, x2, x1, x0, ...) x0
#define NUM_ARGS(...) NUM_ARGS_H1(dummy, ##__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define APPLY_ALL_H3(t, n, ...) APPLY##n(t, __VA_ARGS__)
#define APPLY_ALL_H2(t, n, ...) APPLY_ALL_H3(t, n, __VA_ARGS__)
#define APPLY_ALL(t, ...) APPLY_ALL_H2(t, NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

//#define __REQ_STRINGIFY(type) {if(type * temp_ag = mParent->GetAgent<type>()){;} else throw #type;}
#define REQUIRE(type) {if(type * temp_ag = mParent->GetAgent<type>()){;} else throw #type;}

#define REGISTER_AGENT(type) REGISTER_ELEMENT(type, AgentFactory::Get(), StringIntern(#type))
#define AGENT(type) virtual StringIntern GetFullName() const override { return StringIntern(#type); } \
					static StringIntern TypeName() { return StringIntern(#type); } \
                    virtual void __Update(const GameObjectParams &params) override { \
						auto & s = Agent::GetSync<type>();  \
						if(s.first) { s.first = false; s.elapsed += params.dt; } \
						if(s.elapsed >= GetFreq()) { Update({params.world, params.sector, params.pos, s.elapsed, params.render}); s.executed = true; }  \
					} \
				    virtual void __AfterUpdate() override { auto & s = Agent::GetSync<type>();  s.first = true; if(s.executed) { s.elapsed = 0.f; } s.executed = false; }

struct AgentFactory : public boost::noncopyable
{
	using FactoryType = TemplateFactory<StringIntern, Agent>;
	static FactoryType &Get();
};

#endif // Agent_h__