



#include "TextureManager.h"

#include <iostream>
#include <boost\filesystem.hpp>
#include "ErrorImage.h"
#include <tools\Log.h>

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

void TextureManager::LoadDirectory(const std::string &mDir)
{
  boost::filesystem::path targetDir(mDir);
  boost::filesystem::recursive_directory_iterator iter(targetDir);

  int loaded = 0;
  for (const boost::filesystem::path &file : iter) 
  {
    if (boost::filesystem::is_regular_file(file) && boost::filesystem::extension(file) == ".png")
    {
      LoadTexture(file.stem().string());
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

void TextureManager::LoadTextureMultiplied(const std::string & mask, const std::string & mat)
{
  for (unsigned int i = 0; i < mMultiAtlas.size(); ++i)
  {
    if (LoadToAtlasMultiplied(i, mask, mat))
    {
      // Кажется мы смогли загрузить все текстуры в один атлас.
      return;
    }
  }
  // Мы пытались, но не смоги...
  // Попробуем создать еще один атлас.
  mMultiAtlas.resize(mMultiAtlas.size() + 1);
  if (!LoadToAtlasMultiplied(mMultiAtlas.size() - 1, mask, mat))
  {
    // Ошибка.
    std::cout << "TextureManager. Load texture error." << std::endl;
  }
}

void TextureManager::LoadTexturesMultipliedBackground(const std::string & mask, const std::string & mat, const std::string & back)
{
  for (unsigned int i = 0; i < mMultiAtlas.size(); ++i)
  {
    if (LoadToAtlasMultipliedBackground(i, mask, mat, back))
    {
      // Кажется мы смогли загрузить все текстуры в один атлас.
      return;
    }
  }
  // Мы пытались, но не смоги...
  // Попробуем создать еще один атлас.
  mMultiAtlas.resize(mMultiAtlas.size() + 1);
  if (!LoadToAtlasMultipliedBackground(mMultiAtlas.size() - 1, mask, mat, back))
  {
    // Ошибка.
    std::cout << "TextureManager. Load texture error." << std::endl;
  }
}

std::tuple<PTexture, glm::vec4> TextureManager::GetTexture(const std::string &name) const
{
  auto itTexture = mTextures.find(name);
  if (itTexture == mTextures.end())
  {
    // Текстура не найдена.
    const auto &texture = mTextures.find(error_image.name)->second;
    return std::tuple<PTexture, glm::vec4>(mMultiAtlas[texture.index].texture, texture.pos);
  }

  return std::tuple<PTexture, glm::vec4>(mMultiAtlas[(*itTexture).second.index].texture, (*itTexture).second.pos);
}

void TextureManager::Compile()
{
  LOG(trace) << "texture atlases recompiling";
  for (unsigned int i = 0; i < mMultiAtlas.size(); ++i)
  {
    mMultiAtlas[i].texture = std::make_shared<Texture>(mMultiAtlas[i].atlas.GetAtlas(), false);
    mMultiAtlas[i].texture->GenMipmap();
//    mMultiAtlas[i].atlas.GetAtlas().Save("Atlas_" + std::to_string(i) + ".png");
  }
  LOG(trace) << "done";
}

bool TextureManager::LoadToAtlas(size_t atlas, const std::initializer_list<std::string> &names)
{
  // TODO: Удаление из атласа, если не смогли вставить.
  for (auto &name : names)
  {
    Bitmap bitmap;
    try
    {
      bitmap = Bitmap(std::string("data\\textures\\") + name + ".png");
    }
    catch (char *msg)
    {
      std::cout << msg << std::endl;
      continue;
    }
    auto uv = mMultiAtlas[atlas].atlas.Add(name, bitmap);

    mTextures[name] = { atlas, uv };
  }

  return true;
}

bool TextureManager::LoadToAtlasMultipliedBackground(size_t atlas, const std::string &mask, const std::string &mat,const std::string &back)
{
  // TODO: Удаление из атласа, если не смогли вставить.
  Bitmap bitmap_mask, bitmap_mat, bitmap_back;
  try
  {
    bitmap_mask = Bitmap(std::string("data\\textures\\") + mask + ".png");
    bitmap_mat = Bitmap(std::string("data\\textures\\") + mat + ".png");
    bitmap_back = Bitmap(std::string("data\\textures\\") + back + ".png");
  }
  catch (const std::exception &e)
  {
    LOG(error) << e.what();
  }

  bitmap_mask *= bitmap_mat;
  bitmap_back |= bitmap_mask;

  auto pos = mMultiAtlas[atlas].atlas.Add(mask, bitmap_back);

  mTextures[mask + "_" + mat] = { atlas, pos };

  return true;
}

bool TextureManager::LoadToAtlasMultiplied(size_t atlas, const std::string &mask, const std::string &mat)
{
  // TODO: Удаление из атласа, если не смогли вставить.
  Bitmap bitmap_mask, bitmap_mat;
  try
  {
    bitmap_mask = Bitmap(std::string("data\\textures\\") + mask + ".png");
    bitmap_mat = Bitmap(std::string("data\\textures\\") + mat + ".png");
  }
  catch (const std::exception &e)
  {
    LOG(error) << e.what();
  }

  bitmap_mask *= bitmap_mat;
  
  auto pos = mMultiAtlas[atlas].atlas.Add(mask, bitmap_mask);

  mTextures[mask+"_"+mat] = { atlas, pos };

  return true;
}