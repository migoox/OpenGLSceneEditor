#pragma once

#include <GL/glew.h>

#include "Debug.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"

class Renderer
{
public:
    static void Clear();

    static void ClearLights(Shader& objectShader);

    static void PushLight(Light& light, Shader& objectShader);
    static void PushLight(DirectionalLight& light, Shader& objectShader);
    static void PushLight(PointLight& light, Shader& objectShader);
    static void PushLight(Spotlight& light, Shader& objectShader);

    // warning: if model doesn't follow counter-clockwise culling manier it won't work properly, also not closed objects
    // like 2d "bilboards" with texture will disapear when looking at them from behind
    static void FaceCulling(bool enabled) { enabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE); }

    static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
    static void Draw(const Mesh& mesh, const Shader& shader);
    static void Draw(const Mesh& mesh, Shader& shader, const Material& material);
    static void Draw(const Mesh& mesh, Shader& shader, const Transform& transform, const Camera& camera);
    static void Draw(const Mesh& mesh, const Texture& texture, Shader& shader, const Transform& transform, const Camera& camera);

    static void SetClearColor(glm::vec4 color) 
    { 
        s_ClearColor = color; 
        GLCall(glClearColor(s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a));
    }

    static glm::vec4 GetClearColor() { return s_ClearColor; }

private:
    static glm::vec4 s_ClearColor;
    static unsigned int s_FrameLightSize;
    static unsigned int s_FrameDirLightSize;
    static unsigned int s_FramePointLightSize;
    static unsigned int s_FrameSpotlightSize;
};
