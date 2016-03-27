



#pragma once
#ifndef Model_h__
#define Model_h__

#include "Texture.h"
#include "TemplateMesh.h"
#include "Vertex.h"
#include <rapidjson\document.h>
#include <tools\StringIntern.h>

using PModel = std::shared_ptr<class Model>;
using PCModel = std::shared_ptr<const class Model>;

// Модель. Имеет всю информацию для рисования.
class Model
{
public:
	Model();

	void JsonLoad(const rapidjson::Value & val);

	enum Type
	{
		Static,
	};

	inline void SetTexture(PTexture texture)
	{
		mTexture = texture;
	}

	inline PMesh<VertexVTN> &GetMesh() noexcept
	{
		return mMeshes;
	}

	inline const PMesh<VertexVTN> &GetMesh() const noexcept
	{
		return mMeshes;
	}

	inline const PTexture &GetTexture() const noexcept
	{
		return mTexture;
	}

	inline Type GetType() const noexcept
	{
		return mType;
	}

	inline PShader &GetShader()
	{
		return mShader;
	}

	inline StringIntern GetSpriteName()
	{
		return mSprite;
	}

	void SetSprite(const StringIntern &s);

	const glm::mat4 &GetModel() const
	{
		return mModel;
	}

	void SetModel(const glm::mat4 &m)
	{
		mModel = m;
	}

	void BuildAABB(glm::vec3 VertexVTN::* p = nullptr);

	inline const std::tuple<const glm::vec4&, const glm::vec4&> Model::GetAABB() const
	{
		return mMeshes->GetAABB();
	}

	bool IsAabbDot();

	void Compile()
	{
		mMeshes->Compile(mShader->GetAttributeLocation(mMeshes->GetAttribute()));
	}

	void Release()
	{
		mMeshes->Release();
	}

private:

	friend class Render;
	PMesh<VertexVTN> mMeshes;
	PTexture mTexture;
	PShader mShader;
	glm::mat4 mModel;

	StringIntern mSprite = StringIntern(""); //используется для восстановлении связи с атласом

	Type mType = Static;
};

#endif // Model_h__

