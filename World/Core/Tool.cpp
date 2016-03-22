#include "Tool.h"
#include <Serealize.h>
#include <Render\Render.h>
#include <glm\gtx\transform.hpp>
#include <Core\World.h>
#include <Core\Foliage.h>

Tool::Tool()
	: Agent(nullptr, "Tool", "")
{
}

void Tool::JsonLoad(const rapidjson::Value &val)
{
	JSONLOAD(NVP(tool));
}

PAgent Tool::Clone(GameObject * parent, const std::string &name)
{
	auto t = MakeAgent<Tool>(*this);
	t->mParent = parent;
	return t;
}

void Tool::Update(const GameObjectParams & params)
{
	static size_t break_model = params.render->AddModel("data/models/selection.obj", "break", "shaders/basic.glsl");
	static float progress = 0;

	params.render->SetModelMatrix(break_model, glm::translate(glm::mat4(1), glm::vec3{ 9999.f }));
	if (tool == StringIntern("pickaxe"))
	{
		progress += params.dt;
		params.render->SetModelMatrix(break_model, glm::translate(glm::mat4(1), params.pos));
		if (progress >= 0.5)
		{
			progress = 0;
			params.world->SetBlock(params.pos, nullptr);
		}
	}
	else
		if (tool == StringIntern("hoe"))
		{
			progress += params.dt;

			if (progress >= 0.5)
			{
				progress = 0;
				auto b = params.world->GetBlock(params.pos);
				if (b->GetId() == StringIntern("dirt") || b->GetId() == StringIntern("grass"))
				{
					params.world->SetBlock(params.pos, DB::Get().Create("dirt_cultivated"));
				}
			}
		}
		else
			if (tool == StringIntern("watering_can"))
			{
				progress += params.dt;

				if (progress >= 0.5)
				{
					progress = 0;
					auto b = params.world->GetBlock(params.pos);
					if (b->GetId() == StringIntern("dirt_cultivated"))
					{
						params.world->SetBlock(params.pos, DB::Get().Create("dirt_cultivated_hydrated"));
					}
				}
			}
			else
				if (tool == StringIntern("sickle"))
				{
					progress += params.dt;

					if (progress >= 0.5)
					{
						progress = 0;
						for (int i = -1; i <= 1; ++i)
							for (int j = -1; j <= 1; ++j)
								for (int k = -1; k <= 1; ++k)
								{
									if (auto b = params.world->GetBlock(params.pos + glm::vec3(i, j, k)))
									{
										if (auto f = b->GetFromFullName<Foliage>("Foliage"))
										{
											params.world->SetBlock(params.pos + glm::vec3(i, j, k), nullptr);
										}
									}
								}
					}
				}
}
