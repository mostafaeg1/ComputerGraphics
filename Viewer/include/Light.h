#pragma once
#include <string>
#include <glm/glm.hpp>

class Light
{
public:
	Light(int id, glm::vec3 position);
	std::string GetName() { return name; }
	glm::vec3& GetPosition() { return position; }

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float alpha;
	static bool show_light;
	static bool show_reflected_light;
	bool phong;
	void changeShowLight();
	void changeShowReflectedLight();
	void changeLightTypeToShade();
	void changeLightTypeToPhong();
	bool getLightType();
	bool getShowLight();
	bool getShowReflectedLight();
	void updatePosition(glm::vec3 new_pos);
	void updateAlpha(float newAlpha);
	float getAlpha();

private:
	std::string name;
	glm::vec3 position;
};