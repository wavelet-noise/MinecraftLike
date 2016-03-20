#pragma once
#ifndef RenderAgent_h__
#define RenderAgent_h__


#include <memory>
#include <type_traits>
#include <boost/core/noncopyable.hpp>
#include "TemplateFactory.h"
#include "../tools/StringIntern.h"
#include "../rapidjson/document.h"
#include "../Render/Model.h"
#include "TessellatorParams.h"


typedef std::shared_ptr<class BlockTessellator> PGameObjectTessellator;

template<class T, class... Args>
inline std::shared_ptr<T> MakeBlockTessellator(Args&&... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}


// Тесселятор блоков.
// Формирует модель блока.
class BlockTessellator
{
public:
  virtual ~BlockTessellator() = default;

  virtual PGameObjectTessellator Clone() = 0;

  virtual void JsonLoad(const rapidjson::Value &val);

  virtual const Model &GetModel(const TessellatorParams &params) = 0;

  // Прозрачен ли блок?
  inline bool IsTransparent() const noexcept
  {
    return mTransparent;
  }

  // Статичен ли блок?
  // Данный параметр указывает тесселятору, можно ли склеивать блок.
  inline bool IsStatic() const noexcept
  {
    return mStatic;
  }

private:
  bool mStatic = true;
  bool mTransparent = false;
};


#define REGISTER_BLOCK_TESSELLATOR(type) REGISTER_ELEMENT(type, BlockTessellatorFactory::Get(), StringIntern(#type))

struct BlockTessellatorFactory : public boost::noncopyable
{
  using FactoryType = TemplateFactory<StringIntern, BlockTessellator>;
  static FactoryType &Get();
};

#endif // RenderAgent_h__
