#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include "ShaderProgram.h"
#include "Texture2D.h"
#include <sstream>
#include "InitShader.h"



class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	Renderer();
	virtual ~Renderer();
	int GetViewportWidth() const;
	int GetViewportHeight() const;


	//Opengl
	void LoadShaders();
	void LoadTexture();
	void Render(const std::shared_ptr<Scene>& scene);



private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	float* color_buffer;
	float* z_buffer;
	int viewport_width;
	int viewport_height;

	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;

	float max_depth;
	float min_depth;

	//Opengl
	ShaderProgram lightShader;
	ShaderProgram colorShader;
	Texture2D texture1;
	string texture_path;

};
