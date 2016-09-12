#include "Ore.h"
#include <Serealize.h>
#include <Core\GameObject.h>
#include <imgui.h>
#include <Core\DB.h>
#include <Core\ChestSlot.h>

//void Ore::OrePart::save(boost::archive::binary_oarchive& ar, const unsigned) const
//{
//	ar & id & count & chance;
//}
//
//void Ore::OrePart::load(boost::archive::binary_iarchive& ar, const unsigned)
//{
//	std::string sid;
//	ar >> sid;
//	ar >> count;
//	ar >> chance;
//	id = sid;
//}

void Ore::JsonLoad(const rapidjson::Value &val)
{
	JSONLOAD(NVP(contains), NVP(size));
}

PAgent Ore::Clone(GameObject * parent, const std::string &name)
{
	auto t = MakeAgent<Ore>(*this);
	t->mParent = parent;
	return t;
}

void Ore::DrawGui(float gt)
{
	if (Settings::Get().IsDebug())
	{
		ImGui::Text((boost::format("%1% strikes left") % size).str().c_str());
	}
}

ChestSlot Ore::DigSome()
{
	if (contains.empty())
		return{};

	int t = rand() % contains.size();
	float ch = rand() / static_cast<float>(RAND_MAX);

	--size;
	if (contains[t].chance >= ch)
	{
		return ChestSlot{ DB::Get().Create(contains[t].id), float(contains[t].count) };
	}

	return {};
}

bool Ore::Expire() const
{
	return size <= 0;
}

//void Ore::save(boost::archive::binary_oarchive& ar, const unsigned) const
//{
//	ar & contains;
//	ar & size;
//}
//
//void Ore::load(boost::archive::binary_iarchive& ar, const unsigned)
//{
//	ar & contains;
//	ar & size;
//}
