



#pragma once
#ifndef RenderList_h__
#define RenderList_h__

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "RenderModel.h"


class Render
{
public:
  Render();
  ~Render();

  static void Initialize();


  size_t AddModel(const std::string &mesh, const std::string &texture, const std::string &shader);

  void EraseModel(size_t id);

  void SetModelMatrix(size_t id, const glm::mat4 &matrix);

  void Draw(class Camera &camera);

private:
  glm::ivec2 mVersion;

  std::vector<std::unique_ptr<RenderModel>> mModels;

  // TODO: Сделать список пустых id.
  size_t mGenId = 0;

};

#endif // RenderList_h__
