



#include "MeshPartialBlockGenerator.h"
#include <type_traits>
#include "..\Render\TextureManager.h"
#include <Serealize.h>

static glm::vec3 vertexCube[] =
{
  { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f }, // front
  { 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 0.0f }, // right 
  { 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 0.0f }, // back
  { 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 0.0f }, // left
  { 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f, 1.0f }, // top
  { 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f, 0.0f }  // bot
};

static char indexCube[] =
{
	0, 3, 4, 1, // front
	1, 4, 7, 5, // right 
	5, 7, 6, 2, // back
	2, 6, 3, 0, // left
	3, 6, 7, 4, // top
	2, 0, 1, 5  // bot
};

static glm::vec3 normalCube[] =
{
  {  0.0f, -1.0f,  0.0f }, // front
  {  1.0f,  0.0f,  0.0f }, // right 
  {  0.0f,  1.0f,  0.0f }, // back
  { -1.0f,  0.0f,  0.0f }, // left
  {  0.0f,  0.0f,  1.0f }, // top
  {  0.0f,  0.0f, -1.0f }, // bot
};

static size_t indexCubeSide[] =
{
  0, 3, 2, 2, 1, 0
};

static glm::vec2 textureCube[] =
{
  { 0.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f },{ 1.0f, 0.0f }
};


MeshPartialBlockGenerator::MeshPartialBlockGenerator()
{
  mTextures.resize(6);
}

PMesh<MeshPartialBlockGenerator::VertexType> MeshPartialBlockGenerator::Create(Side side, const std::array<char, 8> &neib)
{
  PMesh<VertexType> mesh = std::make_shared<TemplateMesh<VertexType>>();

  enum 
  {
    VERTEX_COUNT = sizeof(vertexCube) / sizeof(vertexCube[0]),
    SIDE_COUNT = VERTEX_COUNT / 4,
  };
  
  auto realsides = 0;
  for (size_t i = 0, sideCount = 0; i < SIDE_COUNT; ++i)
  {
	  if (side & (1 << i))
	  {
		  realsides++;
	  }
  }

  for (size_t i = 0, sideCount = 0; i < SIDE_COUNT; ++i)
  {
    if (side & (1 << i))
    {
      glm::vec2 test[] =
      {
        { mTextures[i].x, mTextures[i].y },
        { mTextures[i].x, mTextures[i].w },
        { mTextures[i].z, mTextures[i].w },
        { mTextures[i].z, mTextures[i].y },
      };

      for (size_t j = 0; j < 4; ++j)
      {
        mesh->PushVertex({
          { 
            vertexCube[i * 4 + j][0],
            vertexCube[i * 4 + j][1],
            vertexCube[i * 4 + j][2]
          },
          {
            test[j][0],
            test[j][1]
          },
          {
            normalCube[i][0],
            normalCube[i][1],
            normalCube[i][2]
          },
		  {
			  1.f / float(neib[indexCube[i * 4 + j]])
		  }
        });
      }

      for (size_t j = 0; j < (sizeof(indexCubeSide) / sizeof(indexCubeSide[0])); ++j)
      {
        mesh->PushIndex(indexCubeSide[j] + sideCount);
      }
      sideCount += 4;
    }
  }

  return mesh;
}

void MeshPartialBlockGenerator::SetTexture(int side, const std::string &texture)
{
  for (unsigned int i = 0; i < 6; ++i)
  {
    if (side & (1 << i))
    {
		mTextures[i] = std::get<1>(TextureManager::Get().GetTexture(texture));
    }
  }
}

void MeshPartialBlockGenerator::JsonLoad(const rapidjson::Value & val)
{
  if (val.HasMember("all"))
  {
    SetTexture(MeshPartialBlockGenerator::ALL, val["all"].GetString());
  }
  if (val.HasMember("separate"))
  {
    const rapidjson::Value &arr = val["separate"];
    SetTexture(MeshPartialBlockGenerator::FRONT,  arr.Begin()->GetString());
    SetTexture(MeshPartialBlockGenerator::RIGHT,  arr[1].GetString());
    SetTexture(MeshPartialBlockGenerator::BACK,   arr[2].GetString());
    SetTexture(MeshPartialBlockGenerator::LEFT,   arr[3].GetString());
    SetTexture(MeshPartialBlockGenerator::TOP,    arr[4].GetString());
    SetTexture(MeshPartialBlockGenerator::BOTTOM, arr[5].GetString());
  }
}
