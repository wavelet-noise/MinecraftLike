#include "WindowRecipe.h"
#include <Core\DB.h>

WindowRecipe::WindowRecipe()
{
}

void WindowRecipe::Draw(glm::vec2 mainwin_size)
{
	ImGui::Begin("Recipe & Usage", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (recipe)
	{
		ImGui::Text("Recipe");
		decltype(auto) t = DB::Get().GetRecipe(what);
		if (!t.empty())
			for (const auto &r : t)
			{
				r->DrawGui();
			}
	}
	else
	{
		ImGui::Text("Using");
		decltype(auto) t = DB::Get().GetUsing(what);
		if (!t.empty())
			for (const auto &r : t)
			{
				r->DrawGui();
			}
	}
	ImGui::End();
}

void WindowRecipe::ShowRecipe(const StringIntern & s)
{
	recipe = true;
	what = s;
}

void WindowRecipe::ShowUsing(const StringIntern & s)
{
	recipe = false;
	what = s;
}
