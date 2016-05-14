#include "ParticleSystem.h"
#include <gl/glew.h>
#include <Render\Resources.h>
#include <Render\TextureManager.h>

void ParticleSystem::Add(const glm::vec3 & pos, StringIntern tex, float liftime, float size, const Color & color)
{
	if (mCount >= maxSize - 1)
		return;

	vertex[mCount].pos = glm::vec4(pos, size);
	vertex[mCount].col = color.Tou8Color();
	auto &atl = TextureManager::Get().GetTexture(tex);
	auto &uv4 = std::get<1>(atl);
	uvs[mCount * 4 + 0] = glm::vec2(uv4.x, uv4.w);
	uvs[mCount * 4 + 1] = glm::vec2(uv4.x, uv4.y);
	uvs[mCount * 4 + 2] = glm::vec2(uv4.z, uv4.w);
	uvs[mCount * 4 + 3] = glm::vec2(uv4.z, uv4.y);

	vext[mCount].lifetime = liftime;

	mCount++;
}

ParticleSystem::~ParticleSystem()
{
	if (mCreated)
	{
		glDeleteBuffers(3, mVbo);
		glDeleteVertexArrays(1, &mVao);
	}
}

static const GLfloat quad_data[] = {
	0.0, 1.0,
	0.0, 0.0,
	1.0, 1.0,
	1.0, 0.0
};

ParticleSystem::ParticleSystem()
{
	vertex.resize(maxSize);
	vext.resize(maxSize);
	uvs.resize(maxSize * 4);

	glGenVertexArrays(1, &mVao);
	glGenBuffers(3, mVbo);
	glBindVertexArray(mVao);

	glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Particle), nullptr, GL_STREAM_DRAW);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, pos));
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Particle), (void*)offsetof(Particle, col));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);


	glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, mVbo[2]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), nullptr, GL_STREAM_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
	glEnableVertexAttribArray(2);

	shader = Resources::Get().GetShader("shaders/particles.glsl");

	mCreated = true;
}

void ParticleSystem::Draw(const Camera & cam) const
{
	if (mCreated)
	{

		glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
		//glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Particle), nullptr, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mCount * sizeof(Particle), &vertex[0]);

		glBindBuffer(GL_ARRAY_BUFFER, mVbo[2]);
		//glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), nullptr, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mCount * 4 * sizeof(glm::vec2), &uvs[0]);

		shader->Use();
		shader->SetUniform(cam.GetView(), "modelview");
		shader->SetUniform(cam.GetProject(), "projection");

		glBindVertexArray(mVao);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mCount);
	}
}

void ParticleSystem::Update(float dt)
{
	for (int i = 0; i < mCount; i++)
	{
		vext[i].lifetime -= dt;
		if (vext[i].lifetime <= 0)
		{
			vext[i]        = vext[mCount - 1];
			vertex[i]      = vertex[mCount - 1];
			uvs[i * 4 + 0] = uvs[(mCount - 1) * 4 + 0];
			uvs[i * 4 + 1] = uvs[(mCount - 1) * 4 + 1];
			uvs[i * 4 + 2] = uvs[(mCount - 1) * 4 + 2];
			uvs[i * 4 + 3] = uvs[(mCount - 1) * 4 + 3];

			mCount--;
			i--;
		}
	}
}
