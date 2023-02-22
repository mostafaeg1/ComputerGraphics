#include "MeshModel.h"
#include "MathUtils.h"
#include <iostream>
#include  <math.h>
#include "Renderer.h"
using namespace std;

float gitMaxDistance(vector<glm::vec3> vertices);
float getScaleValue();
bool MeshModel::drawAxis = false;
bool MeshModel::drawBox = false;
bool MeshModel::drawVertexNormal = false;
bool MeshModel::drawFaceNormal = false;
bool MeshModel::drawRectangle = false;



MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords,const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	Shininess(1.0f)
{

	ambientColor = glm::vec3(((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX)));
	diffuseColor = glm::vec3(((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX)));
	specularColor = glm::vec3(((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX)), ((double)rand() / (RAND_MAX)));

	this->position = glm::vec3(0, 0, 0);
	this->x = glm::vec3(100, 0, 0);
	this->x = glm::vec3(0, 100, 0);
	this->x = glm::vec3(-100, 100, 0);
	this->model_color = glm::vec3(0.0f, 0.0f, 0.0f); //black
	this->boundingBoxColor = glm::vec3(0.0f, 255.0f, 0.0f); //red
	this->model_name = model_name.substr(0, model_name.length() - 4);
	this->localScale = glm::mat4x4(1);
	this->localTranslate = glm::mat4x4(1);
	this->worldScale = glm::mat4x4(1);
	this->worldTranslate = glm::mat4x4(1);
	this->x_localRotate = glm::mat4x4(1);
	this->x_worldRotate = glm::mat4x4(1);
	this->y_localRotate = glm::mat4x4(1);
	this->y_worldRotate = glm::mat4x4(1);
	this->z_localRotate = glm::mat4x4(1);
	this->z_worldRotate = glm::mat4x4(1);

	this->fillFace_state = false;
	this->drawFaceFrame_state = true;
	this->colors = vector <glm::vec3>(this->GetFacesCount());


	modelVertices.reserve(3 * faces.size());	
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int v_index = currentFace.GetVertexIndex(j) - 1;
			int n_index = currentFace.GetNormalIndex(j) - 1;

			Vertex vertex;
			vertex.position = vertices[v_index];
			vertex.normal = normals[n_index];

			if (textureCoords.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = textureCoords[textureCoordsIndex];
			}

			modelVertices.push_back(vertex);
		}
	}



	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
	
}


void MeshModel::updateAmbient(glm::vec3 newColor) {
	ambientColor = newColor;
}
void MeshModel::updateDiffuse(glm::vec3 newColor) {
	diffuseColor = newColor;
}
void MeshModel::updateSpecular(glm::vec3 newColor) {
	specularColor = newColor;
}

void MeshModel::updateShininess(float newShininess) {
	Shininess = newShininess;
}

float MeshModel::getShininess() {
	return Shininess;
}

bool MeshModel::applyToonShadingStatus() {
	return apply_toon_shading;
}
void MeshModel::changeApplyToonShadingStatus(bool state) {
	apply_toon_shading =state;
}

bool MeshModel::applyTextureMappingStatus() {
	return texture_mapping;
}
void MeshModel::changeApplyTextureMappingStatus(bool state) {
	texture_mapping = state;
}


glm::vec3 MeshModel::getAmbient() {
	return ambientColor;
}

glm::vec3 MeshModel::getDiffuse() {
	return diffuseColor;
}

glm::vec3 MeshModel::getSpecular() {
	return specularColor;
}


bool  MeshModel::getFillFaceState() {
	return this->fillFace_state;
}

bool  MeshModel::getGrayState() {
	return this->gray_state;
}

void  MeshModel::changeFillFaceState() {
	fillFace_state = true;
	gray_state=false;
}

void  MeshModel::changeGrayState() {
	fillFace_state = false;
	gray_state = true;
}
void MeshModel::setEmpty() {
	fillFace_state = false;
	gray_state = false;
}

bool MeshModel::getDrawFaceFrameState() {
	return this->drawFaceFrame_state;
}
void MeshModel::changeDrawFaceFrameState() {
	this->drawFaceFrame_state=!drawFaceFrame_state;
}

vector<glm::vec3> MeshModel::getFacesColors() {
	return colors;
}

void MeshModel::updateVertexNormalScale(float newScale) {
	this->VertexNoramlScale = newScale;
}

void MeshModel::updateFaceNormalScale(float newScale) {
	this->FaceNoramlScale = newScale;
}

void MeshModel::updateFaceNormalColor(glm::vec3 newColor) {
	this->FaceNormalColor = newColor;
}

void MeshModel::updateVertexNormalColor(glm::vec3 newColor) {
	this->VertexNormalColor = newColor;
}

glm::vec3 MeshModel::getVertexNormalColor() {
	return this->VertexNormalColor;
}
glm::vec3 MeshModel::getFaceNormalColor() {
	return this->FaceNormalColor;
}
float MeshModel::getVertexNormalScale() {
	return this->VertexNoramlScale;
}
float MeshModel::getFaceNormalScale() {
	return this->FaceNoramlScale;
}

glm::vec3 MeshModel::getBoundingBoxColor() {
	return this->boundingBoxColor;
}


void  MeshModel::updateBoundingBoxColor(glm::vec3 newColor) {
	 this->boundingBoxColor=newColor;
}




void MeshModel::changeDrawFaceNormalstate() {
	drawFaceNormal = !drawFaceNormal;
}

bool MeshModel::getDrawFaceNormalstate() {
	return drawFaceNormal;
}

void MeshModel::changeDrawVertexNormalstate() {
	drawVertexNormal = !drawVertexNormal;
}

bool MeshModel::getDrawVertexNormalstate() {
	return drawVertexNormal;
}

bool MeshModel::getDrawRectangleState() {
	return drawRectangle;
}


void MeshModel::changeDrawBoxstate() {
	drawBox = !drawBox;
}

void MeshModel::changeDrawRectanglestate() {
	drawRectangle = !drawRectangle;
}

bool MeshModel::getDrawBoxstate() {
	return drawBox;
}

void MeshModel::changeDrawAxisstate(){
	drawAxis = !drawAxis;
}

bool MeshModel::getDrawAxisstate() {
	return drawAxis;
}

void MeshModel::setColor(glm::vec3 color) {
	model_color = color;
}
glm::vec3 MeshModel::getColor() {
	return model_color;
}

std::vector<glm::vec3>  MeshModel::getVertices() {
	return vertices;
}

void MeshModel::setLocalTranslate(glm::mat4 local) {
	localTranslate = local;
}
void MeshModel::setWorldTranslate(glm::mat4 word) {
	worldTranslate = word;
}

void MeshModel::setWorldScale(glm::mat4 world) {
	worldScale = world ;
}
void MeshModel::setLocalScale(glm::mat4 local) {
	localScale = local;
}

void MeshModel::setLocal_x_Rotation(glm::mat4 local) {
	x_localRotate = local;
}
void MeshModel::setWorld_x_Rotation(glm::mat4 world) {
	x_worldRotate = world;
}
void MeshModel::setLocal_y_Rotation(glm::mat4 local) {
	y_localRotate = local;
}
void MeshModel::setWorld_y_Rotation(glm::mat4 world) {
	y_worldRotate = world;
}
void MeshModel::setLocal_z_Rotation(glm::mat4 local) {
	z_localRotate = local;
}
void MeshModel::setWorld_z_Rotation(glm::mat4 world) {
	z_worldRotate = world;
}
std::vector<glm::vec3> MeshModel::getNormals() {
	return normals;
}

glm::mat4 MeshModel::getLocalTransfromation() {
	return    z_localRotate* y_localRotate * x_localRotate * localTranslate * localScale;
}

glm::mat4 MeshModel::getWorldTransfromation() {
	return    worldTranslate * 
	worldScale * x_worldRotate * y_worldRotate * z_worldRotate;
}

glm::mat4 MeshModel::getTranformation() {
	return getWorldTransfromation() * getLocalTransfromation();
}

glm::mat4 MeshModel::getTranformationWithoutScale() {
	return  x_localRotate * y_localRotate * z_localRotate * localTranslate;
}


MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}

float getDistance(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

glm::vec4 M4mulV4(glm::mat4 mat, glm::vec4 v) {
	glm::vec4 res = glm::vec4();
	for (int i = 0; i < 4; i++) {
		float sum = 0;
		for (int j = 0; j < 4; j++) {
			sum += mat[i][j] * v[j];
		}
		res[i] = sum;
	}
	return res;
}



void MeshModel::ApplyTransformationToVertices(glm::mat4 transform) {
	for (int i = 0; i < vertices.size(); i++) {
		glm::vec4 transformed = M4mulV4(transform, glm::vec4(vertices[i], 1.0f));
		vertices[i] = transformed / transformed.w;
	}
}

std::vector<glm::vec3> MeshModel::getNewVerticesAfterTransformation() {
	std::vector<glm::vec3> newvertices = std::vector<glm::vec3>(vertices.size());
	for (int i = 0; i < vertices.size(); i++) {
		glm::vec4 transformed = M4mulV4(getTranformation(), glm::vec4(vertices[i], 1.0f));
		newvertices[i] = transformed / transformed.w;;
	}
	return newvertices;
}



void MeshModel::FitToScreen(){
	float scale_value = getScaleValue() / gitMaxDistance(this->vertices);
	glm::mat4 scaleMat= MathUtils::makescaleMat(scale_value,scale_value,scale_value);
	ApplyTransformationToVertices(scaleMat);
}


float gitMaxDistance(vector<glm::vec3> vertices) {
	float min_x = 0.0;
	float min_y = 0.0;
	float max_x = 0.0;
	float max_y = 0.0;
	for each (glm::vec3 v in vertices) {
		min_x = min(min_x,v.x);
		min_y = min(min_x, v.x);
		max_x = max(max_y, v.y);
		max_y = max(max_y, v.y);
	}
	return getDistance(min_x, min_y, max_x, max_y);
}

float getScaleValue() {
	return 250;
}

std::pair<glm::vec3, glm::vec3> MeshModel::GetMinAndMaxPoint()
{
	constexpr float min_value = std::numeric_limits<float>::min(), max_value = std::numeric_limits<float>::max();
	glm::vec3 min_point(max_value, max_value, max_value), max_point(min_value, min_value, min_value);

	for each (glm::vec3 vertex in vertices)
	{
		min_point.x = std::min(min_point.x, vertex.x);
		min_point.y = std::min(min_point.y, vertex.y);
		min_point.z = std::min(min_point.z, vertex.z);
		max_point.x = std::max(max_point.x, vertex.x);
		max_point.y = std::max(max_point.y, vertex.y);
		max_point.z = std::max(max_point.z, vertex.z);
	}

	return std::make_pair(min_point, max_point);
}

vector<pair<glm::vec3, glm::vec3>> MeshModel::GetBoundingBox()
{
	pair<glm::vec3, glm::vec3> extremums = GetMinAndMaxPoint();
	glm::vec3 min_point = extremums.first, max_point = extremums.second;
	vector<std::pair<glm::vec3, glm::vec3>> bounding_box;

	bounding_box.push_back(std::make_pair(glm::vec3(min_point.x, min_point.y, min_point.z), glm::vec3(max_point.x, min_point.y, min_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(min_point.x, min_point.y, min_point.z), glm::vec3(min_point.x, max_point.y, min_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(min_point.x, min_point.y, min_point.z), glm::vec3(min_point.x, min_point.y, max_point.z)));

	bounding_box.push_back(std::make_pair(glm::vec3(max_point.x, max_point.y, min_point.z), glm::vec3(max_point.x, min_point.y, min_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(max_point.x, max_point.y, min_point.z), glm::vec3(min_point.x, max_point.y, min_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(max_point.x, max_point.y, min_point.z), glm::vec3(max_point.x, max_point.y, max_point.z)));

	bounding_box.push_back(std::make_pair(glm::vec3(max_point.x, min_point.y, max_point.z), glm::vec3(max_point.x, min_point.y, min_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(max_point.x, min_point.y, max_point.z), glm::vec3(max_point.x, max_point.y, max_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(max_point.x, min_point.y, max_point.z), glm::vec3(min_point.x, min_point.y, max_point.z)));

	bounding_box.push_back(std::make_pair(glm::vec3(min_point.x, max_point.y, max_point.z), glm::vec3(min_point.x, max_point.y, min_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(min_point.x, max_point.y, max_point.z), glm::vec3(min_point.x, min_point.y, max_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(min_point.x, max_point.y, max_point.z), glm::vec3(max_point.x, max_point.y, max_point.z)));

	return bounding_box;
}

const std::vector<Vertex>& MeshModel::GetModelVertices()
{
	return modelVertices;
}

GLuint MeshModel::GetVAO() const
{
	return vao;
}

