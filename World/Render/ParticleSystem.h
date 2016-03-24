#pragma once
#include <GL\glew.h>
#include <Render\Camera.h>
#include <Render\Render.h>

struct Particle
{
	glm::vec4 pos;
	glm::vec2 tex;
	glm::u8vec4 col;
};

struct ParticleExtender
{
	float lifetime;
};

class ParticleSystem
{
public:
	void Render(const Camera &cam) const;
	void Add(const glm::vec3 &pos, float lifetime = 1.f, float size = 1.f, const Color &color = Color::white);
	~ParticleSystem();
	ParticleSystem();

	static ParticleSystem & Get()
	{
		static ParticleSystem ps;
		return ps;
	}

	void Draw(const Camera &cam) const;

private:
	GLuint mVao, mVbo[2], mVbi;
	bool mCreated = false;

	const int maxSize = 1000;
	std::vector<Particle> vertex;
	std::vector<ParticleExtender> vext;
	int mCount = 0;

	PShader shader;
};