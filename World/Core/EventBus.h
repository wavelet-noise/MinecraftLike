#pragma once
#include <map>
#include <memory>
#include <vector>
#include <boost/format.hpp>
#include <glm/glm.hpp>
#include <core/Sector.h>
#include <core/GameObject.h>
#include <Core/orders/OrderBus.h>
#include <imgui.h>

using PEvent = std::shared_ptr<class Event>;

class Event
{
public:
	virtual ~Event() {}

	template <typename T_>
	inline static size_t Idfor()
	{
		static size_t result(Nextid());
		return result;
	}

	virtual size_t GetId() 
	{
		return -1;
	}

	virtual boost::optional<glm::vec3> GetPos()
	{
		return {};
	}

	virtual void DrawGui(float gt)
	{
		ImGui::Text("unimplemented event");
	}

private:
	static size_t Nextid()
	{
		static size_t next_id(0);
		return next_id++;
	}
};

template<typename T> 
struct NumberedEvent : public Event
{
	size_t GetId() override
	{
		return Idfor<T>();
	}
};

struct EventSound : public NumberedEvent<EventSound>
{
};

struct EventOrderStart : public NumberedEvent<EventOrderStart>
{
	EventOrderStart(POrder p) : ord(p) {}
	POrder ord;

};

struct EventOrderDone : public NumberedEvent<EventOrderDone>
{
	EventOrderDone(POrder p) : ord(p) {}
	POrder ord;

};

struct EventSectorReady : public NumberedEvent<EventOrderStart>
{
	EventSectorReady(std::shared_ptr<Sector> p) : sec(p) {}
	std::shared_ptr<Sector> sec;

};

struct EventCreatureSpawn : public NumberedEvent<EventCreatureSpawn>
{
	EventCreatureSpawn(PGameObject p) : obj(p) {}
	PGameObject obj;

	void DrawGui(float gt) override
	{
		ImGui::Text((boost::format("EventCreatureSpawn: obj = %1%") % obj->GetId()).str().c_str());
	}
};

struct EventCreatureDeath : public NumberedEvent<EventCreatureDeath>
{
	EventCreatureDeath(Goid p) : obj(p) {}
	Goid obj;
	WPos pos;

};

struct EventItemPlace : public NumberedEvent<EventItemPlace>
{
	EventItemPlace(PGameObject p) : obj(p) {}
	PGameObject obj;

};

class EventBus
{
public:
	void Update()
	{
		channels = back_channels;
		back_channels.clear();
	}

	template<typename _Ty>
	const std::vector<std::shared_ptr<Event>> &ListenChannel()
	{
		return channels[Event::Idfor<_Ty>()];
	}

	template<>
	const std::vector<std::shared_ptr<Event>> &ListenChannel<Event>()
	{
		static std::vector<std::shared_ptr<Event>> all;
		all.clear();

		for (const auto &e : channels)
		{
			all.insert(all.begin(), e.second.begin(), e.second.end());
		}

		return all;
	}

	template<typename _Ty, typename ... _Types>
	void Publish(_Types&& ... _Args)
	{
		back_channels[Event::Idfor<_Ty>()].push_back(std::make_shared<_Ty>(std::forward<_Types>(_Args)...));
	}

	static EventBus &Get()
	{
		static EventBus instance;
		return instance;
	}

private:
	std::map<size_t, std::vector<std::shared_ptr<Event>>> back_channels, channels;
};