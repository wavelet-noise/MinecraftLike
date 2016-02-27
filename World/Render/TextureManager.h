// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef TextureManager_h__
#define TextureManager_h__

#include "Texture.h"
#include <string>
#include <unordered_map>
#include "Atlas.h"


/// Менеджер текстур.
/// Хранит все игровые текстуры.
class TextureManager
{
public:
  static TextureManager &Get();

  /// Загрузить указанную текстуру.
  void LoadTexture(const std::string &name);

  void LoadDirectory(const std::string & s);

  /// Загрузить набор текстур.
  /// Все текстуры будут добавлены в один текстурный атлас.
  void LoadTexture(const std::initializer_list<std::string> &names);

  /// Загрузка тесктуры @mat умноженной на маску @mask. Имя является суммой строк @mat и @mask
  void LoadTextureMultiplied(const std::string & mask, const std::string & mat);

  /// Получить текстуру по имени.
  /// Если текстуры не существует, возвращается нулевой указатель.
  std::tuple<PTexture, glm::uvec4> GetTexture(const std::string &name) const;

  /// Создает текстуры в графической памяти.
  void Compile();

private:

  struct AtlasChunk
  {
    size_t index; // Номер атласа.
    glm::uvec4 pos;     // Положение в атласе.
  };

  struct AtlasTexture
  {
    AtlasTexture()
      : atlas({ 2048, 2048 })
    {};
    Atlas atlas;
    PTexture texture;
  };

  std::vector<AtlasTexture> mMultiAtlas;

  std::unordered_map<std::string, AtlasChunk> mTextures;

private:
  TextureManager();
  ~TextureManager() {};
  TextureManager(const TextureManager &) = delete;
  TextureManager& operator=(const TextureManager &) = delete;

  bool LoadToAtlas(size_t atlas, const std::initializer_list<std::string> &names);
  bool LoadToAtlasMultiplied(size_t atlas, const std::string & mask, const std::string & mat);

};



#endif // TextureManager_h__
