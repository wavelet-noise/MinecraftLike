// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef SectorSplicer_h__
#define SectorSplicer_h__


#include "..\tools\QueuedThread.h"


class Tessellator : public QueuedThread
{
public:
  Tessellator();
  ~Tessellator();
};



#endif // SectorSplicer_h__