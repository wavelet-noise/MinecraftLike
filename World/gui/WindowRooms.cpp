#include "WindowRooms.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Core\World.h>
#include "WS.h"
#include <vector>
#include <Window/Window.h>
#include <imgui_user.h>

WindowRooms::WindowRooms()
{
}

void WindowRooms::Draw(glm::vec2 wsize, float gt)
{
	if (mOpen)
	{
		ImGui::SetNextWindowSize(static_cast<glm::vec2>(Window::GetSize())/4.f);
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
				goto unselect;
			}

			static std::vector<std::string> room_types_s = [&]() -> auto
			{
				std::vector<std::string> a;

				for(size_t i = 0 ; i < static_cast<size_t>(RoomType::ROOMTYPECOUNT); i++)
				{
					a.push_back(Room::TypeName(static_cast<RoomType>(i)));
				}

				return a;
			}();

			ImGui::Combo("", reinterpret_cast<int*>(&selected->type), room_types_s, static_cast<int>(RoomType::ROOMTYPECOUNT));
		}
	unselect:;
		ImGui::End();
	}
}
