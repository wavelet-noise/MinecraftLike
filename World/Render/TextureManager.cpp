// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "TextureManager.h"

#include <iostream>
#include <boost\filesystem.hpp>
#include "ErrorImage.h"

TextureManager &TextureManager::Get()
{
  static TextureManager obj;
  return obj;
}

TextureManager::TextureManager()
{
  mMultiAtlas.resize(1);
  Bitmap bitmap(glm::uvec2(error_image.width, error_image.height), 
    reinterpret_cast<const unsigned char *>(error_image.pixel_data));
  auto pos = mMultiAtlas[0].atlas.Add(error_image.name, bitmap);
  mTextures[error_image.name] = { 0, pos };
}

void TextureManager::LoadTexture(const std::string &name)
{
  LoadTexture(std::initializer_list<std::string>{name});
}

void TextureManager::LoadDirectory(const std::string &dir)
{
  boost::filesystem::path targetDir(dir);
  boost::filesystem::recursive_directory_iterator iter(targetDir);

  int loaded = 0;
  for (const boost::filesystem::path &file : iter) 
  {
    if (boost::filesystem::is_regular_file(file) && boost::filesystem::extension(file) == ".png")
    {
      LoadTexture(file.string());
    }
  }
}

void TextureManager::LoadTexture(const std::initializer_list<std::string> &names)
{
  for (unsigned int i = 0; i < mMultiAtlas.size(); ++i)
  {
    if (LoadToAtlas(i, names))
    {
      // Кажется мы смогли загрузить все текстуры в один атлас.
      return;
    }
  }
  // Мы пытались, но не смоги...
  // Попробуем создать еще один атлас.
  mMultiAtlas.resize(mMultiAtlas.size() + 1);
  if (!LoadToAtlas(mMultiAtlas.size() - 1, names))
  {
    // Ошибка.
    std::cout << "TextureManager. Load texture error." << std::endl;
  }
}

//TODO:лучше использовать сразу текстурные координаты, вместо отступа и размера в пикселях
std::tuple<PTexture, glm::uvec4> TextureManager::GetTexture(const std::string &name) const
{
  auto itTexture = mTextures.find(name);
  if (itTexture == mTextures.end())
  {
    // Текстура не найдена.
    const auto &texture = mTextures.find(error_image.name)->second;
    return std::tuple<PTexture, glm::uvec4>(mMultiAtlas[texture.index].texture, texture.pos);
  }

  return std::tuple<PTexture, glm::uvec4>(mMultiAtlas[(*itTexture).second.index].texture, (*itTexture).second.pos);
}

void TextureManager::Compile()
{
  for (unsigned int i = 0; i < mMultiAtlas.size(); ++i)
  {
    mMultiAtlas[i].texture = std::make_shared<Texture>(mMultiAtlas[i].atlas.GetAtlas(), false);
    mMultiAtlas[i].texture->GenMipmap();
//    mMultiAtlas[i].atlas.GetAtlas().Save("Atlas_" + std::to_string(i) + ".png");
  }
}

bool TextureManager::LoadToAtlas(size_t atlas, const std::initializer_list<std::string> &names)
{
  // TODO: Удаление из атласа, если не смогли вставить.
  for (auto &name : names)
  {
    Bitmap bitmap;
    try
    {
      bitmap = Bitmap(name);
    }
    catch (char *msg)
    {
      std::cout << msg << std::endl;
      continue;
    }
    auto pos = mMultiAtlas[atlas].atlas.Add(name, bitmap);

    boost::filesystem::path p(name);
    mTextures[p.stem().string()] = { atlas, pos };
  }

  return true;
}
