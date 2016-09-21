/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <string>
#include <memory>

#include "shader.h"
#include "Texture.h"
#include <unordered_map>
#include <render\Camera.h>

#define BUFFER_SIZE 5000
struct Vtpc
{
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec4 col;
};

class SpriteBatch
{
public:
    SpriteBatch();
    ~SpriteBatch();

    std::vector<Vtpc> vertices;
    std::vector<GLushort> index;
    int cur = 0;


	void AddCube(const glm::vec3 & min, const glm::vec3 & max, const glm::vec2 & uv_min, const glm::vec2 & uv_max, PTexture tex, const glm::vec4 & col = glm::vec4(1, 1, 1, 1));

	void SetCam(Camera * camera);

	void Render();

	PShader curent_shader;
	PTexture curent_texture;
	Camera * current_camera = nullptr;

	GLuint m_vbo[2];
    GLuint m_vao = 0;
};
#endif // SPRITEBATCH_H
