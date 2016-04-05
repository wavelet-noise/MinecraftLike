#pragma once
#include <map>
#include <memory>
#include <vector>

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
	virtual size_t GetId() 
	{
		return Idfor<T>();
	}
};

struct EventSound : public NumberedEvent<EventSound>
{

};

struct EventOrderStart : public NumberedEvent<EventOrderStart>
{

};

struct EventOrderDone : public NumberedEvent<EventOrderDone>
{

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

	template<typename _Ty>
	void Publish(std::shared_ptr<Event> e)
	{
		back_channels[Event::Idfor<_Ty>()].push_back(e);
	}

	static EventBus &Get()
	{
		static EventBus instance;
		return instance;
	}

private:
	std::map<size_t, std::vector<std::shared_ptr<Event>>> back_channels, channels;
};