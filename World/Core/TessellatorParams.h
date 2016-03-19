



#pragma once
#ifndef TessellatorParams_h__
#define TessellatorParams_h__

#include <glm/glm.hpp>
#include "../tools/CoordSystem.h"

struct TessellatorParams
{
  class Tessellator *tesselator;
  class SectorTessellator *sector;
  SPos spos;
  WBPos wbpos;
};


#endif // TessellatorParams_h__