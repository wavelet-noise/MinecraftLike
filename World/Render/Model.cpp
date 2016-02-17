// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================

#include "Model.h"
#include "DBShaders.h"
#include <Serealize.h>

Model::Model()
{
  mShader = DBShaders::Get().GetShader("shaders/basic.glsl");
}

void Model::JsonLoad(const rapidjson::Value & val)
{
}
