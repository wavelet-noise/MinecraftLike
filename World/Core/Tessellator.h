#pragma once
#ifndef SectorSplicer_h__
#define SectorSplicer_h__


#include "..\tools\QueuedThread.h"
#include "..\tools\CoordSystem.h"
#include "BlockTessellator.h"
#include "SectorTessellator.h"
#include <unordered_map>
#include "..\tools\StringIntern.h"
#include <tuple>
#include <vector>
#include <Render\Camera.h>

class Tessellator : public QueuedThread<Tessellator>
{
public:
	Tessellator(class RenderSector &render);

	// ѕотокобезопасные методы.

	// ”становить тессел€тор в указанную позицию.
	void Set(const WBPos &pos, PGameObjectTessellator block);

	void Set(const SPos &spos, std::vector<std::tuple<size_t, StringIntern>> &&blocks);

	// —ообщить сектору, что он изменилс€.
	void SayChanged(const SPos &pos);
	void SayCamera(std::shared_ptr<Camera> c);
	void SetSlise(int s);
	int GetSlise();

public:
	// ћетоды не предназначенные дл€ использовани€ вне потока тессел€тора.

	void Start();

	void Process();

	void Stop();

	PGameObjectTessellator GetBlock(const WBPos &pos);
	void Interrupt();
private:
	std::unordered_map<SPos, std::shared_ptr<SectorTessellator>> mSectors;

	class RenderSector &mRender;
	std::shared_ptr<Camera> cam;
	int slise = 0;

private:
	// Ќайти сектор по позиции сектора.
	std::shared_ptr<SectorTessellator> FindSector(const SPos &pos);

};



#endif // SectorSplicer_h__