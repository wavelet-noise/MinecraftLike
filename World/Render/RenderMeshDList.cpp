// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "RenderMeshDList.h"

#include <assert.h>
#include <gl/glew.h>
#include "OpenGLCall.h"
#include <string>

RenderMeshDList::RenderMeshDList()
{
  for (auto &i : mEnabled)
  {
    i = false;
  }
}


RenderMeshDList::~RenderMeshDList()
{
  if (mCreated)
  {
    GL_CALL(glDeleteLists(mList, 1));
  }
}

void RenderMeshDList::SetAttribute(const std::vector<Attribute> &attribute, const std::vector<int> &locations)
{
  mVertexSize = 0;
  for (const auto &attr : attribute)
  {
    if (std::string("vertex") == attr.name)
    {
      mAttribute[ATTRIBUTE_VERTEX] = attr;
      mEnabled[ATTRIBUTE_VERTEX] = true;
    }
    if (std::string("texture") == attr.name)
    {
      mAttribute[ATTRIBUTE_TEXTURE] = attr;
      mEnabled[ATTRIBUTE_TEXTURE] = true;
    }
    mVertexSize += attr.size;
  }
}

void RenderMeshDList::Send(const float *vertex, size_t vertexCount, const size_t *index, size_t indexCount)
{
  assert(vertex && "vertex pointer is null");
  assert(index && "index pointer is null");
  if (!mCreated)
  {
    GL_CALL(mList = glGenLists(1));
    mCreated = true;
  }

  /// Имеется массив вершин и индексов, нужно сформировать дисплейный список.
  /// Для этого нужно перейти к неиндексированному массиву.

  GL_CALL(glNewList(mList, GL_COMPILE));
  GL_CALL(glBegin(GL_TRIANGLES));
  for (size_t i = 0; i < indexCount; ++i)
  {
    if (mEnabled[ATTRIBUTE_TEXTURE])
    {
      GL_CALL(glTexCoord2fv(&vertex[(mVertexSize * index[i] + mAttribute[ATTRIBUTE_TEXTURE].offset) / sizeof(float)]));
    }
    if (mEnabled[ATTRIBUTE_VERTEX])
    {
      GL_CALL(glVertex3fv(&vertex[(mVertexSize * index[i] + mAttribute[ATTRIBUTE_VERTEX].offset) / sizeof(float)]));
    }
  }
  GL_CALL(glEnd());
  GL_CALL(glEndList());
}

void RenderMeshDList::Draw() const
{
  if (mCreated)
  {
    GL_CALL(glCallList(mList));
  }
}

