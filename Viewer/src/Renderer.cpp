#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"
#include "InitShader.h"
#include <iostream>
#include "MathUtils.h"
#include "Scene.h"
#include <random>


#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))



Renderer::Renderer()
{
	texture_path = "../Data/Textures/Stone.jpg";
}

Renderer::~Renderer()
{

}


void Renderer::Render(const std::shared_ptr<Scene>& scene)
{
	auto t_path = scene->getTexturePath();
	if (t_path != texture_path) {
		texture_path = t_path;
		LoadTexture();
	}
	int cameraCount = scene->GetCameraCount();
	if (cameraCount > 0)
	{
		int modelCount = scene->GetModelCount();
		auto camera = scene->GetActiveCamera();

		for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			auto currentModel = scene->GetModel(currentModelIndex);
			auto apply_toon_shading_status = currentModel.applyToonShadingStatus();
			auto apply_texture_mapping_status = currentModel.applyTextureMappingStatus();
			// Activate the 'colorShader' program (vertex and fragment shaders)
			colorShader.use();

			// Set the uniform variables
			colorShader.setUniform("model", currentModel.getTranformation());
			colorShader.setUniform("view", camera.GetViewTransformation());
			colorShader.setUniform("projection", camera.GetProjectionTransformation());
			colorShader.setUniform("camera_position", glm::vec4(scene->GetActiveCamera().GetPosition(), 1.0f));

			colorShader.setUniform("material.ambient", glm::vec4(currentModel.getAmbient(), 1.0f));
			colorShader.setUniform("material.diffuse", glm::vec4(currentModel.getDiffuse(), 1.0f));
			colorShader.setUniform("material.specular", glm::vec4(currentModel.getSpecular(), 1.0f));
			colorShader.setUniform("material.shininess", currentModel.getShininess());

			colorShader.setUniform("projection_type", 0);
			colorShader.setUniform("projection_scale", 1.0f);
			colorShader.setUniform("projection_offset", 0.0f);
			int light_count = scene->GetLightCount();

			colorShader.setUniform("apply_toon_shading_status", apply_toon_shading_status);
			colorShader.setUniform("apply_texture_mapping_status", apply_texture_mapping_status);

			colorShader.setUniform("lights.size", light_count);
			for (int i = 0; i < light_count; i++)
			{
				auto& light = scene->GetLight(i);

				std::ostringstream light_stream;
				light_stream << "lights.list[" << i << "].";

				std::ostringstream specular;
				specular << light_stream.str() << "specular";

				std::ostringstream diffuse;
				diffuse << light_stream.str() << "diffuse";

				std::ostringstream ambient;
				ambient << light_stream.str() << "ambient";

				std::ostringstream pos;
				pos << light_stream.str() << "position";

				colorShader.setUniform(ambient.str().data(), glm::vec4(light.ambient, 1.0f));
				colorShader.setUniform(diffuse.str().data(), glm::vec4(light.diffuse, 1.0f));
				colorShader.setUniform(specular.str().data(), glm::vec4(light.specular, 1.0f));
				colorShader.setUniform(pos.str().data(), glm::vec4(light.GetPosition(), 1.0f));
			}

			texture1.bind(0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(currentModel.GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelVertices().size());
			glBindVertexArray(0);

			texture1.unbind(0);

		}
	}
}


int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}


void Renderer::LoadShaders()
{
	colorShader.loadShaders("vshader.glsl", "fshader.glsl");
}

void Renderer::LoadTexture()
{
	texture1.loadTexture(texture_path, true);
}