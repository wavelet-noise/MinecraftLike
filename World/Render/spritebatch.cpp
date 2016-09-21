/*******************************************************************************
		Copyright (C) 2015 Samsonov Andrey

		This software is distributed freely under the terms of the MIT LICENSE.
		See "LICENSE.txt"
*******************************************************************************/
#include "spritebatch.h"
#include <tools\Log.h>
#include <GL/glew.h>
#include <Render\Resources.h>

typedef std::codecvt<wchar_t, char, mbstate_t> cvt;

//#include <unistd.h>

SpriteBatch::SpriteBatch()
{
	vertices.resize(BUFFER_SIZE * 4);
	index.resize(BUFFER_SIZE * 6);

	curent_shader = Resources::Get().GetShader("shaders/color.glsl");

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(2, m_vbo);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vtpc)*BUFFER_SIZE * 4, nullptr, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vtpc), (void*)(offsetof(Vtpc, pos)));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vtpc), (void*)(offsetof(Vtpc, uv)));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vtpc), (void*)(offsetof(Vtpc, col)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*BUFFER_SIZE * 6, nullptr, GL_STREAM_DRAW);

	LOG(info) << "spritebatch ready";
}

SpriteBatch::~SpriteBatch()
{
	glDeleteBuffers(2, m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

GLushort cube_elements[] = {
	// front
	0, 1, 2,
	2, 3, 0
};

void SpriteBatch::AddCube(const glm::vec3 & _min, const glm::vec3 & _max, const glm::vec2 & uv_min, const glm::vec2 & uv_max, PTexture tex, const glm::vec4 & col)
{
	auto max = _max;
	auto min = _min;

	if (cur >= BUFFER_SIZE - 4)
		Render();

	if (curent_texture != tex)
	{
		Render();
		curent_texture = tex;
	}

	Vtpc v0, v1, v2, v3;

	if (min.x > max.x)
		std::swap(min.x, max.x);

	if (min.y > max.y)
		std::swap(min.y, max.y);

	v0.pos = { min.x, min.y, min.z };
	v1.pos = { max.x, min.y, min.z };
	v2.pos = { max.x, max.y, min.z };
	v3.pos = { min.x, max.y, min.z };

	v0.uv = { uv_min.x, uv_min.y };
	v1.uv = { uv_max.x, uv_min.y };
	v2.uv = { uv_max.x, uv_max.y };
	v3.uv = { uv_min.x, uv_max.y };

	v0.col = col;
	v1.col = col;
	v2.col = col;
	v3.col = col;

	vertices[cur * 4 + 0] = v0;
	vertices[cur * 4 + 1] = v1;
	vertices[cur * 4 + 2] = v2;
	vertices[cur * 4 + 3] = v3;

	index[cur * 6]     = cur * 4;
	index[cur * 6 + 1] = cur * 4 + 1;
	index[cur * 6 + 2] = cur * 4 + 2;
	index[cur * 6 + 3] = cur * 4 + 2;
	index[cur * 6 + 4] = cur * 4 + 3;
	index[cur * 6 + 5] = cur * 4 + 0;

	cur++;
}

void SpriteBatch::SetCam(Camera * camera)
{
	current_camera = camera;
}

void SpriteBatch::Render()
{
	if (cur == 0 || !curent_texture)
		return;

	curent_shader->Use();
	curent_shader->SetUniform(current_camera->GetViewProject(), "transform_VP");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, curent_texture->GetId());

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vtpc)*cur * 4, &vertices[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLshort)*cur * 6, &index[0]);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, cur * 6, GL_UNSIGNED_SHORT, NULL);

	cur = 0;
}
