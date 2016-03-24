#include "ParticleSystem.h"
#include <gl/glew.h>
#include <Render\Resourses.h>

void ParticleSystem::Render(const Camera & cam) const
{
}

void ParticleSystem::Add(const glm::vec3 & pos, float liftime, float size, const Color & color)
{
	if (mCount >= maxSize - 1)
		return;

	vertex[mCount].pos = glm::vec4(pos, size);
	vertex[mCount].col  = color.Tou8Color();
	vext[mCount].lifetime = liftime;

	mCount++;
}

ParticleSystem::~ParticleSystem()
{
	if (mCreated)
	{
		glDeleteBuffers(2, mVbo);
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

	glGenVertexArrays(1, &mVao);
	glGenBuffers(2, mVbo);
	glBindVertexArray(mVao);

	int offset = 0;
	glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Particle), nullptr, GL_STREAM_DRAW);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offset); offset += sizeof(Particle::pos);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offset); offset += sizeof(Particle::tex);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Particle), (void*)offset); offset += sizeof(Particle::col);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);


	glBindBuffer(GL_ARRAY_BUFFER, mVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	shader = Resourses::Get().GetShader("shaders/particles.glsl");

	mCreated = true;
}

void ParticleSystem::Draw(const Camera & cam) const
{
	if (mCreated)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, maxSize * sizeof(Particle), &vertex[0], GL_STREAM_DRAW);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, mCount * sizeof(Particle), &vertex[0]);

		shader->Use();
		shader->SetUniform(cam.GetView(), "modelview");
		shader->SetUniform(cam.GetProject(), "projection");

		glBindVertexArray(mVao);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mCount);
	}
}
