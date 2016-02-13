// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef RenderList_h__
#define RenderList_h__

#include <list>
#include <glm/glm.hpp>
#include "Model.h"
#include <mutex>
#include <memory>

class RenderIterator;

/// Список рисования (сцена):
/// Внутри сцены есть список моделей(предположительно вектор),
/// все они рисуются каждый кадр.
/// Все модели в этом списке скомпилированы и не могут быть изменены.
/// Добавление модели в сцену потокобезопасно.
/// При добавлении модели в сцену на нее возвращается итератор.
/// Итератор предоставляет потокобезопасный доступ к матрице модели.
/// 
/// При добавлении в сцену, модель помещается в список на добавление.
/// В начале новой итерации рендера, все модели из списка на добавление 
/// компилируются и перемещаются в список моделей.
/// 
/// Через итератор можно поменять модель.В этом случае новая модель добавится
/// в список на добавление, старая модель добавится в список на удаление.
class RenderList
{
public:
  RenderList();
  ~RenderList();

  /// Добавить модель на отрисовку. 
  RenderIterator PushModel(const Model &model, const glm::mat4 &matrix);

  void Draw();

private:
  void AddElements();

  void SwapMatrix();

private:
  friend RenderIterator;
  struct Element
  {
    using Iterator = std::list<Element>::iterator;

    Element(const Model &mod, const glm::mat4 &mat)
      : model(mod), matrixBack(mat)
    {}
    Model model;
    glm::mat4 matrix;
    glm::mat4 matrixBack;
    std::shared_ptr<Iterator> mIterator;
  };
  // Список элементов на отрисовку.
  std::list<Element> mDrawList;
  // Список элементов на добавление.
  std::list<Element> mAddList;
  // Список элементов на удаление.
  std::list<Element> mRemoveList;

  mutable std::mutex mMutex;
};

class RenderIterator
{
public:
  using ElementType = RenderList::Element;
  using IteratorType = std::weak_ptr<ElementType::Iterator>;

  inline RenderIterator(const IteratorType &iterator, std::mutex &mutex) noexcept
    : mIterator(iterator), mMutex(mutex)
  {}

  /// Получение модели. Мотокобезопасная операция.
  inline const Model &GetModel() const noexcept
  {
    std::lock_guard<std::mutex> lock(mMutex);
    return (*mIterator.lock())->model;
  }

  /// Установка матрицы модели. Потокобезопасная операция.
  inline void SetMatrix(const glm::mat4 &matrix)
  {
    std::lock_guard<std::mutex> lock(mMutex);
    (*mIterator.lock())->matrixBack = matrix;
  }

private:
  IteratorType mIterator;
  std::mutex &mMutex;
};


#endif // RenderList_h__
