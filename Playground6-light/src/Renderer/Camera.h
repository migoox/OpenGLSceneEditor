#pragma once
#include "Transfrom.h"

class Camera : public Transform
{
public:
	Camera() :
		m_FOVY(45.f),
		m_Ratio(1.f),
		m_ZNear(0.3f),
		m_ZFar(1000.0f)
	{
		// reverting z axis is required to get right-handed orientation
		m_ProjectionMatrix = glm::perspective(m_FOVY, m_Ratio, m_ZNear, m_ZFar) * glm::scale(glm::vec3(1.f, 1.f, -1.f));
	}

	Camera(float fovy, float widthByHeightRatio, float zNear, float zFar) :
		m_FOVY(fovy),
		m_Ratio(widthByHeightRatio),
		m_ZNear(zNear),
		m_ZFar(zFar)
	{
		// reverting z axis is required to get right-handed orientation
		m_ProjectionMatrix = glm::perspective(m_FOVY, m_Ratio, m_ZNear, m_ZFar) * glm::scale(glm::vec3(1.f, 1.f, -1.f));
	}

	~Camera() { }

	// returns view matrix, which is one of the MVP matrices:  projection * view * model * vertex
	glm::mat4 GetViewMatrix() const;

	// returns projection matrix, which is one of the MVP matrices:  projection * view * model * vertex
	glm::mat4 GetProjectionMatrix() const;

	// returns camera matrix, which is equal to outcome of multiplication: projectionMatrix * viewMatrix
	glm::mat4 GetCameraMatrix() const;

	// sets fov of frustum in y axis 
	void SetFOVVertical(float fovy);

	// sets window width by height ratio
	void SetWidthByHeightRatio(float ratio);
	
	// sets nearest possible distance from camera (objects closer than zNear won't be seen)
	void SetZNear(float zNear);

	// sets farest possible distance from camera (objects further than zFar will be clipped)
	void SetZFar(float zFar);

private:
	glm::mat4 m_ProjectionMatrix;

	float m_FOVY;
	float m_Ratio;
	float m_ZNear;
	float m_ZFar;
};