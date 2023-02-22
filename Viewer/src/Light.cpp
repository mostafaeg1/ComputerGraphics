#include "Light.h"
#include <iostream>
bool Light::show_light = false;
bool Light::show_reflected_light = false;


Light::Light(int id, glm::vec3 position) :
	name("light" + std::to_string(id)),
	position(position),
	ambient(glm::vec3(1.0f, 1.0f, 1.0f)),
	diffuse(glm::vec3(1.0f, 1.0f, 1.0f)),
	specular(glm::vec3(0.1f, 0.1f, 0.1f))
{
	alpha = 1;
	phong = false;
}	


void Light::updateAlpha(float newAlpha) {
	alpha = newAlpha;
}
float Light::getAlpha() {
	return alpha;
}

void Light::changeLightTypeToShade() {
	phong = false;
}
void Light::changeLightTypeToPhong() {
	phong = true;
}
bool Light::getLightType() {
	return phong;
}

void Light::updatePosition(glm::vec3 new_pos) {
	position = new_pos;
}
void Light::changeShowLight() {
	show_light  =! show_light;
}
void Light::changeShowReflectedLight() {
	show_reflected_light = !show_reflected_light;

}
bool Light::getShowLight() {
	return show_light;
}
bool Light::getShowReflectedLight() {
	return show_reflected_light;
}