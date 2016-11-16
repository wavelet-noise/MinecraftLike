#include "WindowStorages.h"
#include <Core\World.h>
#include <Core\Chest.h>
#include <Render\TextureManager.h>
#include <DragNDrop.h>
#include "WS.h"

std::list<std::tuple<glm::vec3, PGameObject>>& Storages::List()
{
	return storages;
}

void Storages::AddStorage(glm::vec3 pos, PGameObject go)
{
	storages.push_back(make_tuple(pos, go));
}

void Storages::RemoveStorage(PGameObject go)
{
	//storages.remove(go);
}

WindowStorages::WindowStorages()
{
}

void WindowStorages::Draw(glm::vec2 mainwin_size, float gt)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);

	if (mOpen)
	{
		ImGui::Begin("Storages", &mOpen, ImGuiWindowFlags_AlwaysAutoResize);
		{
			const auto &all = Storages::Get().List();
			int inrow = 0;
			for(const auto &s : all)
			{
				auto ch = std::get<1>(s)->GetAgent<Chest>();
				ch->DrawGui(gt);
				inrow += ch->columns;
				if (inrow < 10)
					ImGui::SameLine();
				else
					inrow = 0;
			}
		}

		ImGui::End();
	}
}
