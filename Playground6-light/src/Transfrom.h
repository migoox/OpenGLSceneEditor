#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

enum class Space {  Local, Global };

class Transform
{
public:
	Transform() :
		m_Origin(glm::vec3(0.f)),
		m_Scale(glm::vec3(1.f)),
		m_Rotation(glm::angleAxis(0.f, glm::vec3(1.0f, 0.f, 0.f))),
		m_Translation(glm::vec3(0.f)),
		m_Right(glm::vec3(1.f, 0.f, 0.f)),
		m_Up(glm::vec3(0.f, 1.f, 0.f)),
		m_Forward(glm::vec3(0.f, 0.f, 1.f))
	{ }

	~Transform() { }
	
	// Returns model matrix
		   glm::mat4 GetModelMatrix()		const;

	// Returns model matrix
		   glm::mat3 GetNormalMatrix()		const;

	// Returns local x axis 
	inline glm::vec3 GetRight()				const		  { return m_Right;  };

	// Returns local y axis 
	inline glm::vec3 GetUp()				const		  { return m_Up; }

	// Returns local z axis 
	inline glm::vec3 GetForward()			const		  { return m_Forward; }

	// Returns origin (anchor point, center of local axises) of model
	inline glm::vec3 GetOrigin()			const		  { return m_Origin; }

	// Returns scale of the model
	inline glm::vec3 GetScale()				const		  { return m_Scale; }

	// Returns rotation angles in radians
		   glm::vec3 GetEulers()			const;

	// Returns rotation angles in degrees
		   glm::vec3 GetEulersDegrees()		const;

	// Returns quaternion which defines rotation of model
	inline glm::quat GetQuaternion()		const		  { return m_Rotation; }

	// Returns global position of the model
	inline glm::vec3 GetTranslation()		const		  { return m_Translation; }
	
	// Sets origin (anchor point, center of local axises) of model
		   void SetOrigin(glm::vec3 origin)				  { m_Origin = origin; }

	// Sets scale of the model
		   void SetScale(glm::vec3 scale)				  { m_Scale = scale; }

	// Sets quaternion defining global rotation, basing on euler angles in radians
		   void SetRotation(glm::vec3 eulers_rad);

	// Sets global translation
		   void SetTranslation(glm::vec3 translation)	  { m_Translation = translation; }

	// Rotates model in order to force forward to point at target
		   void LookAt(glm::vec3 target, glm::vec3 up = glm::vec3(0.f, 1.f, 0.f));

	// Rotates model (local or global), each component of vec3 represents delta angle of rotation 
	// around axis x, y, z (no eulers rotation axises, quaternions are used)
		   void Rotate(glm::vec3 delta_radians, Space relativeTo = Space::Local);

	// Rotates around axis anchored in specific point
		   void RotateAround(float euler_rad, glm::vec3 point, glm::vec3 axis);

	// Moves model by given vector (local or global)
		   void Translate(glm::vec3 translation, Space relativeTo = Space::Global);
private:
	glm::vec3 m_Right;
	glm::vec3 m_Up;
	glm::vec3 m_Forward;

	glm::vec3 m_Eulers;

	glm::vec3 m_Origin;
	glm::vec3 m_Scale;
	glm::quat m_Rotation;
	glm::vec3 m_Translation;
};
