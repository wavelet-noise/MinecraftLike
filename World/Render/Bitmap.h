// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#ifndef BITMAP_H
#define BITMAP_H

#define GLM_SWIZZLE
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include "Color.h"
#include <algorithm>

/// Битовое изображение в оперативной памяти.
class Bitmap
{
public:
  /// Создает пустую битмапу.
  Bitmap();

  /// Загружает битмапу из png файла.
  Bitmap(const std::string &fileName);

  /// Создать битмапу заданного размера и цвета.
  Bitmap(const glm::uvec2 &size, const Color &color = Color::black);

  /// Создать битмапу заданного размера и массива данных.
  Bitmap(const glm::uvec2 &size, const unsigned char *data);

  /// Разрушает битмапу.
  ~Bitmap();

  void Set(const glm::uvec2 &pos, Color color);

  Color Get(const glm::uvec2 &pos) const;

  /// Вернуть размер битмапы.
  const glm::uvec2 &GetSize() const;

  /// Вернуть данные битмапы.
  const std::vector<unsigned char> &GetRaw() const;

  /// Сохранить в файл.
  void Save(const std::string &fileName) const;

  /// Вставить битмапу.
  void Insert(const glm::uvec2 &pos, const Bitmap &bitmap);

  /// Зеркалирование.
  /// direction Направление. true - по горизонтали. false - по вертикали.
  void Mirror(bool direction = true);


  Bitmap &operator *=(const Bitmap &other)
  {
    if (glm::length(glm::vec2(other.GetSize())) == 0)
      return *this;

    std::transform(mData.begin(), mData.end(),
      other.mData.begin(),
      mData.begin(),
      [](unsigned char a, unsigned char b) -> unsigned char {
      return static_cast<unsigned char>(
          ((a / static_cast<float>(std::numeric_limits<unsigned char>::max())) *
          (b / static_cast<float>(std::numeric_limits<unsigned char>::max()))) *
          std::numeric_limits<unsigned char>::max()
        );
    });

    return *this;
  }

private:

  std::vector<unsigned char> mData;

  // Размер битмапы.
  glm::uvec2 mSize;

};

#endif // BITMAP_H
