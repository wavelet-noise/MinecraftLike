// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "RenderMeshVao.h"

#include <gl/glew.h>
#include "OpenGLCall.h"


RenderMeshVao::RenderMeshVao()
{

}


RenderMeshVao::~RenderMeshVao()
{
  GL_CALL(glDeleteBuffers(1, &mVbi));
  GL_CALL(glDeleteBuffers(1, &mVbo));
  GL_CALL(glDeleteVertexArrays(1, &mVao));
}

void RenderMeshVao::UseShader(const Shader *shader)
{
  mShader = shader;
}

void RenderMeshVao::SetAttribute(const std::vector<Attribute> &attribute)
{
  if (!mCreated)
  {
    Create();
  }

  glBindVertexArray(mVao);
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);

  mVertexSize = 0;
  for (const auto &attr : attribute)
  {
    mVertexSize += attr.size;
  }

  for (const auto &attr : attribute)
  {
    auto location = glGetAttribLocation(mShader->GetId() , attr.name);
    glVertexAttribPointer(location, attr.size / sizeof(float), GL_FLOAT, GL_FALSE, mVertexSize, (char *)NULL + attr.offset);
    glEnableVertexAttribArray(location);
  }
}

void RenderMeshVao::Compile(const float *vertex, size_t vertexCount, const size_t *index, size_t indexCount)
{
  if (!mCreated)
  {
    Create();
  }

  mIndexCount = indexCount;

  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, mVbo));
  GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertexCount * mVertexSize, vertex, GL_STATIC_DRAW));

  GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbi));
  GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), index, GL_STATIC_DRAW));
}

void RenderMeshVao::Draw() const
{
  if (mCreated)
  {
    GL_CALL(glBindVertexArray(mVao));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbi));
    GL_CALL(glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, NULL));
  }
}

void RenderMeshVao::Create()
{
  GL_CALL(glGenVertexArrays(1, &mVao));
  GL_CALL(glGenBuffers(1, &mVbo));
  GL_CALL(glGenBuffers(1, &mVbi));
  GL_CALL(glBindVertexArray(mVao));
  GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbi));
}
