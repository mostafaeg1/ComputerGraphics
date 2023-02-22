#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include <iostream>

Scene::Scene() :
	active_camera_index(0),
	active_model_index(0),
	texture_path("../Data/Textures/crate.jpg")
{
	next_light_index = 0;
}

void Scene::updateTexturePath(string newPath) {
	texture_path = newPath;
}
string Scene::getTexturePath() {
	return texture_path;
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
		mesh_models.push_back(mesh_model);
}

int Scene::GetModelCount() const
{
	return mesh_models.size();
}

MeshModel& Scene::GetModel(int index) const
{
	return *mesh_models[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models[active_model_index];
}

void Scene::AddCamera(const Camera& camera)
{
	cameras.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras.size();
}

Camera& Scene::GetCamera(int index) 
{
	return cameras[index];
}

Camera& Scene::GetActiveCamera()
{
	return cameras[0];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index;
}


void Scene::AddLight()                 //default light
{
	lights.push_back(std::make_shared<Light>(next_light_index++, glm::vec3(0.0f, 0.0f, 0.0f)));
}


int Scene::GetLightCount() const
{
	return lights.size();
}


void Scene::AddLight(glm::vec3 position)      //adding light at given position
{
	lights.push_back(std::make_shared<Light>(next_light_index++, position));
}

Light& Scene::GetLight(size_t index) const
{
	return *lights[index];
}

Light& Scene::GetActiveLight() const
{
	return *lights[active_light_index];
}


void Scene::SetActiveLightIndex(int index)
{
	active_light_index = index;
}


int Scene::GetActiveLightIndex() const
{
	return active_light_index;
}

