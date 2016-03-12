// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef GameObject_h__
#define GameObject_h__

#include <map>
#include <memory>
#include <type_traits>
#include "GameObjectParams.h"
#include "Agent.h"
#include "..\tools\StringIntern.h"

using PGameObject = std::shared_ptr<GameObject>;

template<class T, class... Args>
inline std::shared_ptr<T> MakeGameObject(Args&&... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}

class GameObject
{
public:
  GameObject(const StringIntern &__id);
  virtual ~GameObject();

  virtual void Update(GameObjectParams &params);

  /// client
  /// рисует gui этого агента для переданного в параметрах блока. Должен вызываться каждый кадр, когда требуется отрисовка окна
  virtual void DrawGui(InteractParams &params);

  ///client/server syncronize
  virtual void Interact(InteractParams &params);

  ///client/server paralell
  ///выполняется 1 раз для каждого агента каждого игрового объекта, хранящегося в базе данных, после полной загрузки последней
  virtual void Afterload();

  virtual PGameObject Clone();

  Agent *GetFromFullName(const StringIntern &name);

  const Agent *GetFromFullName(const StringIntern &name) const;

  template<class T>
  T *GetFromFullName(const StringIntern &name)
  {
    return static_cast<T*>(GetFromFullName(name));
  }

  template<class T>
  const T *GetFromFullName(const StringIntern &name) const
  {
    return static_cast<const T*>(GetFromFullName(name));
  }

  template<class T>
  T *GetFromFullName(const std::string &name)
  {
    return static_cast<T*>(GetFromFullName(StringIntern(name)));
  }

  template<class T>
  const T *GetFromFullName(const std::string &name) const
  {
    return static_cast<T*>(GetFromFullName(StringIntern(name)));
  }

  void PushAgent(PAgent go);

  StringIntern GetId();

protected:
  friend class DB;
  std::map<StringIntern, PAgent> mAgents;

  friend class TemplateItemMaterial;
  StringIntern id;
};



#endif // GameObject_h__
