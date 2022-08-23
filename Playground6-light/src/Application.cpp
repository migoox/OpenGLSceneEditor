#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Transfrom.h"
#include "Light.h"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/string_cast.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Cuboid.h"
#include "Camera.h"

#include <time.h>

int winX = 1024, winY = 768;

GLFWwindow* InitWindow()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return nullptr;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_REFRESH_RATE, 60);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(winX, winY, "Cube", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return nullptr;

    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return nullptr;
    }

    std::cout << "Using GL Version: " << glGetString(GL_VERSION) << std::endl;

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return window;
}

void InitLights(std::vector<Light>& lights, const unsigned int count)
{

}

void InitDirectionalLights(std::vector<DirectionalLight>& lights, const unsigned int count)
{
    DirectionalLight light;
    light.SetTranslation(glm::vec3(0.f, 3.f, 3.f));
    light.SetRotation(glm::vec3(-0.5f, 0.f, 0.f));
    light.Rotate(glm::vec3(0.f, 3.f, 0.f), Space::Global);
    lights.push_back(light);
}

void InitPointLights(std::vector<PointLight>& lights, const unsigned int count)
{
    PointLight light(1.f, 0.1f, 0.01f);
    light.SetTranslation(glm::vec3(0.f, 0.f, 2.f));
    lights.push_back(light);
}

void InitSpotlights(std::vector<Spotlight>& lights, const unsigned int count)
{
    Spotlight light;
    light.SetTranslation(glm::vec3(0.f, 0.f, 2.f));
    lights.push_back(light);
}

void InitSimpleObjects(std::vector<Cube>& cubes, const unsigned int count)
{
    cubes.reserve(count);
    srand(time(NULL));
    auto random = []() -> float { return (float)(rand() % 1999) / 1000.f - 1.f; };
    for (unsigned int i = 0; i < count; i++)
    {
        Cube cube = Cube(0.4f);
        glm::vec3 dir = glm::normalize(glm::vec3(random(), random(), random()));
        float magnitude = 0.5f + ((random() + 1.f) / 2.f) * 2.f;
        cube.Translate(dir * magnitude);
        cube.SetRotation(glm::vec3(random() * glm::pi<float>(),
                                    random() * glm::pi<float>(), 
                                    random() * glm::pi<float>()));

        cube.SetSimpleMaterial(SimpleMaterials::GetMaterial(static_cast<SimpleMaterials::Type>(rand() % 4)));
        //Cube.SetMaterial({glm::vec3(0.73f, 1.f, 0.2f), glm::vec3(0.73f, 1.f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), 256.f});
        cubes.push_back(cube);
    }
}

void InitObjects(std::vector<Cube>& cubes, const unsigned int count)
{
    cubes.reserve(count);
    srand(time(NULL));
    auto random = []() -> float { return (float)(rand() % 1999) / 1000.f - 1.f; };

    std::shared_ptr<Texture> texture1 = std::shared_ptr<Texture>(new Texture("res/textures/dice.png"));
    std::shared_ptr<Texture> specularMap1 = std::shared_ptr<Texture>(new Texture("res/specular_maps/marble_dice_specular.png"));
    std::shared_ptr<Texture> texture2 = std::shared_ptr<Texture>(new Texture("res/textures/box.png"));
    std::shared_ptr<Texture> specularMap2 = std::shared_ptr<Texture>(new Texture("res/specular_maps/box_specular.png"));

    Material mat1 = { specularMap1, texture1, 32.f };
    Material mat2 = { specularMap2, texture2, 256.f };

    for (unsigned int i = 0; i < count; i++)
    {
        Cube cube = Cube(0.3f);
        cube.SetMaterial(mat1);

        //Cube.SetSimpleMaterial(SimpleMaterials::GetMaterial(static_cast<SimpleMaterials::SimpleMaterialType>(rand() % 4)));
        //Cube.SetMaterial({glm::vec3(0.73f, 1.f, 0.2f), glm::vec3(0.73f, 1.f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), 256.f});
        glm::vec3 dir = glm::normalize(glm::vec3(random(), random(), random()));
        float magnitude = 0.5f + ((random() + 1.f) / 2.f) * 1.f;
        cube.Translate(dir * magnitude);
        cube.Translate(glm::vec3(0.f, 0.f, 2.f));
        cube.SetRotation(glm::vec3(random() * glm::pi<float>(),
            random() * glm::pi<float>(),
            random() * glm::pi<float>()));
        cubes.push_back(cube);
    }
}

void UpdateTransform(GLFWwindow* window, Transform* transform, float dt)
{
    const int rotSpeed = 2 * glm::pi<float>() / 2.0f;
    const int moveSpeed = 2.5f;
    int state;
    float roll = 0.f, pitch = 0.f, yaw = 0.f;

    state = glfwGetKey(window, GLFW_KEY_Z);
    if (state == GLFW_PRESS)
    {
        pitch = rotSpeed * dt;
    }

    state = glfwGetKey(window, GLFW_KEY_X);
    if (state == GLFW_PRESS)
    {
        yaw = rotSpeed * dt;
    }

    state = glfwGetKey(window, GLFW_KEY_C);
    if (state == GLFW_PRESS)
    {
        roll = rotSpeed * dt;
    }
    state = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
    if (state == GLFW_PRESS)
    {
        transform->Rotate(glm::vec3(pitch, yaw, roll), Space::Global);
    }
    else
    {
        transform->Rotate(glm::vec3(pitch, yaw, roll), Space::Local);
    }

    state = glfwGetKey(window, GLFW_KEY_I);
    if (state == GLFW_PRESS)
    {
        transform->Translate(glm::vec3(0.00f, 0.0f, moveSpeed * dt), Space::Local);
    }

    state = glfwGetKey(window, GLFW_KEY_K);
    if (state == GLFW_PRESS)
    {
        transform->Translate(glm::vec3(0.00f, 0.0f, -moveSpeed * dt), Space::Local);
    }

    state = glfwGetKey(window, GLFW_KEY_J);
    if (state == GLFW_PRESS)
    {
        transform->Translate(glm::vec3(-moveSpeed * dt, 0.0f, 0.f), Space::Local);
    }

    state = glfwGetKey(window, GLFW_KEY_L);
    if (state == GLFW_PRESS)
    {
        transform->Translate(glm::vec3(moveSpeed * dt, 0.0f, 0.f), Space::Local);
    }

    state = glfwGetKey(window, GLFW_KEY_P);
    if (state == GLFW_PRESS)
    {
        transform->Translate(glm::vec3(0.0f, moveSpeed * dt, 0.0f), Space::Local);
    }

    state = glfwGetKey(window, GLFW_KEY_O);
    if (state == GLFW_PRESS)
    {
        transform->Translate(glm::vec3(0.0f, -moveSpeed * dt, 0.0f), Space::Local);
    }
}

void UpdateCamera(GLFWwindow* window, Camera& camera, float dt)
{
    static float mLastX = winX / 2.f;
    static float mLastY = winY / 2.f;

    static bool firstFrame = true;
    static bool StaticCameraState = false;
    static bool EPressed = false;
    double xpos, ypos;
    int state;
    glfwGetCursorPos(window, &xpos, &ypos);
    float mouseX = (xpos - winX / 2.f) / winX * 2.0f;
    float mouseY = (winY / 2.0f - ypos) / winY * 2.0f;

    const float cameraSpeed = 1.25f;
    const float sensitivity = 0.05f;

    float offsetX = glm::radians(-sensitivity * (mLastX - xpos));
    float offsetY = glm::radians(sensitivity * (mLastY - ypos));
    mLastX = xpos;
    mLastY = ypos;

    state = glfwGetKey(window, GLFW_KEY_E);
    if (state == GLFW_PRESS)
    {
        if (!EPressed)
        {
            if (StaticCameraState)
                StaticCameraState = false;
            else
                StaticCameraState = true;
        }
        EPressed = true;
    }
    else
    {
        EPressed = false;
    }

    if (StaticCameraState)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    }
       
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!firstFrame)
    {
        camera.Rotate(glm::vec3(0.f, offsetX, 0.f), Space::Global);
        camera.Rotate(glm::vec3(-offsetY, 0.f, 0.f), Space::Local);
    }
    firstFrame = false;

    auto front = camera.GetForward();
    front.y = 0.f;
    front = glm::normalize(front);
    auto right = camera.GetRight();
    right.y = 0.f;
    right = glm::normalize(right);

    // forward
    state = glfwGetKey(window, GLFW_KEY_W);
    if (state == GLFW_PRESS)
    {
        camera.Translate(front * cameraSpeed * dt, Space::Global);
    }
    // backwards
    state = glfwGetKey(window, GLFW_KEY_S);
    if (state == GLFW_PRESS)
    {
        camera.Translate(-front * cameraSpeed * dt, Space::Global);
    }
    // right
    state = glfwGetKey(window, GLFW_KEY_D);
    if (state == GLFW_PRESS)
    {
        camera.Translate(right * cameraSpeed * dt, Space::Global);
    }
    // left
    state = glfwGetKey(window, GLFW_KEY_A);
    if (state == GLFW_PRESS)
    {
        camera.Translate(-right * cameraSpeed * dt, Space::Global);
    }
    // up
    state = glfwGetKey(window, GLFW_KEY_SPACE);
    if (state == GLFW_PRESS)
    {
        camera.Translate(glm::vec3(0.f, cameraSpeed * dt, 0.f), Space::Global);
    }
    // down
    state = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
    if (state == GLFW_PRESS)
    {
        camera.Translate(glm::vec3(0.f, -cameraSpeed * dt, 0.f), Space::Global);
    }
}

void UpdateImGuiFPS(float dt)
{
    ImGui::Begin("FPS");
    ImGui::Text(std::to_string(1.f / dt).c_str());
    ImGui::End();
}

void UpdateImGuiLightColor(Light& light, unsigned int index = 0)
{
    static float colors[40 * 3] = { 1.f };
    static float ambientAndDiffuses[40 * 2] = { 1.f };

    float color[3] = { colors[index * 3], colors[index * 3 + 1], colors[index * 3 + 2] };
    float ambientAndDiffuse[2] = { ambientAndDiffuses[index * 2], ambientAndDiffuses[index * 2 + 1] };

    std::string title = "Light " + std::to_string(index);

    ImGui::Begin(title.c_str());

    ImGui::ColorEdit3("Color", &(colors[index * 3]));
    ImGui::SliderFloat2("Ambient, Diffuse", &(ambientAndDiffuses[index * 2]), 0.f, 1.f);

    ImGui::End();

    light.SetLightByColor(glm::vec3(colors[index * 3], colors[index * 3 + 1], colors[index * 3 + 2]),
                          glm::vec3(ambientAndDiffuses[index * 2]), glm::vec3(ambientAndDiffuses[index * 2 + 1]));

    light.SetColor(glm::vec4(colors[index * 3], colors[index * 3 + 1], colors[index * 3 + 2], 1.f));
}

int main(void)
{
    GLFWwindow* window = InitWindow();
    if (!window)
        return -1;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // imgui initialization
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    // depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // background color
    glClearColor(0.05f, 0.05f, 0.05f, 1.f);

    // camera
    Camera camera(45.0f, (float)winX / (float)winY, 0.3f, 1000.0f);

    // objects
    std::vector<Cube> objects;
    //InitSimpleObjects(objects, 0);
    InitObjects(objects, 10);

    // lights
    std::vector<Light> lights;
    std::vector<DirectionalLight> directionalLights;
    std::vector<PointLight> pointLights;
    std::vector<Spotlight> spotlights;
    InitLights(lights, 0);
    InitDirectionalLights(directionalLights, 0);
    InitPointLights(pointLights, 0);
    InitSpotlights(spotlights, 0);
   
    // shaders
    Shader objectShader("res/shaders/Material.shader");
    Shader lightSourceShader("res/shaders/LightSource.shader");

    // renderer
    Renderer renderer;

    // time
    float deltaTime = 0.0f;	         // Time between current frame and last frame
    float lastFrame = glfwGetTime(); // Time of last frame

    do {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // clear framebuffers
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // prepare next imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // create imgui windows
        UpdateImGuiFPS(deltaTime);
        
        // update frame
        UpdateCamera(window, camera, deltaTime);

        // update projection and view matrices for object and light
        objectShader.Bind();
        objectShader.SetUniformMat4f("u_ViewMatrix", camera.GetViewMatrix());
        objectShader.SetUniformMat4f("u_ProjectionMatrix", camera.GetProjectionMatrix());
        objectShader.SetUniform3f("u_ViewerPosition", camera.GetTranslation());
        objectShader.Unbind();

        for (auto& currLight : lights)
        {
            UpdateImGuiLightColor(currLight, currLight.GetIndex());
            currLight.UpdateShader(objectShader);
        }

        for (auto& currLight : directionalLights)
        {
            UpdateImGuiLightColor(currLight, currLight.GetIndex() + 10);
            currLight.UpdateShader(objectShader);
        }

        for (auto& currLight : pointLights)
        {
            UpdateImGuiLightColor(currLight, currLight.GetIndex() + 20);
            currLight.UpdateShader(objectShader);
        }

        for (auto& currLight : spotlights)
        {
            UpdateTransform(window, &currLight, deltaTime);
            UpdateImGuiLightColor(currLight, currLight.GetIndex() + 30);
            currLight.UpdateShader(objectShader);
        }

        for (auto& currentObject : objects)
        {
            // update model matrix and material of current object
            objectShader.Bind();
            objectShader.SetUniformMat4f("u_ModelMatrix", currentObject.GetModelMatrix());
            objectShader.SetUniformMat3f("u_NormalMatrix", currentObject.GetNormalMatrix());
            objectShader.SetUniform1i("u_Material.diffuse", 0);
            objectShader.SetUniform1i("u_Material.specular", 1);
            objectShader.SetUniform1f("u_Material.shininess", 16.f);
            objectShader.Unbind();

            // draw object
            currentObject.GetTexture()->Bind(0);
            currentObject.GetSpecularMap()->Bind(1);
            renderer.Draw(currentObject.GetMesh(), objectShader);
            currentObject.GetTexture()->Unbind();
            currentObject.GetSpecularMap()->Unbind();
        }

        // draw light sources
        for (auto currLight : lights)
        {
            lightSourceShader.Bind();
            lightSourceShader.SetUniformMat4f("u_ModelMatrix", currLight.GetModelMatrix());
            lightSourceShader.SetUniformMat4f("u_ViewMatrix", camera.GetViewMatrix());
            lightSourceShader.SetUniformMat4f("u_ProjectionMatrix", camera.GetProjectionMatrix());
            lightSourceShader.SetUniform4f("u_LightColor", currLight.GetColor());
            lightSourceShader.Unbind();

            renderer.Draw(currLight.GetMesh(), lightSourceShader);
        }
        for (auto currLight : directionalLights)
        {
            lightSourceShader.Bind();
            lightSourceShader.SetUniformMat4f("u_ModelMatrix", currLight.GetModelMatrix());
            lightSourceShader.SetUniformMat4f("u_ViewMatrix", camera.GetViewMatrix());
            lightSourceShader.SetUniformMat4f("u_ProjectionMatrix", camera.GetProjectionMatrix());
            lightSourceShader.SetUniform4f("u_LightColor", currLight.GetColor());
            lightSourceShader.Unbind();

            renderer.Draw(currLight.GetMesh(), lightSourceShader);
        }
        for (auto currLight : pointLights)
        {
            lightSourceShader.Bind();
            lightSourceShader.SetUniformMat4f("u_ModelMatrix", currLight.GetModelMatrix());
            lightSourceShader.SetUniformMat4f("u_ViewMatrix", camera.GetViewMatrix());
            lightSourceShader.SetUniformMat4f("u_ProjectionMatrix", camera.GetProjectionMatrix());
            lightSourceShader.SetUniform4f("u_LightColor", currLight.GetColor());
            lightSourceShader.Unbind();

            renderer.Draw(currLight.GetMesh(), lightSourceShader);
        }
        for (auto currLight : spotlights)
        {
            lightSourceShader.Bind();
            lightSourceShader.SetUniformMat4f("u_ModelMatrix", currLight.GetModelMatrix());
            lightSourceShader.SetUniformMat4f("u_ViewMatrix", camera.GetViewMatrix());
            lightSourceShader.SetUniformMat4f("u_ProjectionMatrix", camera.GetProjectionMatrix());
            lightSourceShader.SetUniform4f("u_LightColor", currLight.GetColor());
            lightSourceShader.Unbind();

            renderer.Draw(currLight.GetMesh(), lightSourceShader);
        }
        // render imgui 
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
