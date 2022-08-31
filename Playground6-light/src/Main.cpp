#include "Application.h"
#include "MyLayers.h"
#include "Scene.h"
#include "glm/glm.hpp"

Application* CreateApplication(int argc, char** argv);

glm::quat ToQuaternion(glm::vec3 eulers) // zyx
{
	// Abbreviations for the various angular functions
	double cy = cos(eulers.z * 0.5);
	double sy = sin(eulers.z * 0.5);
	double cp = cos(eulers.y * 0.5);
	double sp = sin(eulers.y * 0.5);
	double cr = cos(eulers.x * 0.5);
	double sr = sin(eulers.x * 0.5);

	glm::quat q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

int main(int argc, char** argv)
{
	Application* app = CreateApplication(argc, argv);
	app->Run();
	delete app;

	glm::vec3 eulers = glm::radians(glm::vec3(95.f, 180.f, 20.f));
	glm::quat q1 =
		glm::angleAxis(eulers.x, glm::vec3(0.0f, 1.f, 0.f)) *	// yaw
		glm::angleAxis(eulers.y, glm::vec3(1.0f, 0.f, 0.f)) *	// pitch
		glm::angleAxis(eulers.z, glm::vec3(0.0f, 0.f, 1.f));	// roll;

	std::cout << glm::to_string(q1) << std::endl;

	eulers = glm::eulerAngles(q1);

	glm::quat q2 = ToQuaternion(eulers);

	std::cout << glm::to_string(q2) << std::endl;

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