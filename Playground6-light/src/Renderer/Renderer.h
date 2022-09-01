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

class Renderer
{
public:
    static void Clear();
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
};
