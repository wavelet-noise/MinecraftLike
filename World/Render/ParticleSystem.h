#pragma once
#include <GL\glew.h>
#include <Render\Camera.h>
#include <Render\Render.h>

struct Particle
{
	glm::vec4 pos;
	glm::u8vec4 col;
};

struct ParticleExtender
{
	float lifetime;
};

class ParticleSystem
{
public:
	void Add(const glm::vec3 &pos, StringIntern tex, float lifetime = 1.f, float size = 1.f, const Color &color = Color::white);
	~ParticleSystem();
	ParticleSystem();

	static ParticleSystem & Get()
	{
		static ParticleSystem ps;
		return ps;
	}

	void Draw(const Camera &cam) const;
	void Update(float dt);

private:
	GLuint mVao, mVbo[3], mVbi;
	bool mCreated = false;

	const int maxSize = 1000;
	std::vector<Particle> vertex;
	std::vector<glm::vec2> uvs;
	std::vector<ParticleExtender> vext;
	int mCount = 0;

	PShader shader;
};