



#include "GameObject.h"
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/map.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/vector.hpp>

GameObject::GameObject(const StringIntern &__id) :id(__id)
{
}

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update(GameObjectParams &params)
{
	for (auto &agent : mAgents)
	{
		agent.second->__Update(params);
	}
}

void GameObject::DrawGui(float gt)
{
	for (auto &agent : mAgents)
	{
		agent.second->DrawGui(gt);
	}
}

void GameObject::Interact(InteractParams & params)
{
	for (auto &agent : mAgents)
	{
		agent.second->Interact(params);
	}
}

void GameObject::Requirements()
{
	for (auto &agent : mAgents)
	{
		agent.second->Requirements();
	}
}

void GameObject::Afterload()
{
	for (auto &agent : mAgents)
	{
		agent.second->Afterload(this);
	}
}

void GameObject::OnDestroy(const GameObjectParams &params)
{
	for (auto &agent : mAgents)
	{
		agent.second->OnDestroy(params);
	}
}

void GameObject::OnCreate(const GameObjectParams & params)
{
	for (auto &agent : mAgents)
	{
		agent.second->OnCreate(params);
	}
}

void GameObject::OnAdjacentChanged(const GameObjectParams & params)
{
	for (auto &agent : mAgents)
	{
		agent.second->OnAdjacentChanged(params);
	}
}

PGameObject GameObject::Clone()
{
	auto a = MakeGameObject<GameObject>(id);
	a->placable = placable;
	a->active = active;
	a->walkable = walkable;

	for (const auto &ag : mAgents)
	{
		a->mAgents[ag.first] = ag.second->Clone(a.get());
	}

	return a;
}

StringIntern GameObject::GetId()
{
	return id;
}

void GameObject::save(boost::archive::binary_oarchive& ar, const unsigned v) const
{
	ar << std::string(id);
	ar << mAgents.size();
	for (auto i = mAgents.begin(); i != mAgents.end(); ++i)
	{
		ar << i->second->GetFullName().get();

		boost::asio::streambuf sb;
		boost::archive::binary_oarchive oa(sb);
		i->second->save(oa, v);

		auto output = std::vector<char>(sb.size());
		memcpy(&output[0], boost::asio::buffer_cast<const char*>(sb.data()), sb.size());

		ar << sb.size();
		ar << output;
	}
}

void GameObject::load(boost::archive::binary_iarchive& ar, const unsigned v)
{
	std::string id;
	size_t size;
	ar >> id >> size;

	for(int i = 0; i < size; ++i)
	{
		std::string fullname;
		ar >> fullname;

		auto ag = AgentFactory::Get().Create(StringIntern(fullname));
		ag->mParent = this;

		int size;
		ar >> size;
		auto input = std::vector<char>(size);
		ar >> input;

		boost::iostreams::array_source sr(&input[0], size);
		boost::iostreams::stream<decltype(sr)> source(sr);
	    boost::archive::binary_iarchive ia(source);

		ag->load(ia, v);
	}
}

void GameObject::PushAgent(PAgent go)
{
	mAgents[go->GetFullName()] = go;
}

std::string GameObject::GetDescription()
{
	std::stringstream ss;

	ss << id;

	return ss.str();
}