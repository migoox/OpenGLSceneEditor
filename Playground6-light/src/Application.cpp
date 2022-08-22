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
    GLFWwindow* window = glfwCreateWindow(winX, winY, "Cuboid", NULL, NULL);
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

void InitObjects(std::vector<Cuboid>& cuboids, const unsigned int count)
{
    cuboids.reserve(count);
    srand(time(NULL));
    auto random = []() -> float { return (float)(rand() % 1999) / 1000.f - 1.f; };
    for (unsigned int i = 0; i < count; i++)
    {
        Cuboid cuboid = Cuboid(0.4f, 0.4f, 0.4f);
        glm::vec3 dir = glm::normalize(glm::vec3(random(), random(), random()));
        float magnitude = 0.5f + ((random() + 1.f) / 2.f) * 2.f;
        cuboid.Translate(dir * magnitude);
        cuboid.SetRotation(glm::vec3(random() * glm::pi<float>(),
                                    random() * glm::pi<float>(), 
                                    random() * glm::pi<float>()));

        cuboid.SetMaterial(Materials::GetMaterial(static_cast<Materials::MaterialType>(rand() % 4)));
        //cuboid.SetMaterial({glm::vec3(0.73f, 1.f, 0.2f), glm::vec3(0.73f, 1.f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), 256.f});
        cuboids.push_back(cuboid);
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

void UpdateImGuiLightColor(Light& light)
{
    static float color[] = { 1.f, 1.f, 1.f };
    static float ambientAndDiffuse[] = { 1.f, 1.f};

    ImGui::Begin("Light Color ");
    ImGui::ColorEdit3("", color);
    ImGui::SliderFloat2("Ambient, Diffuse", ambientAndDiffuse, 0.f, 1.f);
    light.SetLightByColor(glm::vec3(color[0], color[1], color[2]), 
                          glm::vec3(ambientAndDiffuse[0]), glm::vec3(ambientAndDiffuse[1]));
    ImGui::End();
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
    std::vector<Cuboid> objects;
    InitObjects(objects, 16);

    // light
    Light light;
    light.SetColor(glm::vec4(1.f, 1.f, 1.f, 1.f));

    // shaders
    Shader objectShader("res/shaders/Object.shader");
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
        UpdateImGuiLightColor(light);

        // update frame
        UpdateTransform(window, &light, deltaTime);
        UpdateCamera(window, camera, deltaTime);

        // update projection and view matrices for object and light
        objectShader.Bind();
        objectShader.SetUniformMat4f("u_ViewMatrix", camera.GetViewMatrix());
        objectShader.SetUniformMat4f("u_ProjectionMatrix", camera.GetProjectionMatrix());
        objectShader.SetUniform4f("u_LightColor", light.GetColor());
        objectShader.SetUniform3f("u_Light.position", light.GetTranslation());
        objectShader.SetUniform3f("u_Light.ambient", light.GetAmbient());
        objectShader.SetUniform3f("u_Light.diffuse", light.GetDiffuse());
        objectShader.SetUniform3f("u_Light.specular", light.GetSpecular());
        objectShader.SetUniform3f("u_ViewerPosition", camera.GetTranslation());
        objectShader.Unbind();

        // draw objects
        for (auto& object : objects)
        {
            // update model matrix and material of current object
            objectShader.Bind();
            objectShader.SetUniformMat4f("u_ModelMatrix", object.GetModelMatrix());
            objectShader.SetUniformMat3f("u_NormalMatrix", object.GetNormalMatrix());
            objectShader.SetUniform3f("u_Material.ambient", object.GetMaterial().ambient);
            objectShader.SetUniform3f("u_Material.diffuse", object.GetMaterial().diffuse);
            objectShader.SetUniform3f("u_Material.specular", object.GetMaterial().specular);
            objectShader.SetUniform1f("u_Material.shininess", object.GetMaterial().shininess);
            objectShader.Unbind();

            // draw object
            renderer.Draw(object.GetMesh(), objectShader);
        }
        
        // update light source
        lightSourceShader.Bind();
        lightSourceShader.SetUniformMat4f("u_ModelMatrix", light.GetModelMatrix());
        lightSourceShader.SetUniformMat4f("u_ViewMatrix", camera.GetViewMatrix());
        lightSourceShader.SetUniformMat4f("u_ProjectionMatrix", camera.GetProjectionMatrix());
        lightSourceShader.SetUniform4f("u_LightColor", light.GetColor());
        lightSourceShader.Unbind();

        // draw light representation
        renderer.Draw(light.GetMesh(), lightSourceShader);

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


