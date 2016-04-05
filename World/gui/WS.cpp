#include "WS.h"

void WS::Draw(glm::vec2 wsize)
{

	WindowPerfomance &wp = WindowPerfomance::Get();
	WindowInventory &winv = WindowInventory::Get();
	WindowDb &wdb = WindowDb::Get();
	WindowRecipe &wr = WindowRecipe::Get();
	WindowEventlog &we = WindowEventlog::Get();
	WindowTools &wt = WindowTools::Get();

	if (ImGui::IsKeyPressed(GLFW_KEY_F3))
	{
		wp.Toggle();
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_F5))
	{
		we.Toggle();
	}

	wp.Draw(wsize);
	wdb.Draw(wsize);
	winv.Draw(wsize);
	wr.Draw(wsize);
	we.Draw(wsize);
	wt.Draw(wsize);
}
