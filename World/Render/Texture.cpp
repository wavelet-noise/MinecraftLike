// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Texture.h"
#include <GL\glew.h>
#include "OpenGLCall.h"
#include <exception>
#include <tools\Log.h>

Texture::Texture(const Bitmap &bitmap, bool mip, bool smoothing, TextureDim dim, const glm::vec3 &size)
  : mTextureId(0), mSize(bitmap.GetSize()), mMip(mip)
{
  switch (dim)
  {
  case TEXTURE_DIM_1:
    mdim = GL_TEXTURE_1D;
    break;
  case TEXTURE_DIM_2:
    mdim = GL_TEXTURE_2D;
    break;
  case TEXTURE_DIM_3:
    mdim = GL_TEXTURE_3D;
    break;
  default:
    LOG(fatal) << "logic error";
    throw;
  }

  if(!mSize.x || !mSize.y || !IsPow2(mSize.x) || !IsPow2(mSize.y))
  {
    throw "Texture not created. Incorrect size.";
  }

  GL_CALL(glGenTextures(1, &mTextureId));
  if(!mTextureId)
  {
    throw "Texture not created. GL error.";
  }

  GL_CALL(glBindTexture(mdim, mTextureId));

  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
  if(dim > TEXTURE_DIM_1)
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
  if (dim > TEXTURE_DIM_2)
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT));

  if (mip)
  {
    GL_CALL(glTexParameteri(mdim, GL_TEXTURE_MIN_FILTER, smoothing ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST));
    GL_CALL(glTexParameteri(mdim, GL_TEXTURE_MAG_FILTER, smoothing ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST));
  }
  else
  {
    GL_CALL(glTexParameteri(mdim, GL_TEXTURE_BASE_LEVEL, 0));
    GL_CALL(glTexParameteri(mdim, GL_TEXTURE_MAX_LEVEL, 0));
    GL_CALL(glTexParameteri(mdim, GL_TEXTURE_MIN_FILTER, smoothing ? GL_LINEAR : GL_NEAREST));
    GL_CALL(glTexParameteri(mdim, GL_TEXTURE_MAG_FILTER, smoothing ? GL_LINEAR : GL_NEAREST));
  }

  if (size.x > 0)
  {
    switch (dim)
    {
    case TEXTURE_DIM_1:
      GL_CALL(glTexImage1D(mdim, 0, GL_RGBA, size.x, 0, GL_RGBA, GL_UNSIGNED_BYTE, &bitmap.GetRaw()[0]));
      break;
    case TEXTURE_DIM_2:
      GL_CALL(glTexImage2D(mdim, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, &bitmap.GetRaw()[0]));
      break;
    case TEXTURE_DIM_3:
      GL_CALL(glTexImage3D(mdim, 0, GL_RGBA, size.x, size.y, size.z, 0, GL_RGBA, GL_UNSIGNED_BYTE, &bitmap.GetRaw()[0]));
      break;
    default:
      LOG(fatal) << "logic error";
      throw;
    }
  }
  else // можем получить размеры только из 2д текстур
    GL_CALL(glTexImage2D(mdim, 0, GL_RGBA, mSize.x, mSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, &bitmap.GetRaw()[0]));
}

Texture::Texture() : mdim(GL_TEXTURE_2D)
{
  GL_CALL(glGenTextures(1, &mTextureId));
}

void Texture::GenMipmap()
{
  if (mMip)
    glGenerateMipmap(mdim);
}

Texture::~Texture()
{
  GL_CALL(glDeleteTextures(1, &mTextureId));
}

void Texture::DepthTexture(const glm::vec2 &size)
{
  mSize = size;
  glBindTexture(mdim, mTextureId);

  glTexParameteri(mdim, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(mdim, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameterf(mdim, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(mdim, GL_TEXTURE_WRAP_T, GL_CLAMP);

  glTexImage2D(mdim, 0, GL_DEPTH_COMPONENT, mSize.x, mSize.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
  glBindTexture(mdim, 0);
}

void Texture::Set(TextureSlot slot) const
{
  GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
  GL_CALL(glBindTexture(mdim, mTextureId));
}