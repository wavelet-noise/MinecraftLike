#include "WS.h"
#include <Settings.h>

void WS::Draw(glm::vec2 wsize)
{

	WindowPerfomance &wp = WindowPerfomance::Get();
	WindowInventory &winv = WindowInventory::Get();
	WindowDb &wdb = WindowDb::Get();
	WindowRecipe &wr = WindowRecipe::Get();
	WindowEventlog &we = WindowEventlog::Get();
	WindowTools &wt = WindowTools::Get();
	WindowCraftQueue &wcq = WindowCraftQueue::Get();

	if (ImGui::IsKeyPressed(GLFW_KEY_F3))
	{
		wp.Toggle();
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_F5))
	{
		we.Toggle();
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_F11))
	{
		Settings::Get().debug = !Settings::Get().debug;
	}

	wp.Draw(wsize);
	wdb.Draw(wsize);
	winv.Draw(wsize);
	wr.Draw(wsize);
	we.Draw(wsize);
	wt.Draw(wsize);
	wcq.Draw(wsize);
}
