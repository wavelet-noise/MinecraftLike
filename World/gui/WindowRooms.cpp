#include "WindowRooms.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Core\World.h>
#include "WS.h"

WindowRooms::WindowRooms()
{
}

void WindowRooms::Draw(glm::vec2 wsize, float gt)
{
	if (mOpen)
	{
		//ImGui::SetNextWindowSize()
		ImGui::Begin("Room managment", &mOpen);
		for(const auto & c : WS::Get().w->rooms)
		{
			if (ImGui::Selectable(c->name.c_str(), selected == c, ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (ImGui::IsMouseDoubleClicked(0))
					Settings::Get().LookAt((c->min + c->max) / 2.f);
				selected = c;
			}
		}

		if (selected)
		{
			ImGui::Separator();
			char buf[100] = { 0 };
			strcpy_s(buf, &selected->name[0]);
			ImGui::InputText("Name", buf, 100);
			selected->name = buf;
			if (ImGui::Button("Remove"))
			{
				WS::Get().w->rooms.remove(selected);
				selected = nullptr;
			}
		}
		ImGui::End();
	}
}
