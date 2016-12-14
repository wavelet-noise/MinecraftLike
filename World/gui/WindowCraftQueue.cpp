#include "WindowCraftQueue.h"
#include <Core\World.h>
#include <Core\agents\Chest.h>
#include <Render\TextureManager.h>
#include <DragNDrop.h>
#include "WS.h"

WindowCraftQueue::WindowCraftQueue()
{
}

void WindowCraftQueue::Draw(glm::vec2 mainwin_size, float gt)
{
	if (mOpen)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);

		ImGui::Begin("WindowCraftQueue", &mOpen, ImGuiWindowFlags_AlwaysAutoResize);
		{
			auto &c = WS::Get().w->GetRecipeOrders();
			if (c.empty())
			{
				ImGui::Text("No craft orders");
			}

			for (const auto &i : c)
			{
				bool b;
				i.recipe->DrawGui(gt,b);
				ImGui::SameLine();
				if (i.infinite)
					ImGui::Text("x inf");
				else
					ImGui::Text("x %d", i.elapsed);
			}
		}

		{
			auto &c = WS::Get().w->GetDelayedRecipeOrders();
			if (!c.empty())
			{
				ImGui::Text("Delayed:");
			}

			for (const auto &i : c)
			{
				bool b;
				i.recipe->DrawGui(gt,b);
				ImGui::SameLine();
				if (i.infinite)
					ImGui::Text("x inf");
				else
					ImGui::Text("x %d", i.elapsed);
			}
		}

		ImGui::End();
	}
}
