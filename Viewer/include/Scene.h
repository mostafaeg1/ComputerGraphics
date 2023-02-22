#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"
#include "Light.h"

using namespace std;

class Scene {
public:
	Scene();

	void AddModel(const shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	
	void AddCamera(const Camera& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index);
	Camera& GetActiveCamera();

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;


	void AddLight();          //defualt
	void AddLight(glm::vec3 position);    
	int GetLightCount() const;
	Light& GetLight(size_t index) const;
	Light& GetActiveLight() const;
	void SetActiveLightIndex(int index);
	int  GetActiveLightIndex() const;
	bool HasLights() const { return !lights.empty(); }
	void RemoveLight(size_t index);
	void updateTexturePath(string path);
	string getTexturePath();
	
private:
	vector<shared_ptr<MeshModel>> mesh_models;
	vector<Camera> cameras;
	vector<shared_ptr<Light>> lights;

	int active_camera_index;
	int active_model_index;
	int next_light_index;
	int active_light_index;
	string texture_path;
};