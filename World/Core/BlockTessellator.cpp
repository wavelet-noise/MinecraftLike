// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "BlockTessellator.h"
#include "Sector.h"
#include <Serealize.h>

//possibly must be moved into <renderstrats> file
//deserialize autoreg
#include "SplitBlockTessellator.h"


void BlockTessellator::JsonLoad(const rapidjson::Value & val)
{
  std::string name;
  JSONLOAD(NVP(name), sge::make_nvp("transparent", mTransparent));
  mName = StringIntern(name);
}

BlockTessellatorFactory::FactoryType & BlockTessellatorFactory::Get()
{
  static FactoryType object;
  return object;
}
