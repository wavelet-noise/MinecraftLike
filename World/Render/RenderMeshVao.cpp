



#include "RenderMeshVao.h"

#include <gl/glew.h>
#include <assert.h>


RenderMeshVao::RenderMeshVao()
{

}


RenderMeshVao::~RenderMeshVao()
{
  if (mCreated)
  {
    glDeleteBuffers(1, &mVbi);
    glDeleteBuffers(1, &mVbo);
    glDeleteVertexArrays(1, &mVao);
  }
}

void RenderMeshVao::SetAttribute(const std::vector<Attribute> &attribute, const std::vector<int> &locations)
{
  assert(attribute.size() == locations.size());
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

  for (size_t i = 0; i < attribute.size(); ++i)
  {
    glVertexAttribPointer(locations[i], attribute[i].size / sizeof(float), 
      GL_FLOAT, GL_FALSE, mVertexSize, (char *)NULL + attribute[i].offset);
    glEnableVertexAttribArray(locations[i]);
  }
}

void RenderMeshVao::Send(const float *vertex, size_t vertexCount, const size_t *index, size_t indexCount)
{
  if (!mCreated)
  {
    Create();
  }

  mIndexCount = indexCount;

  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, vertexCount * mVertexSize, vertex, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbi);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), index, GL_STATIC_DRAW);
}

void RenderMeshVao::Draw() const
{
  if (mCreated)
  {
    glBindVertexArray(mVao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbi);
    glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, NULL);
  }
}

void RenderMeshVao::Create()
{
  glGenVertexArrays(1, &mVao);
  glGenBuffers(1, &mVbo);
  glGenBuffers(1, &mVbi);
  glBindVertexArray(mVao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVbi);
  mCreated = true;
}
