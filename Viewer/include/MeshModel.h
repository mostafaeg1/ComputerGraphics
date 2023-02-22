#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Face.h"
#include <iostream>


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords,const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec3> getFacesColors();
	const std::string& GetModelName() const;
	void FitToScreen();
	void ApplyTransformationToVertices(glm::mat4 transform);
	void translate(float num);
	glm::mat4 getTranformation();
	glm::mat4 getLocalTransfromation();
	glm::mat4 getWorldTransfromation();
	void setLocalTranslate(glm::mat4 local);
	void setWorldTranslate(glm::mat4 world);
	void setWorldScale(glm::mat4 world);
	void setLocalScale(glm::mat4 local);
	void setLocal_x_Rotation(glm::mat4 local);
	void setWorld_x_Rotation(glm::mat4 world);
	void setLocal_y_Rotation(glm::mat4 local);
	void setWorld_y_Rotation(glm::mat4 world);
	void setLocal_z_Rotation(glm::mat4 local);
	void setColor(glm::vec3);
	glm::vec3 getColor();
	std::vector<glm::vec3> getNewVerticesAfterTransformation();
	void setWorld_z_Rotation(glm::mat4 world);
	static glm::mat4 makeTranslateMat(float x, float y, float z);
	glm::vec3 position;
	glm::vec3 x;
	glm::vec3 y;
	glm::vec3 z;
	static bool drawAxis;
	static void changeDrawAxisstate();
	static bool getDrawAxisstate();
	void getBoundingBoxVertices();
	static bool drawBox;
	static bool drawRectangle;
	static void changeDrawBoxstate();
	static void changeDrawRectanglestate();
	static bool getDrawRectangleState();
	static bool getDrawBoxstate();
	static bool drawVertexNormal;
	static void changeDrawVertexNormalstate();
	static bool getDrawVertexNormalstate();
	static bool drawFaceNormal;
	static void changeDrawFaceNormalstate();
	static bool getDrawFaceNormalstate();
	std::pair<glm::vec3, glm::vec3> GetMinAndMaxPoint();
	std::vector<std::pair<glm::vec3, glm::vec3>> GetBoundingBox();
	std::vector<glm::vec3> getNormals();
	float FaceNoramlScale;
	float VertexNoramlScale;
	void updateVertexNormalScale(float newScale);
	void updateFaceNormalScale(float newScale);
	void updateFaceNormalColor(glm::vec3 newColor);
	void updateBoundingBoxColor(glm::vec3 newColor);
	void updateVertexNormalColor(glm::vec3 newColor);
	glm::vec3 getVertexNormalColor();
	glm::vec3 getFaceNormalColor();
	glm::vec3 getBoundingBoxColor();
	float getVertexNormalScale();
	float getFaceNormalScale();
	glm::mat4 getTranformationWithoutScale();
	bool getFillFaceState();
	void changeFillFaceState();
	bool getDrawFaceFrameState();
	void changeDrawFaceFrameState();
	bool  getGrayState();
	void  changeGrayState();
	void setEmpty();
	void updateAmbient(glm::vec3 newColor);
	void updateDiffuse(glm::vec3 newColor);
	void updateSpecular(glm::vec3 newColor);
	void updateShininess(float newShininess);
	float getShininess();
	bool applyToonShadingStatus();
	void changeApplyToonShadingStatus(bool state);
	bool applyTextureMappingStatus();
	void changeApplyTextureMappingStatus(bool state);


	glm::vec3 getAmbient();
	glm::vec3 getDiffuse();
	glm::vec3 getSpecular();

	GLuint GetVAO() const;
	const std::vector<Vertex>& GetModelVertices();


protected:
	glm::mat4 localTranslate;
	glm::mat4 worldTranslate;
	glm::mat4 localScale;
	glm::mat4 worldScale;
	glm::mat4 x_localRotate;
	glm::mat4 x_worldRotate;
	glm::mat4 y_localRotate;
	glm::mat4 y_worldRotate;
	glm::mat4 z_localRotate;
	glm::mat4 z_worldRotate;	

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float Shininess;
	bool apply_toon_shading;
	bool texture_mapping;
	std::vector<Vertex> modelVertices;


	GLuint vbo;
	GLuint vao;

private:
	glm::vec3 model_color;
	bool fillFace_state;
	bool gray_state;
	bool drawFaceFrame_state;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colors;
	std::vector<Face> faces;
	std::vector<glm::vec3> normals;
	std::string model_name;
	glm::vec3 boundingBoxColor;
	glm::vec3 VertexNormalColor;
	glm::vec3 FaceNormalColor;
};
