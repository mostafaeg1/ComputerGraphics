#pragma once
#include <iostream>
#include <glm/glm.hpp>
using namespace std;



class MathUtils {
public:
	static glm::mat4 makeTranslateMat(float x, float y, float z);
	static glm::mat4 makescaleMat(float x, float y, float z);
	static void printMat4(glm::mat4 mat);
	static glm::mat4 makeRotationMat(char str, float degree);
	static glm::mat4 MathUtils::getDefualtMat();
	static glm::vec4 MathUtils::M4mulV4(glm::mat4 mat, glm::vec4 v);
	static float getDistance(glm::vec3 p1,glm::vec3 p2);
};
