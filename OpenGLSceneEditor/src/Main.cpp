#include "Application.h"
#include "Scene.h"
#include "glm/glm.hpp"

Application* CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	Application* app = CreateApplication(argc, argv);
	app->Run();
	delete app;
}

Application* CreateApplication(int argc, char** argv)
{
	ApplicationSpecification spec;
	spec.Name = "Name";
	spec.WindowSize = { 1600, 900 };

	Application* app = new Application(spec);
	app->PushLayer<Scene>();
	return app;
}