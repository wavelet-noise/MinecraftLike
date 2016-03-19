



#ifndef TEXTURE_H
#define TEXTURE_H
#include <memory>
#include <glm/glm.hpp>
#include "Bitmap.h"

/// Описание текстурных слотов.
enum TextureSlot
{
  TEXTURE_SLOT_0 = 0,
  TEXTURE_SLOT_1 = 1,
  TEXTURE_SLOT_2 = 2,
  TEXTURE_SLOT_3 = 3,
  TEXTURE_SLOT_4 = 4,
  TEXTURE_SLOT_5 = 5,
  TEXTURE_SLOT_6 = 6,
  TEXTURE_SLOT_7 = 7,

  TEXTURE_SLOT_COUNT,
};

enum TextureDim
{
  TEXTURE_DIM_1,
  TEXTURE_DIM_2,
  TEXTURE_DIM_3,

  TEXTURE_DIM_COUNT,
};

class Texture;
using PTexture = std::shared_ptr<Texture>;
using PCTexture = std::shared_ptr<const Texture>;

/// Текстура. Находится в видеопамяти.
class Texture
{
public:
  /// Создать текстуру на основе битмапы.
  Texture(const Bitmap &bitmap, bool mip = false, bool smoothing = false, TextureDim dim = TEXTURE_DIM_2, const glm::vec3 &size = {});

  /// id  only texture
  Texture();

  /// Разрушить текстуру.
  ~Texture();

  void DepthTexture(const glm::vec2 &size);

  /// Вернуть размер текстуры.
  inline const glm::uvec2 &GetSize() const
  {
    return mSize;
  }

  inline unsigned int GetId()
  {
    return mTextureId;
  }

  void GenMipmap();

  /// Установить текстуру на заданный текстурный слот.
  void Set(TextureSlot slot) const;

private:
  // id текстуры. 0 - не существующая текстура.
  unsigned int mTextureId;
  bool mMip = false;
  int mdim;

  // Размер текстуры в текстелях.
  glm::uvec2 mSize;

private:

  // Вспомогательная функция. Узнает, является ли число степенью 2.
  inline bool IsPow2(unsigned int a) const
  {
    return !(a & (a - 1));
  }

};


#endif // TEXTURE_H
