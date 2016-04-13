#include "WindowTools.h"
#include <Core\OrderBus.h>
#include <GLFW\glfw3.h>

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
			if (ImGui::Button("Dig square"))
			{
				selected = SelectedOrder::DIG_SQUARE;
			}

			if (ImGui::Button("Dig circle"))
			{

			}

			if (ImGui::Button("Dig line"))
			{

			}

			if (ImGui::Button("Dig pattern"))
			{

			}

			ImGui::TreePop();
		}

		if (ImGui::Button("Collect items"))
		{
			selected = SelectedOrder::COLLECT_SQUARE;
		}

		ImGui::End();
	}
}
