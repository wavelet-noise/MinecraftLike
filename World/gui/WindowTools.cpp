#include "WindowTools.h"
#include <Core\OrderBus.h>
#include <GLFW\glfw3.h>
#include "WindowCraftQueue.h"

WindowTools::WindowTools()
{
}

void WindowTools::Draw(glm::vec2 wsize)
{

	if (mOpen)
	{
		ImGui::Begin("Tools", &mOpen, ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::Button("Global crear order"))
		{
			OrderBus::Get().Clear();
		}

		if (ImGui::TreeNode("Digging"))
		{
			ImGui::Checkbox("Dig ores", &Settings::Get().dig_ores);
			if (ImGui::Button("Dig square"))
			{
				selected = SelectedOrder::DIG_SQUARE;
			}

			if (ImGui::Button("Dig circle"))
			{
				selected = SelectedOrder::DIG_CIRCLE;
			}

			if (ImGui::Button("Dig line"))
			{

			}

			if (ImGui::Button("Dig pattern"))
			{

			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Building"))
		{
			if (ImGui::Button("Place block"))
			{
				selected = SelectedOrder::PLACE_BLOCK;
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Crafting"))
		{
			if (ImGui::Button("Craft one"))
			{
				selected = SelectedOrder::CRAFT_ONE;
			}

			if (ImGui::Button("Crafting queue"))
			{
				WindowCraftQueue::Get().SetVisibility(true);
			}

			ImGui::TreePop();
		}

		ImGui::End();
	}
}