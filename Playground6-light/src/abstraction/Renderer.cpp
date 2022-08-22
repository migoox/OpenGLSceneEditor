#include "Renderer.h"
#include <iostream>

void Renderer::Clear() const
{
        GLCall( glClear( GL_COLOR_BUFFER_BIT ) );
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
        shader.Bind();
        va.Bind();
        ib.Bind();
        GLCall( glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr) );
        shader.Unbind();
        va.Unbind();
        ib.Unbind();
}

void Renderer::Draw(const Mesh& mesh, const Shader& shader) const
{
    shader.Bind();
    mesh.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndices().size(), GL_UNSIGNED_INT, nullptr));
    mesh.Unbind();
    shader.Unbind();
}

void Renderer::Draw(const Mesh& mesh, Shader& shader, const Transform& transform, const Camera& camera) const
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

void Renderer::Draw(const Mesh& mesh, const Texture& texture, Shader& shader, const Transform& transform, const Camera& camera) const
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


