// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef RenderModel_h__
#define RenderModel_h__


#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"


struct RenderModel
{
  // TODO: Проверки наличия компонентов и возврат успешности компилирования.
  void Compile() const;

  PCMesh mMeshes;
  PShader mShader;
  PTexture mTexture;

  glm::mat4 mModel;
};



#endif // RenderModel_h__