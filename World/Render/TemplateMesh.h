



#pragma once
#ifndef TemplateMesh_h__
#define TemplateMesh_h__

#include <memory>
#include <vector>
#include "RenderMeshDList.h"
#include "RenderMeshVao.h"
#include <Serealize.h>
#include <tiny_obj_loader.h>
#include <tools\Log.h>

template<class V>
class TemplateMesh;

template<class V>
using PMesh = std::shared_ptr<TemplateMesh<V>>;

template<class V>
using PCMesh = std::shared_ptr<const TemplateMesh<V>>;

template<class V>
class TemplateMesh
{
public:
	using VertexType = V;

	inline TemplateMesh()
	{
		mStrategy = std::make_unique<RenderMeshVao>();
		//mStrategy = std::make_unique<RenderMeshDList>();
	}

	inline VertexType &Vertex(size_t i)
	{
		return mVertex[i];
	}

	inline size_t &Index(size_t i)
	{
		return mIndex[i];
	}

	inline void Reserve(size_t vertexCount, size_t indexCount)
	{
		mVertex.reserve(vertexCount);
		mIndex.reserve(indexCount);
	}

	inline void Clear()
	{
		mVertex.clear();
		mIndex.clear();
	}

	inline void Release()
	{
		mVertex.swap(decltype(mVertex)());
		mIndex.swap(decltype(mIndex)());
	}

	inline size_t SizeVertex() const
	{
		return mVertex.size();
	}

	inline size_t SizeIndex() const
	{
		return mIndex.size();
	}

	inline void PushVertex(const VertexType &vertex)
	{
		mVertex.push_back(vertex);
	}

	inline void PushIndex(size_t index)
	{
		mIndex.push_back(index);
	}

	inline bool Empty() const noexcept
	{
		return mVertex.empty() && mIndex.empty();
	}

	void Push(const TemplateMesh<VertexType> &mesh)
	{
		mIndex.reserve(mesh.mIndex.size());
		size_t size = mVertex.size();
		for (const auto &i : mesh.mIndex)
		{
			mIndex.push_back(size + i);
		}

		mVertex.reserve(mVertex.size() + mesh.SizeVertex());
		mVertex.insert(mVertex.end(), mesh.mVertex.begin(), mesh.mVertex.end());
	}

	/// Создать сетку в видеопамяти.
	/// @param vertex указатель на начало буфера вершин.
	/// @param vertexSize количество элементов в буфере вершин.
	/// @param index указатель на начало буфера индексов.
	/// @param indexSize количество элементов в буфере индексов.
	inline void Compile(const std::vector<int> &locations)
	{
		mStrategy->SetAttribute(GetAttribute(), locations);
		mStrategy->Send(reinterpret_cast<float *>(mVertex.data()), mVertex.size(),
			reinterpret_cast<size_t *>(mIndex.data()), mIndex.size());
	}

	/// Нарисовать сетку.
	inline void Draw()
	{
		mStrategy->Draw();
	}

	IRenderMeshStrategy &GetStrategy()
	{
		return *mStrategy;
	}

	inline const std::vector<Attribute> &GetAttribute() const noexcept
	{
		return VertexType::Get();
	}

	void JsonLoad(const rapidjson::Value &val)
	{
		JSONLOAD(sge::make_nvp("vertex", mVertex), sge::make_nvp("index", mIndex));
	}

	void Load(const std::string &s, glm::vec3 VertexType::* pos, glm::vec3 VertexType::* norm, glm::vec2 VertexType::* uv)
	{
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> mats;
		std::string err;

		bool ok = tinyobj::LoadObj(shapes, mats, err, s.c_str());
		if (!ok)
		{
			LOG(error) << err;
		}

		mVertex.reserve(shapes[0].mesh.positions.size() / 3);
		for (size_t i = 0; i < shapes[0].mesh.positions.size() / 3; ++i)
		{
			VertexType vt;
			vt.*pos = { shapes[0].mesh.positions[i * 3], shapes[0].mesh.positions[i * 3 + 1], shapes[0].mesh.positions[i * 3 + 2] };
			vt.*norm = { shapes[0].mesh.normals[i * 3], shapes[0].mesh.normals[i * 3 + 1], shapes[0].mesh.normals[i * 3 + 2] };
			vt.*uv = { shapes[0].mesh.texcoords[i * 2], shapes[0].mesh.texcoords[i * 2 + 1] };

			mVertex.push_back(vt);
		}
		mIndex = shapes[0].mesh.indices;
	}

	void BuildAABB(glm::vec3 VertexType::* p)
	{
		glm::vec3 _min{ 0 }, _max{ 0 };
		if (!mVertex.empty())
		{
			for (decltype(mVertex.size()) i = 0; i < mVertex.size(); ++i)
			{
				const auto &t = mVertex[i].*p;
				for (int j = 0; j < 3; ++j)
				{
					_max[j] = glm::max(t[j], _max[j]);
					_min[j] = glm::min(t[j], _min[j]);
				}
			}
			empty_aabb = false;
		}
		else
		{
			empty_aabb = true;
		}
		min = glm::vec4(_min, 1);
		max = glm::vec4(_max, 1);
	}

	bool IsAabbDot()
	{
		return empty_aabb;
	}

	const std::tuple<const glm::vec4 &, const glm::vec4 &> GetAABB() const
	{
		return std::tie(min, max);
	}

private:
	glm::vec4 min, max;
	bool empty_aabb = true;

	std::vector<VertexType> mVertex;
	std::vector<size_t> mIndex;

	std::unique_ptr<IRenderMeshStrategy> mStrategy;
};



#endif // TemplateMesh_h__


