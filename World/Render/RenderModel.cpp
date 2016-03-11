// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "RenderModel.h"

void RenderModel::Compile() const
{
  mMesh->Presend(*mShader);
  mMesh->Send(*mShader);
}


