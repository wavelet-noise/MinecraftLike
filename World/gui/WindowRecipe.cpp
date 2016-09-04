#include "WindowRecipe.h"
#include <Core\DB.h>

WindowRecipe::WindowRecipe()
{
}

void WindowRecipe::Draw(glm::vec2 mainwin_size, float gt)
{
	ImGui::SetNextWindowSize({ 500, 500 });

	if (mOpen)
	{
		ImGui::Begin("Recipe & Usage", &mOpen);
		switch (current)
		{
		case RECIPE:
		{
			ImGui::Text("Recipe");
			decltype(auto) t = DB::Get().GetRecipe(what);
			if (!t.empty())
				for (const auto &r : t)
				{
					r->DrawGui(gt);
				}
		}
		break;

		case USAGE:
		{
			ImGui::Text("Using");
			decltype(auto) t = DB::Get().GetUsing(what);
			if (!t.empty())
				for (const auto &r : t)
				{
					r->DrawGui(gt);
				}
		}
		break;

		case DEEP:
		{
			ImGui::Text("Deep recipe");
			if (!ready)
				dr = DB::Get().GetDeepRecipe(what, { StringIntern("material_iron"), StringIntern("material_birch") });
			ready = true;

			if (mOpen)
			{
				if (dr)
					dr->DrawGui(gt);
			}
		}
		break;
		}
		ImGui::End();
	}
}

void WindowRecipe::ShowRecipe(const StringIntern & s)
{
	current = RECIPE;
	what = s;
	Show();
}

void WindowRecipe::ShowUsing(const StringIntern & s)
{
	current = USAGE;
	what = s;
	Show();
}

void WindowRecipe::ShowDeepRecipe(const StringIntern & s)
{
	what = s;
	ready = false;
	current = DEEP;
	Show();
}