



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
#include <string>

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

// Агент.
// Тип агента задается потомками на этапе компиляции и не меняется. Аналогичен типу объекта.
// Имя агента задается потомками в конструкторе. Может использоваться для идентификации
// экземпляров агентов. Также может отсутствовать.
// У агентов, которые обязательно присутствуют в игровом объекте, имя должно отсутствовать.
class Agent
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

	// client/server paralell
	// выполняется 1 раз для каждого агента каждого игрового объекта, хранящегося в базе данных, после полной загрузки последней
	virtual void Afterload(GameObject * parent);

	// client
	// рисует gui этого агента для переданного в параметрах блока. Должен вызываться каждый кадр, когда требуется отрисовка окна
	virtual void DrawGui();

	// client/server syncronize
	virtual void Interact(const InteractParams &params);

	virtual void OnDestroy(const GameObjectParams &params);

	virtual void OnCreate(const GameObjectParams & params);

	virtual void OnAdjacentChanged(const GameObjectParams & params);

	virtual StringIntern GetFullName() const = 0;

	// client/server paralell
	virtual void JsonLoad(const rapidjson::Value &val);

protected:
	GameObject *mParent;

	const StringIntern mTypeName;
	StringIntern mAgentName;
	StringIntern mFullName;
};


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