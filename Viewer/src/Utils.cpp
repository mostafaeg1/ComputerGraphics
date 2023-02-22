#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Utils.h"
#include <iostream>
#include <iomanip>      // std::setw
#include <algorithm>    // std::min
using namespace std;


//glm::vec4 mul(glm::mat4 mat, glm::vec4 v) {
//	glm::vec4 res = glm::vec4();
//	for (int i = 0; i < 4; i++) {
//		float sum = 0;
//		for (int j = 0; j < 4; j++) {
//			sum += mat[i][j] * v[j];
//		}
//		res[i] = sum;
//   }
//	return res;
//}

//void ApplyTransformationToVertices(glm::mat4 transform, vector<glm::vec3>& vertices) {
//	for (int i = 0; i < vertices.size();i++) {
//		glm::vec4 transformed = mul(transform, glm::vec4(vertices[i], 1.0f));
//		vertices[i]= transformed;
//	}
//}	


glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::ifstream ifile(filePath.c_str());

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			vertices.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// TODO: Handle texture coordinates
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}

	/* Printing mesh to console :

	cout << "   " << "------- Vertices -------" << "(" << vertices.size() << ") \n\n";
	cout << setw(7) << 'x' << setw(10) << 'y' << setw(10) << 'z' << "\n\n";

	for each(glm::vec3 v in vertices) {	
		cout<< setw(7) << v.x << setw(10) << v.y<< setw(10) << v.z<<"\n";
	}

	cout << "\n   " << "------- Faces -------" << "(" << faces.size() << ") \n\n";
	for each (Face f in faces) {
		cout << setw(7) << "v1: " << f.GetVertexIndex(0) << ", vn1: " << f.GetNormalIndex(0) << " ,vt1: " << f.GetTextureIndex(0) << "\t";
		cout << setw(7) << "v2: " << f.GetVertexIndex(1) << ", vn2: " << f.GetNormalIndex(1) << " ,vt2: " << f.GetTextureIndex(1) << "\t";
		cout << setw(7) << "v3: " << f.GetVertexIndex(2) << ", vn3: " << f.GetNormalIndex(2) << " ,vt3: " << f.GetTextureIndex(2) << "\n";
	}
	*/


	


	return std::make_shared<MeshModel>(faces, vertices, normals, textureCoords, Utils::GetFileName(filePath));
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}


