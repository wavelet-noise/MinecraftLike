// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef IRenderStrategy_h__
#define IRenderStrategy_h__

#include <boost\noncopyable.hpp>
#include "GameObjectParams.h"
#include "..\Render\Model.h"
#include <memory>
#include <rapidjson\document.h>
#include "TemplateFactory.h"
#include "..\tools\StringIntern.h"

using PRenderStrategy = std::shared_ptr<class IRenderBlockStrategy>;

template<class T, class... Args>
inline std::shared_ptr<T> MakeRenderStrategy(Args&&... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}

/// —тратеги€ формирует и возвращает модель блока.
/// ѕредоставл€ет информацию о типе возвращаемой модели,
/// статическа€ или динамическа€. ” статической модели 
/// не измен€етс€ меш или матрица модели.
class IRenderBlockStrategy
{
public:
  IRenderBlockStrategy() = default;
  virtual ~IRenderBlockStrategy() {};

  virtual void Load(const rapidjson::Value &val) = 0;

  virtual const Model &GetModel(const GameObjectParams &params) = 0;

  virtual bool IsStatic() const = 0;
};

#define REGISTER_RENDER_STRATEGY(type) REGISTER_ELEMENT(type, RenderBlockStrategyFactory::Get(), StringIntern(#type))

struct RenderBlockStrategyFactory : public boost::noncopyable
{
  using FactoryType = TemplateFactory<StringIntern, IRenderBlockStrategy>;
  static FactoryType &Get();
};

#endif // IRenderStrategy_h__