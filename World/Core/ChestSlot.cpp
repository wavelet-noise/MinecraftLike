#include "ChestSlot.h"
#include <Render\TextureManager.h>
#include <imgui.h>
#include <Core\GameObject.h>

void ChestSlot::DrawGui(bool selected)
{
	if (!obj)
	{
		auto &atl = TextureManager::Get().GetTexture("white");
		auto &tex = std::get<0>(atl);
		auto &uv4 = std::get<1>(atl);

		auto uv = glm::vec2(uv4.x, uv4.y);
		auto uv2 = glm::vec2(uv4.z, uv4.w);

		ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex->GetId()), { 32,32 }, uv2, uv, -1, selected ? ImVec4{ 1, 1, 1, 1 } : ImVec4{ 0, 0, 0, 0 });
	}
	else
	{
		auto &atl = TextureManager::Get().GetTexture(obj->GetId());
		auto &tex = std::get<0>(atl);
		auto &uv4 = std::get<1>(atl);

		auto uv = glm::vec2(uv4.x, uv4.y);
		auto uv2 = glm::vec2(uv4.z, uv4.w);

		ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex->GetId()), { 32,32 }, uv2, uv, -1, selected ? ImVec4{ 1, 1, 1, 1 } : ImVec4{ 0, 0, 0, 0 });
		auto draw_list = ImGui::GetWindowDrawList();

		if (count >= 100)
			draw_list->AddText(ImGui::GetItemRectMax() - ImVec2(22, 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(int(count)).c_str());
		else if (count >= 10)
			draw_list->AddText(ImGui::GetItemRectMax() - ImVec2(16, 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(int(count)).c_str());
		else
			draw_list->AddText(ImGui::GetItemRectMax() - ImVec2(10, 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(int(count)).c_str());

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("%s x%g\n%s", obj->GetId().get().c_str(), count, obj->GetDescription().c_str());
		}
	}
}
