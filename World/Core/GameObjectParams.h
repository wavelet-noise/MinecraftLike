



#pragma once
#ifndef GameObjectParams_h__
#define GameObjectParams_h__

#include <glm/glm.hpp>

struct GameObjectParams
{
  class World *world;
  class Sector *sector;
  glm::vec3 pos;
  float dt;
  class Render *render;
};

struct InteractParams
{
  class World *world;
  glm::vec3 pos;
  float dt;
};

#endif // GameObjectParams_h__