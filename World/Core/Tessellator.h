// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef SectorSplicer_h__
#define SectorSplicer_h__


#include "..\tools\QueuedThread.h"
#include "..\tools\CoordSystem.h"
#include "BlockTessellator.h"
#include "SectorTessellator.h"
#include <unordered_map>


class Tessellator : public QueuedThread<Tessellator>
{
public:
  Tessellator(class Render &render);

  /// ѕотокобезопасные методы.

  /// ”становить тессел€тор в указанную позицию.
  void Set(const WBPos &pos, PBlockTessellator block);

  /// —ообщить сектору, что он изменилс€.
  void SayChanged(const SPos &pos);

public:
  /// ћетоды не предназначенные дл€ использовани€ вне потока тессел€тора.

  void Start();

  void Process();

  void Stop();

  PBlockTessellator GetBlock(const WBPos &pos);
  
private:
  std::unordered_map<SPos, std::shared_ptr<SectorTessellator>> mSectors;

  class Render &mRender;

private:
  /// Ќайти сектор по позиции сектора.
  std::shared_ptr<SectorTessellator> FindSector(const SPos &pos);

};



#endif // SectorSplicer_h__