



#pragma once
#ifndef SectorTessellator_h__
#define SectorTessellator_h__


#include "Config.h"
#include "BlockTessellator.h"
#include <array>
#include "..\tools\CoordSystem.h"
#include "..\Render\Model.h"
#include <glm/glm.hpp>
#include "SectorBase.h"


class SectorTessellator : public SectorBase<PGameObjectTessellator>
{
public:
	SectorTessellator(const SPos &pos);

	// Сообщить сектору, что он изменен.
	void SayChanged();

	// Если сектор был изменен, обновляет геометрию и посылает в рендер.
	bool Update(class Tessellator *tesselator, class RenderSector &render);

	// требует загруженых шейдеров в Resources
	static void Init();

	inline bool IsChanged() const noexcept
	{
		return mChanged;
	}

	inline SPos GetPos() const noexcept
	{
		return mPos;
	}

private:
	SPos mPos;
	bool mChanged = false;
	void __PushMmodel(const Model &model, const SBPos &pos);
};



#endif // SectorTessellator_h__