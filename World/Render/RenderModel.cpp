



#include "RenderModel.h"

void RenderModel::Compile() const
{
  mMeshes->Presend(*mShader);
  mMeshes->Send(*mShader);
}


