#pragma once
#include "Settings.h"
#include <map>
#include <memory>
#include <type_traits>
#include "GameObjectParams.h"
#include "Agent.h"
#include "..\tools\StringIntern.h"

using PGameObject = std::shared_ptr<GameObject>;

template<class T, class... Args>
inline std::shared_ptr<T> MakeGameObject(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

class GameObject
{
public:
	GameObject(const StringIntern &__id);
	virtual ~GameObject();

	virtual void Update(GameObjectParams &params);

	// client
	// рисует gui этого агента дл€ переданного в параметрах блока. ƒолжен вызыватьс€ каждый кадр, когда требуетс€ отрисовка окна
	virtual void DrawGui();

	//client/server syncronize
	virtual void Interact(InteractParams &params);

	virtual void Requirements();

	//client/server paralell
	//выполн€етс€ 1 раз дл€ каждого агента каждого игрового объекта, хран€щегос€ в базе данных, после полной загрузки последней
	virtual void Afterload();

	//вызыветс€ дл€ блоков, перед тем как они будут имключены из сектора
	virtual void OnDestroy(const GameObjectParams &params);

	//вызыветс€ дл€ блоков, после того, как они добавлены в сектор
	virtual void OnCreate(const GameObjectParams & params);

	//вызыветс€ после того, как сосед изменилс€
	virtual void OnAdjacentChanged(const GameObjectParams & params);

	virtual PGameObject Clone();

	std::string GetDescription();

	template<class T>
	T *GetAgent()
	{
		StringIntern name = T::TypeName();
		auto it = mAgents.find(name);
		if (it != mAgents.end())
		{
			return static_cast<T*>(it->second.get());
		}

		return nullptr;
	}

	void PushAgent(PAgent go);
	inline bool IsPlacable()
	{
		return placable;
	}

	inline bool IsWalkable()
	{
		return walkable;
	}

	bool operator == (const GameObject &other) const
	{
		if (&other == this)
			return true;

		return other.id == id; //TODO: correct way
	}

	bool operator != (const GameObject &other) const
	{
		return !(other == *this);
	}

	StringIntern GetId();


protected:
	friend class DB;
	friend class World;
	std::map<StringIntern, PAgent> mAgents;

	friend class TemplateItemMaterial;
	friend class TemplateItemMaterialBase;
	StringIntern id;
	bool placable = false;
	bool walkable = false;
};