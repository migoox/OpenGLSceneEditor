#include "Application.h"
#include "MyLayers.h"
#include "Scene.h"
#include "glm/glm.hpp"

Application* CreateApplication(int argc, char** argv);

glm::quat ToQuaternion(double yaw, double pitch, double roll) // yaw (Z), pitch (Y), roll (X)
{
	// Abbreviations for the various angular functions
	double cy = cos(yaw * 0.5);
	double sy = sin(yaw * 0.5);
	double cp = cos(pitch * 0.5);
	double sp = sin(pitch * 0.5);
	double cr = cos(roll * 0.5);
	double sr = sin(roll * 0.5);

	glm::quat q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}


glm::vec3 ToEulerAngles(glm::quat q) {
	glm::vec3 angles;
	glm::mat4 m = glm::mat4_cast(q);

	if (m[2][0] < 1.f)
	{
		if (m[2][0] > -1.f)
		{
			angles.y = asin(-m[2][0]);
			angles.z = atan2(m[1][0], m[0][0]);
			angles.x = atan2(m[2][1], m[2][2]);
		}
		else
		{
			angles.y = glm::pi<float>() / 2.f;
			angles.z = -atan2(-m[1][2], m[1][1]);
			angles.x = 0.f;
		}
	}
	else
	{
		angles.y = - glm::pi<float>() / 2.f;
		angles.z = -atan2(-m[1][2], m[1][1]);
		angles.x = 0.f;
	}
	return angles;
}

int main(int argc, char** argv)
{
	/*Application* app = CreateApplication(argc, argv);
	app->Run();
	delete app;*/
	glm::vec3 eulers_rad = glm::vec3(0.f, 50.f, 0.f);
	std::cout << glm::to_string(eulers_rad) << std::endl;

	//glm::quat m_Rotation = ToQuaternion(glm::radians(eulers_rad.z), glm::radians(eulers_rad.y), glm::radians(eulers_rad.x));

	glm::quat m_Rotation = glm::angleAxis(glm::radians(eulers_rad.z), glm::vec3(0.0f, 0.f, 1.f)) *	// roll
						   glm::angleAxis(glm::radians(eulers_rad.y), glm::vec3(0.0f, 1.f, 0.f)) *	// yaw
						   glm::angleAxis(glm::radians(eulers_rad.x), glm::vec3(1.0f, 0.f, 0.f)); 	// pitch

	
	glm::vec3 result = ToEulerAngles(m_Rotation);
	/*glm::vec3 result = glm::eulerAngles(m_Rotation);

	if (eulers_rad.y > glm::pi<float>() / 2.f)
	{
		result.x -= glm::pi<float>();
		result.y -= glm::pi<float>();
		result.y *= -1.f;
		result.z += glm::pi<float>();

		if (eulers_rad.x > 0.f)
		{
			result.x += glm::pi<float>() * 2.f;
		}
	}*/


	std::cout << glm::to_string(glm::degrees(result)) << std::endl;

	return 0;

}

Application* CreateApplication(int argc, char** argv)
{
	ApplicationSpecification spec;
	spec.Name = "Name";
	spec.WindowSize = { 1600, 900 };

	Application* app = new Application(spec);
	app->PushLayer<Scene>();
	//app->PushLayer<SceneLayer>();
	return app;
}