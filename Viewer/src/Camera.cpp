#include "Camera.h"
#include "Utils.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, const float aspectRatio) :
	zoom(1.0f),
	fovy(glm::pi<float>() / 4.0f),
	height(5),
	zNear(0.1f),
	zFar(200.0f),
	aspectRatio(aspectRatio),
	prespective(true),
	viewTransformation(1),
	eye(eye),
	at(at),
	up(up),
	position(glm::vec3(0.0f, 0.0f, 0.0f))

{
	UpdateProjectionMatrix();

	viewTransformation = glm::lookAt(eye, at, up);
}

Camera::~Camera()
{
}


void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float zNear,
	const float zFar)
{
	prespective = false;
	float width = aspectRatio * height;
	projectionTransformation = glm::ortho(-width / 2, width / 2, -height / 2, height / 2, zNear, zFar);
}

void Camera::SetPerspectiveProjection(const float fovy, const float aspect, const float zNear, const float zFar)
{
	prespective = true;
	this->fovy = fovy;
	this->aspectRatio = aspect;
	this->zNear = zNear;
	this->zFar = zFar;
	projectionTransformation = glm::perspective(fovy,aspect,zNear, zFar);
}


const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projectionTransformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return viewTransformation;
}

void Camera::Zoom(const float factor)
{
	fovy = fovy * factor;
	if (fovy > glm::pi<float>())
	{
		fovy = glm::pi<float>();
	}

	UpdateProjectionMatrix();
}


void Camera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	UpdateProjectionMatrix();
}

void Camera::UpdateProjectionMatrix()
{
	if (prespective)
	{
		SetPerspectiveProjection(fovy, aspectRatio, zNear, zFar);
	}
	else
	{
		SetOrthographicProjection(height, aspectRatio, zNear, zFar);
	}
}

void Camera::SwitchToPrespective()
{
	prespective = true;
	UpdateProjectionMatrix();
}

void Camera::SwitchToOrthographic()
{
	prespective = false;
	UpdateProjectionMatrix();
}

void Camera::SetNear(const float zNear)
{
	this->zNear = zNear;
	UpdateProjectionMatrix();
}

void Camera::SetFar(const float zFar)
{
	this->zFar = zFar;
	UpdateProjectionMatrix();
}

void Camera::SetHeight(const float height)
{
	this->height = height;
	UpdateProjectionMatrix();
}

void Camera::SetFovy(const float fovy)
{
	this->fovy = fovy;
	UpdateProjectionMatrix();
}

float Camera::GetNear()
{
	return zNear;
}

float Camera::GetFar()
{
	return zFar;
}

float Camera::GetFovy()
{
	return fovy;
}

float Camera::GetHeight()
{
	return height;
}

bool Camera::IsPrespective()
{
	return prespective;
}

const glm::vec3& Camera::GetEye() const
{
	return eye;
}