// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef RenderAgent_h__
#define RenderAgent_h__


#include "Agent.h"
#include "IRenderBlockStrategy.h"

typedef std::shared_ptr<class BlockTessellator> PBlockTessellator;

template<class T, class... Args>
inline std::shared_ptr<T> MakeBlockTessellator(Args&&... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}


class BlockTessellator
{
public:
  virtual ~BlockTessellator() = default;

  virtual PBlockTessellator Clone() = 0;

  virtual void Load(const rapidjson::Value &val);

  inline bool IsTransparent() const noexcept
  {
    return mTransparent;
  }

  inline const StringIntern &GetName() const noexcept
  {
    return mName;
  }

private:
  bool mTransparent = false;
  PRenderStrategy mRenderStrategy;
  StringIntern mName;
};


#endif // RenderAgent_h__
