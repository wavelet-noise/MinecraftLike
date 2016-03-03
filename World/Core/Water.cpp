#include "Water.h"
#include <Serealize.h>
#include <Core\World.h>
#include <Core\Tessellator.h>
#include <Core\WaterBlockTesselator.h>

Water::Water()
  : Agent(nullptr, "Water", "")
{
}

void Water::JsonLoad(const rapidjson::Value &val)
{

}

PAgent Water::Clone(GameObject * parent, const std::string &)
{
  return std::make_shared<Water>(*this);
}

void Water::Update(const GameObjectParams & params)
{
  static StringIntern water("water");
  static glm::vec3 offsets[] = {{0,0,-1}, {1,0,0}, {-1,0,0}, {0,1,0}, {0,-1,0}, {0,0,1}};
  auto mixer = [&](const glm::vec3 &off) -> bool {
    if (fill > 0)
    {
      auto down = params.world->GetBlock(params.pos + off);
      if (down->GetId() == water)
      {
        auto dwater = down->GetFromFullName<Water>("Water");
        if (float t = dwater->fill < 1)
        {
          if (t + fill > 1)
          {
            fill -= 1 - t;
            dwater->fill = 1;
          }
          else
          {
            dwater->fill += fill;
            fill = 0;
          }
        }
      }
      return true;
    }
    return false;
  };

  bool edited = false;
  for (const auto &o : offsets)
  {
    edited |= mixer(o);
  }

  if (edited)
  {
    params.world->GetTessellator()->SayChanged(params.sector->GetPos());
    auto wbt = std::make_shared<WaterBlockTessellator>();
    wbt->SayDepth(fill);
    params.world->GetTessellator()->Set(params.pos, wbt);
  }
}

