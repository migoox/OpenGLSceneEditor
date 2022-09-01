#include "Renderer.h"

glm::vec4 Renderer::s_ClearColor = glm::vec4(0.f, 0.f, 0.f, 1.f);
unsigned int Renderer::s_FrameLightSize = 0;
unsigned int Renderer::s_FrameDirLightSize = 0;
unsigned int Renderer::s_FramePointLightSize = 0;
unsigned int Renderer::s_FrameSpotlightSize = 0;

void Renderer::Clear() 
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::ClearLights(Shader& objectShader)
{
    glm::vec3 black(0.f, 0.f, 0.f);
    // default light
    for (unsigned int i = 0; i < s_FrameLightSize; i++)
    {
        objectShader.Bind();
        objectShader.SetUniform3f("u_Lights[" + std::to_string(i) + "].ambient", black);
        objectShader.SetUniform3f("u_Lights[" + std::to_string(i) + "].diffuse", black);
        objectShader.SetUniform3f("u_Lights[" + std::to_string(i) + "].specular", black);
        objectShader.Unbind();
    }

    // directional light
    for (unsigned int i = 0; i < s_FrameDirLightSize; i++)
    {
        objectShader.Bind();
        objectShader.SetUniform3f("u_DirectionalLights[" + std::to_string(i) + "].ambient", black);
        objectShader.SetUniform3f("u_DirectionalLights[" + std::to_string(i) + "].diffuse", black);
        objectShader.SetUniform3f("u_DirectionalLights[" + std::to_string(i) + "].specular", black);
        objectShader.Unbind();
    }

    // point light
    for (unsigned int i = 0; i < s_FramePointLightSize; i++)
    {
        objectShader.Bind();
        objectShader.SetUniform3f("u_PointLights[" + std::to_string(i) + "].ambient", black);
        objectShader.SetUniform3f("u_PointLights[" + std::to_string(i) + "].diffuse", black);
        objectShader.SetUniform3f("u_PointLights[" + std::to_string(i) + "].specular", black);
        objectShader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].constant", 1.f);
        objectShader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].linear", 1.f);
        objectShader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].quadratic", 1.f);
        objectShader.Unbind();
    }

    // spotlight
    for (unsigned int i = 0; i < s_FrameSpotlightSize; i++)
    {
        objectShader.Bind();
        objectShader.SetUniform3f("u_Spotlights[" + std::to_string(i) + "].ambient", black);
        objectShader.SetUniform3f("u_Spotlights[" + std::to_string(i) + "].diffuse", black);
        objectShader.SetUniform3f("u_Spotlights[" + std::to_string(i) + "].specular", black);
        objectShader.Unbind();
    }

    s_FrameLightSize = 0;
    s_FrameDirLightSize = 0;
    s_FramePointLightSize = 0;
    s_FrameSpotlightSize = 0;
}

void Renderer::PushLight(Light& light, Shader& objectShader)
{
    objectShader.Bind();
    objectShader.SetUniform3f("u_Lights[" + std::to_string(s_FrameLightSize) + "].position",    light.GetTranslation());
    objectShader.SetUniform3f("u_Lights[" + std::to_string(s_FrameLightSize) + "].ambient",     light.GetAmbient());
    objectShader.SetUniform3f("u_Lights[" + std::to_string(s_FrameLightSize) + "].diffuse",     light.GetDiffuse());
    objectShader.SetUniform3f("u_Lights[" + std::to_string(s_FrameLightSize) + "].specular",    light.GetSpecular());

    s_FrameLightSize++;
    objectShader.SetUniform1i("u_DefaultLightsCount", s_FrameLightSize);
    objectShader.Unbind();
}

void Renderer::PushLight(DirectionalLight& light, Shader& objectShader)
{
    objectShader.Bind();
    objectShader.SetUniform3f("u_DirectionalLights[" + std::to_string(s_FrameDirLightSize) + "].direction", light.GetDirection());
    objectShader.SetUniform3f("u_DirectionalLights[" + std::to_string(s_FrameDirLightSize) + "].ambient",   light.GetAmbient());
    objectShader.SetUniform3f("u_DirectionalLights[" + std::to_string(s_FrameDirLightSize) + "].diffuse",   light.GetDiffuse());
    objectShader.SetUniform3f("u_DirectionalLights[" + std::to_string(s_FrameDirLightSize) + "].specular",  light.GetSpecular());

    s_FrameDirLightSize++;
    objectShader.SetUniform1i("u_DirectionalLightsCount", s_FrameDirLightSize);
    objectShader.Unbind();
}

void Renderer::PushLight(PointLight& light, Shader& objectShader)
{
    objectShader.Bind();
    objectShader.SetUniform3f("u_PointLights[" + std::to_string(s_FramePointLightSize) + "].position",  light.GetTranslation());
    objectShader.SetUniform3f("u_PointLights[" + std::to_string(s_FramePointLightSize) + "].ambient",   light.GetAmbient());
    objectShader.SetUniform3f("u_PointLights[" + std::to_string(s_FramePointLightSize) + "].diffuse",   light.GetDiffuse());
    objectShader.SetUniform3f("u_PointLights[" + std::to_string(s_FramePointLightSize) + "].specular",  light.GetSpecular());
    objectShader.SetUniform1f("u_PointLights[" + std::to_string(s_FramePointLightSize) + "].constant",  light.GetConstant());
    objectShader.SetUniform1f("u_PointLights[" + std::to_string(s_FramePointLightSize) + "].linear",    light.GetLinear());
    objectShader.SetUniform1f("u_PointLights[" + std::to_string(s_FramePointLightSize) + "].quadratic", light.GetQuadratic());

    s_FramePointLightSize++;
    objectShader.SetUniform1i("u_PointLightsCount", s_FramePointLightSize);
    objectShader.Unbind();
}

void Renderer::PushLight(Spotlight& light, Shader& objectShader)
{
    objectShader.Bind();
    objectShader.SetUniform3f("u_Spotlights[" + std::to_string(s_FrameSpotlightSize) + "].position",    light.GetTranslation());
    objectShader.SetUniform3f("u_Spotlights[" + std::to_string(s_FrameSpotlightSize) + "].direction",   light.GetDirection());
    objectShader.SetUniform3f("u_Spotlights[" + std::to_string(s_FrameSpotlightSize) + "].ambient",     light.GetAmbient());
    objectShader.SetUniform3f("u_Spotlights[" + std::to_string(s_FrameSpotlightSize) + "].diffuse",     light.GetDiffuse());
    objectShader.SetUniform3f("u_Spotlights[" + std::to_string(s_FrameSpotlightSize) + "].specular",    light.GetSpecular());
    objectShader.SetUniform1f("u_Spotlights[" + std::to_string(s_FrameSpotlightSize) + "].cutOff",      light.GetCutOffAngle());
    objectShader.SetUniform1f("u_Spotlights[" + std::to_string(s_FrameSpotlightSize) + "].outerCutOff", light.GetOuterCutOffAngle());

    s_FrameSpotlightSize++;
    objectShader.SetUniform1i("u_SpotlightsCount", s_FrameSpotlightSize);
    objectShader.Unbind();
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) 
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall( glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr) );
    shader.Unbind();
    va.Unbind();
    ib.Unbind();
}

void Renderer::Draw(const Mesh& mesh, const Shader& shader) 
{
    shader.Bind();
    mesh.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndices().size(), GL_UNSIGNED_INT, nullptr));
    mesh.Unbind();
    shader.Unbind();
}

void Renderer::Draw(const Mesh& mesh, Shader& shader, const Material& material)
{
    shader.Bind();
    mesh.Bind();

    std::string name;
    unsigned int slot = 0;

    shader.SetUniform3f("u_Material.ambient", material.Ambient);

    shader.SetUniform3f("u_Material.diffuse", material.Diffuse);

    if (material.Texture.size() == 0)
        shader.SetUniform1i("u_Material.noDiffTexture", 1);
    else
        shader.SetUniform1i("u_Material.noDiffTexture", 0);

    for (unsigned int i = 0; i < material.Texture.size(); i++)
    {
        material.Texture[i]->Bind(slot);
        name = "u_Material.diffuse" + std::to_string(i);
        shader.SetUniform1i(name.c_str(), slot);
        slot++;
    }

    shader.SetUniform3f("u_Material.specular", material.Specular);

    if (material.SpecularMap.size() == 0)
        shader.SetUniform1i("u_Material.noSpecMap", 1);
    else
        shader.SetUniform1i("u_Material.noSpecMap", 0);

    for (unsigned int i = 0; i < material.SpecularMap.size(); i++)
    {
        material.SpecularMap[i]->Bind(slot);
        name = "u_Material.specular" + std::to_string(i);
        shader.SetUniform1i(name.c_str(), slot);
        slot++;
    }

    shader.SetUniform1f("u_Material.shininess", material.Shininess);

    GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndices().size(), GL_UNSIGNED_INT, nullptr));

    for (unsigned int i = 0; i < material.Texture.size(); i++)
        material.Texture[i]->Unbind();

    for (unsigned int i = 0; i < material.SpecularMap.size(); i++)
        material.SpecularMap[i]->Unbind();

    mesh.Unbind();
    shader.Unbind();
}

void Renderer::Draw(const Mesh& mesh, Shader& shader, const Transform& transform, const Camera& camera) 
{
    shader.Bind();
    mesh.Bind();

    shader.SetUniformMat4f("u_ModelMatrix", transform.GetModelMatrix());
    shader.SetUniformMat4f("u_ViewMatrix", camera.GetViewMatrix());
    shader.SetUniformMat4f("u_ProjectionMatrix", camera.GetProjectionMatrix());

    GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndices().size(), GL_UNSIGNED_INT, nullptr));

    mesh.Unbind();
    shader.Unbind();
}

void Renderer::Draw(const Mesh& mesh, const Texture& texture, Shader& shader, const Transform& transform, const Camera& camera) 
{
    shader.Bind();
    mesh.Bind();
    texture.Bind(0);

    shader.SetUniformMat4f("u_ModelMatrix", transform.GetModelMatrix());
    shader.SetUniformMat4f("u_ViewMatrix", camera.GetViewMatrix());
    shader.SetUniformMat4f("u_ProjectionMatrix", camera.GetProjectionMatrix());
    shader.SetUniform1i("u_Texture", 0);

    GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndices().size(), GL_UNSIGNED_INT, nullptr));

    texture.Unbind();
    mesh.Unbind();
    shader.Unbind();
}


