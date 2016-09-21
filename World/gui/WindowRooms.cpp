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
	ImGui::Begin("Room managment", &mOpen);
	if (selected)
	{
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
