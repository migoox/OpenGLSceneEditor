#pragma once
#include "FPCamera.h"
#include "Shader.h"
#include "Renderer.h"
#include "Cuboid.h"
#include "Light.h"

// to do: shader manager
std::shared_ptr<Shader> ObjectShader;
std::shared_ptr<Shader> LightSourceShader;
// --------------------


class CameraLayer : public Layer
{
public:
    virtual void OnAttach() override
    {
        // to do: shader manager
        ObjectShader = std::shared_ptr<Shader>(new Shader("res/shaders/Material.shader"));
        LightSourceShader = std::shared_ptr<Shader>(new Shader("res/shaders/LightSource.shader"));
        // --------------------

        m_EPressed = false;

        Renderer::SetClearColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.f));

        Input::SetCursorMode(CursorMode::Locked);
        float width = (float)Application::GetSpecification().WindowSize.x;
        float height = (float)Application::GetSpecification().WindowSize.y;
        m_Camera.SetWidthByHeightRatio(width / height);
    }

    virtual void OnUpdate(float dTime) override
    {
        // freeze camera 
        if (Input::IsKeyDown(KeyCode::E))
        {
            if (!m_EPressed)
            {
                if (m_Camera.IsFreezed())
                {
                    m_Camera.Freeze(false);
                    Input::SetCursorMode(CursorMode::Locked);
                }
                else
                {
                    m_Camera.Freeze(true);
                    Input::SetCursorMode(CursorMode::Normal);
                }
            }
            m_EPressed = true;
        }
        else
        {
            m_EPressed = false;
        }

        m_Camera.UpdateCamera(dTime);

        // update object shader
        ObjectShader->Bind();
        ObjectShader->SetUniformMat4f("u_ViewMatrix", m_Camera.GetViewMatrix());
        ObjectShader->SetUniformMat4f("u_ProjectionMatrix", m_Camera.GetProjectionMatrix());
        ObjectShader->SetUniform3f("u_ViewerPosition", m_Camera.GetTranslation());
        ObjectShader->Unbind();

        // update light source shader
        LightSourceShader->Bind();
        LightSourceShader->SetUniformMat4f("u_ViewMatrix", m_Camera.GetViewMatrix());
        LightSourceShader->SetUniformMat4f("u_ProjectionMatrix", m_Camera.GetProjectionMatrix());
        LightSourceShader->Unbind();
    }

private:
    FPCamera m_Camera;
    bool m_EPressed;
};

class SceneLayer : public Layer
{
public:
    virtual void OnAttach() override
    {
        InitCubes();
        InitLights();
    }

    virtual void OnUpdate(float dTime) override
    {
        // light
        for (auto& object : m_Objects)
        {
            Light* light = dynamic_cast<Light*>(object);
            if (light)
            {
                light->UpdateShader(*ObjectShader);

                LightSourceShader->Bind();
                LightSourceShader->SetUniformMat4f("u_ModelMatrix", light->GetModelMatrix());
                LightSourceShader->SetUniform4f("u_LightColor", light->GetRepresentation().GetColor());
                LightSourceShader->Unbind();

                Renderer::Draw(light->GetRepresentation().GetMesh(), *LightSourceShader);
            }
        }

        // cubes
        for (auto& object : m_Objects)
        {
            Cube* cube = dynamic_cast<Cube*>(object);
            if (cube)
            {
                ObjectShader->Bind();
                ObjectShader->SetUniformMat4f("u_ModelMatrix", cube->GetModelMatrix());
                ObjectShader->SetUniformMat3f("u_NormalMatrix", cube->GetNormalMatrix());
                ObjectShader->SetUniform1i("u_Material.diffuse", 0);
                ObjectShader->SetUniform1i("u_Material.specular", 1);
                ObjectShader->SetUniform1f("u_Material.shininess", cube->GetMaterial().shininess);
                ObjectShader->Unbind();

                // draw object
                cube->GetTexture()->Bind(0);
                cube->GetSpecularMap()->Bind(1);

                Renderer::Draw(cube->GetMesh(), *ObjectShader);

                cube->GetTexture()->Unbind();
                cube->GetSpecularMap()->Unbind();
            }
        }

    }

    virtual void OnImGuiRender()
    {
        std::string text;

        ImGui::Begin("Objects panel");
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Objects");

        // left
        static int selected = 0;
        {
            ImGui::BeginChild("left pane", ImVec2(150, 0), true);
            for (int i = 0; i < m_Objects.size(); i++)
            {
                text = "[" + std::to_string(i) + "] " + m_Objects[i]->GetName();
                if (ImGui::Selectable(text.c_str(), selected == i))
                    selected = i;
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();

        // right
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            
            text = "Type: " + m_Objects[selected]->GetName();

            ImGui::Text(text.c_str());
            ImGui::Separator();
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Info"))
                {
                    m_Objects[selected]->ImGuiDrawInfoPanel();
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::EndChild();
            ImGui::EndGroup();
        }

        ImGui::SameLine();
        ImGui::End();
    }

private:
    void InitCubes(unsigned int count = 10)
    {
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
            Cube* cube = new Cube(0.3f);
            cube->SetMaterial(mat1);

            glm::vec3 dir = glm::normalize(glm::vec3(random(), random(), random()));
            float magnitude = 0.5f + ((random() + 1.f) / 2.f) * 1.f;
            cube->Translate(dir * magnitude);
            cube->SetRotation(glm::vec3(random() * glm::pi<float>(),
                 random() * glm::pi<float>(),
                 random() * glm::pi<float>()));
            cube->Translate(glm::vec3(0.f, 0.f, 2.f));
            m_Objects.push_back(cube);
        }
    }
    void InitLights()
    {
        DirectionalLight* dlight = new DirectionalLight();
        dlight->SetTranslation(glm::vec3(0.f, 3.f, 3.f));
        dlight->SetRotation(glm::vec3(-0.5f, 0.f, 0.f));
        dlight->Rotate(glm::vec3(0.f, 3.f, 0.f), Space::Global);
        m_Objects.push_back(dlight);

        PointLight* plight = new PointLight(1.f, 0.1f, 0.01f);
        plight->SetTranslation(glm::vec3(0.f, 0.f, 2.f));
        m_Objects.push_back(plight);

        Spotlight* slight = new Spotlight();
        slight->SetTranslation(glm::vec3(0.f, 0.f, 5.f));
        slight->SetRotation(glm::vec3(0.f, glm::radians(180.f), 0.f));
        m_Objects.push_back(slight);
    }
private:
    std::vector<SceneObject*> m_Objects;
};