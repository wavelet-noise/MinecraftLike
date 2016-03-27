#include "Recipe.h"
#include <Serealize.h>
#include <list>
#include <Core\DB.h>
#include <imgui.h>
#include <render\TextureManager.h>

void RecipeIn::JsonLoad(const rapidjson::Value & val)
{
	id = val.Begin()->GetString();
	if (val.Capacity() >= 2)
		count = val[1].GetInt();
}

void RecipeOut::JsonLoad(const rapidjson::Value & val)
{
	id = val.Begin()->GetString();
	if(val.Capacity() >= 2)
		count = val[1].GetInt();
	if (val.Capacity() >= 3)
		chance = val[2].GetDouble();
}

std::string myreplace(std::string s, const std::string &toReplace, const std::string &replaceWith)
{
	const auto &t = s.find(toReplace);
	if (t == std::string::npos)
		return s;

	return s.replace(t, toReplace.length(), replaceWith);
}

void DrawSprite(const StringIntern &s)
{
	auto &atl = TextureManager::Get().GetTexture(s);
	auto &tex = std::get<0>(atl);
	auto &atluv = std::get<1>(atl);

	auto uv = glm::vec2(atluv.x, atluv.y);
	auto uv2 = glm::vec2(atluv.z, atluv.w);

	ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex->GetId()), { 32,32 }, uv2, uv);
}

void Recipe::DrawSome(const StringIntern &s)
{
	auto t = DB::Get().mObjects.find(s);
	if (t != DB::Get().mObjects.end())
	{
		auto &a = *t;
		DrawSprite(a.first);
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("%s\n%s", a.first.get().c_str(), std::get<0>(a.second)->GetDescription().c_str());
		}
	}
}

void Recipe::DrawGui()
{
	for (const auto &inp : input)
	{
		ImGui::SameLine();
		DrawSome(inp.id);
		auto draw_list = ImGui::GetWindowDrawList();

		if (inp.count >= 100)
			draw_list->AddText(ImGui::GetItemBoxMax() - ImVec2(22, 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(inp.count).c_str());
		else if (inp.count >= 10)
			draw_list->AddText(ImGui::GetItemBoxMax() - ImVec2(16, 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(inp.count).c_str());
		else
			draw_list->AddText(ImGui::GetItemBoxMax() - ImVec2(10, 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(inp.count).c_str());
	}

	ImGui::SameLine();
	DrawSprite(StringIntern("arrow_right"));

	for (const auto &out : output)
	{
		ImGui::SameLine();
		DrawSome(out.id);
		auto draw_list = ImGui::GetWindowDrawList();

		if (out.count >= 100)
			draw_list->AddText(ImGui::GetItemBoxMax() - ImVec2(22, 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(out.count).c_str());
		else if (out.count >= 10)
			draw_list->AddText(ImGui::GetItemBoxMax() - ImVec2(16, 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(out.count).c_str());
		else
			draw_list->AddText(ImGui::GetItemBoxMax() - ImVec2(10, 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(out.count).c_str());
	}
}

// разворачивает все строки tag_* в соответствующий массив id
std::list<PRecipe> Recipe::Expand()
{
	if (materials.empty())
		return std::list<PRecipe>();

	std::vector<StringIntern> expanded;
	for (const auto &s : materials)
	{
		if (s.get().find("tag_") != std::string::npos)
		{
			auto taglist = DB::Get().Taglist(s);
			expanded.reserve(expanded.size() + taglist.size());
			for (const auto &t : taglist)
			{
				expanded.push_back(t);
			}
			LOG(trace) << s.get() << " expanded to " << taglist.size() << " materials";
		}
	}

	std::list<PRecipe> list;
	for (const auto &s : expanded)
	{
		auto nr = std::make_shared<Recipe>(*this);
		for (auto &inp : nr->input)
		{
			inp.id = StringIntern(myreplace(inp.id.get(),"%material%", s));
		}

		for (auto &out : nr->output)
		{
			out.id = StringIntern(myreplace(out.id.get(), "%material%", s));
		}
		list.push_back(nr);
	}

	return list;
}

void Recipe::JsonLoad(const rapidjson::Value & val)
{
	JSONLOAD(NVP(input), NVP(output), NVP(tools), NVP(machine), NVP(duration), NVP(materials));
}
