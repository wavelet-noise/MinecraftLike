#include "Tool.h"
#include <Serealize.h>
#include <Render\Render.h>
#include <glm\gtx\transform.hpp>
#include <Core\World.h>

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

	params.render->SetModelMatrix(break_model, glm::translate(glm::mat4(1), glm::vec3{9999.f}));
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
}
