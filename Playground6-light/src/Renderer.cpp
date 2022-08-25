#include "Renderer.h"

glm::vec4 Renderer::s_ClearColor = glm::vec4(0.f, 0.f, 0.f, 1.f);

void Renderer::Clear() 
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
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


