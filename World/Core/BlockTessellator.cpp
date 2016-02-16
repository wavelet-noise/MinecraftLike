// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "BlockTessellator.h"
#include "Sector.h"

//possibly must be moved into <renderstrats> file
//deserialize autoreg
#include "BlockRenderStratery.h"


void BlockTessellator::Load(const rapidjson::Value & val)
{
//   if (val.HasMember("strategy"))
//   {
//     const rapidjson::Value &stratVal = val["strategy"];
//     if(stratVal.HasMember("type"))
//     {
//       std::string type = stratVal["type"].GetString();
//       std::shared_ptr<IRenderBlockStrategy> irs = RenderBlockStrategyFactory::Get().Create(StringIntern(type));
//       irs->Load(stratVal);
//       SetStrategy(irs);
//     }
//     else
//     {
//       LOG(error) << "render strategy has no type";
//     }
// 
//     if (stratVal.HasMember("transparent"))
//     {
//       mTransparent = stratVal["transparent"].GetBool_();
//     }
//   }
}

