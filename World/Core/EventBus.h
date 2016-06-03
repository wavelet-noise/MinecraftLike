#pragma once
#include <map>
#include <memory>
#include <vector>
#include <GL\glew.h>
#include <boost\format.hpp>
#include <glm\glm.hpp>
#include <core\Sector.h>
#include <core\GameObject.h>
#include <Core\OrderBus.h>

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

	virtual std::string to_string() const = 0;

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
	std::string to_string() const override
	{
		return std::string("EventSound:");
	}
};

struct EventOrderStart : public NumberedEvent<EventOrderStart>
{
	EventOrderStart(POrder p) : ord(p) {}
	POrder ord;

	std::string to_string() const override
	{
		if (!ord)
			return "EventOrderStart: none";
		return (boost::format("EventOrderStart: %1%") % ord->to_string()).str();
	}
};

struct EventOrderDone : public NumberedEvent<EventOrderDone>
{
	EventOrderDone(POrder p) : ord(p) {}
	POrder ord;

	std::string to_string() const override
	{
		if(!ord)
			return "EventOrderDone: none";
		return (boost::format("EventOrderDone: %1%") % ord->to_string()).str();
	}
};

struct EventSectorReady : public NumberedEvent<EventOrderStart>
{
	EventSectorReady(std::shared_ptr<Sector> p) : sec(p) {}
	std::shared_ptr<Sector> sec;

	std::string to_string() const override
	{
		if (!sec)
			return "EventSectorReady: none";
		return (boost::format("EventSectorReady: pos = {%1%, %2%, %3%}") % sec->GetPos().x % sec->GetPos().y % sec->GetPos().z).str();
	}
};

struct EventCreatureSpawn : public NumberedEvent<EventCreatureSpawn>
{
	EventCreatureSpawn(PGameObject p) : obj(p) {}
	PGameObject obj;

	std::string to_string() const override
	{
		return (boost::format("EventCreatureSpawn: obj = %1%") % obj->GetId()).str();
	}
};

struct EventItemPlace : public NumberedEvent<EventItemPlace>
{
	EventItemPlace(PGameObject p) : obj(p) {}
	PGameObject obj;

	std::string to_string() const override
	{
		return (boost::format("EventItemPlace: obj = %1%") % obj->GetId()).str();
	}
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