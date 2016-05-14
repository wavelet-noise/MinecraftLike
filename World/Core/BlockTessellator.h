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

	enum Side
	{
		EMPTY = 0,

		FRONT = 1 << 0,
		RIGHT = 1 << 1,
		BACK = 1 << 2,
		LEFT = 1 << 3,
		TOP = 1 << 4,
		BOTTOM = 1 << 5,
		CENTER = 1 << 6,

		ALL = FRONT + RIGHT + BACK + LEFT + TOP + BOTTOM + CENTER
	};

	virtual ~BlockTessellator() = default;

	virtual PGameObjectTessellator Clone() = 0;

	virtual void JsonLoad(const rapidjson::Value &val);

	virtual PModel GetModel(const TessellatorParams &params, int slise = 9999) = 0;

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
