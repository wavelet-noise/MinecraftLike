



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
using PMeshType = PMesh<VertexVTN>;

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

	inline PMeshType &GetMesh() noexcept
	{
		return mMeshes;
	}

	inline const PMeshType &GetMesh() const noexcept
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

protected:

	friend class Render;
	PMeshType mMeshes;
	PTexture mTexture;
	PShader mShader;
	glm::mat4 mModel;

	Type mType = Static;
};

class DeltaModel : public Model
{
	struct GpuReference
	{
		int vbegin = 0;
		int ibegin = 0;
		char vsize = 0;
		char isize = 0;
	};

	std::vector<GpuReference> parts;

public:

	void Init(std::vector<PMeshType> &meshes);

	void UpdatePart(int i, PMeshType &mesh);
};

#endif // Model_h__

