#include "Xyz.h"
#include <Core\DB.h>
#include <Core\World.h>

void Xyz::JsonLoad(const rapidjson::Value & val)
{
}

PAgent Xyz::Clone(GameObject * parent, const std::string & name)
{
	return PAgent();
}

void Xyz::OnCreate(const GameObjectParams & params)
{
	params.world->SetBlock(params.pos + glm::vec3( 1,  0,  0), DB::Get().Create("x+"));
	params.world->SetBlock(params.pos + glm::vec3(-1,  0,  0), DB::Get().Create("x-"));
	params.world->SetBlock(params.pos + glm::vec3( 0,  1,  0), DB::Get().Create("y+"));
	params.world->SetBlock(params.pos + glm::vec3( 0, -1,  0), DB::Get().Create("y-"));
	params.world->SetBlock(params.pos + glm::vec3( 0,  0,  1), DB::Get().Create("z+"));
	params.world->SetBlock(params.pos + glm::vec3( 0,  0, -1), DB::Get().Create("z-"));
}
