// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "IRenderBlockStrategy.h"



RenderBlockStrategyFactory::FactoryType & RenderBlockStrategyFactory::Get()
{
  static FactoryType object;
  return object;
}
