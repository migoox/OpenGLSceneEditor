#include "NodeTreePanel.h"
#include "Light.h"
#include "Model.h"
#include "Cuboid.h"
#include "imgui/imgui_stdlib.h"

namespace ImGui {
    std::vector<float> alphas;
    void PushDisabled(bool disabled = true) {
        alphas.push_back(GetStyle().Alpha);
        GetStyle().Alpha = disabled ? 0.25f : 1.0f;
    }
    void PopDisabled(int num = 1) {
        while (num--) {
            GetStyle().Alpha = alphas.back();
            alphas.pop_back();
        }
    }
}

void UINodeTreePanel::Init()
{
    m_AddWindowActivated = false;
    m_ButtonDisabled = false;
}

void UINodeTreePanel::Display(std::vector<Node>& nodes, int& selectedIndex)
{
    ImGui::Begin("Tree panel");
    std::string label;

    ImGui::BeginChild("tree", ImVec2(0, 200), true);
    for (int i = 0; i < nodes.size(); i++)
    {
        if(nodes[i].IsVisible())
            label = "[" + std::to_string(i) + "] " + nodes[i].GetName();
        else
            label = "[" + std::to_string(i) + "] " + "[invisible]" + nodes[i].GetName();

        if (ImGui::Selectable(label.c_str(), selectedIndex == i))
            selectedIndex = i;
    }
    ImGui::EndChild();

    if (m_AddWindowActivated)
    {
        m_ButtonDisabled = true;
        ImGui::PushDisabled();
    }

    if (ImGui::Button("Add"))
    {
        m_AddWindowActivated = true;
    }

    if (m_ButtonDisabled)
    {
        m_ButtonDisabled = false;
        ImGui::PopDisabled();
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete"))
    {
        if (selectedIndex >= 0)
        {
            nodes.erase(nodes.begin() += selectedIndex);
            selectedIndex = -1;
        }
    }

    ImGui::End();

    if (m_AddWindowActivated)
    {
        ImGui::Begin("Add object");
        static std::string name = "Default Light";
        static int current = 0;
        {
            if(ImGui::Combo("ObjectType", &current,
                "Default Light\0"
                "Directional Light\0"
                "Point Light\0"
                "Spotlight\0"
                "Model\0"
                "Cube\0"))
                if (current == 0)
                    name = "Default Light";
                else if (current == 1)
                    name = "Directional Light";
                else if (current == 2)
                    name = "Point Light";
                else if (current == 3)
                    name = "Spotlight";
                else if (current == 4)
                    name = "Model";
                else
                    name = "Cube";
        }

        {
            ImGui::InputText("Name", &name);
        }

        if (ImGui::Button("Add"))
        {
            Node newNode;
            if (current == 0)
                newNode.Create<Light>();
            else if (current == 1)
                newNode.Create<DirectionalLight>();
            else if (current == 2)
                newNode.Create<PointLight>();
            else if (current == 3)
                newNode.Create<Spotlight>();
            else if (current == 4)
                newNode.Create<Model>();
            else
                newNode.Create<Cuboid>();

            newNode.SetName(name);
            nodes.push_back(newNode);

            m_AddWindowActivated = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            m_AddWindowActivated = false;
        }

        ImGui::End();
    }
}
