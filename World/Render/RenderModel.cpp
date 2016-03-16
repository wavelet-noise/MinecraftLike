// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "RenderModel.h"

void RenderModel::Compile() const
{
  mMeshes->Presend(*mShader);
  mMeshes->Compile(*mShader);
}


