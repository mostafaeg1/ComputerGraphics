#define _USE_MATH_DEFINES
#include "MathUtils.h"
#include <cmath>
//#pragma once
//#include <iostream>
//#include <glm/glm.hpp>

glm::mat4 MathUtils::makeTranslateMat(float x, float y, float z) {
	return  glm::mat4(
		1.0, 0.0, 0.0, 0,
		0.0, 1.0, 0.0, 0,
		0.0, 0.0, 1.0, 0,
		x, y, z, 1.0);

}

glm::mat4 MathUtils::makescaleMat(float x, float y, float z) {
	return  glm::mat4(
		x, 0.0, 0.0, 0,
		0.0, y, 0.0, 0,
		0.0, 0.0, z, 0,
		0.0, 0.0, 0.0, 1.0);

}

void MathUtils::printMat4(glm::mat4 mat) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << mat[i][j] << ", ";
		}
		cout << "\n";
	}
}

glm::mat4 MathUtils::makeRotationMat(char str, float deg)
{
	float sin_degree = sin(deg * 3.14159 / 180);
	float cos_degree = cos(deg * 3.14159 / 180);
	switch (str)
	{
	case  'y':
		return glm::mat4(
			cos_degree, 0, -sin_degree, 0,
			0, 1, 0, 0,
			sin_degree, 0, cos_degree, 0,
			0, 0, 0, 1);
	case 'z':
		return glm::mat4(
			cos_degree, -sin_degree, 0, 0,
			sin_degree, cos_degree, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	case 'x':
		return glm::mat4(
			1, 0, 0, 0,
			0, cos_degree, sin_degree, 0,
			0, -sin_degree, cos_degree, 0,
			0, 0, 0, 1);
	default:
		return glm::mat4(1.0);
	}
}

glm::mat4 MathUtils::getDefualtMat() {
	return glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}



glm::vec4 MathUtils::M4mulV4(glm::mat4 mat, glm::vec4 v) {
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

float MathUtils::getDistance(glm::vec3 p1, glm::vec3 p2) {
	auto x1 = p1.x;
	auto y1 = p1.y;
	auto x2 = p2.x;
	auto y2 = p2.y;
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}