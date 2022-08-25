#pragma once
#include <vector>

#include "glm/glm.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoords;
	glm::vec3 Normal;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	inline const std::vector<Vertex>& GetVertices()		const	{ return m_Vertices; }
	inline const std::vector<unsigned int>& GetIndices()	const	{ return m_Indices;  }
	
	void Set(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

	void Set(float *vertices, unsigned int vcount, unsigned int *indices, unsigned int icount);

	void AddVertex(glm::vec3 vertex, unsigned int index0, unsigned int index1);

	void Bind()   const;
	void Unbind() const;

private:
	// data
	std::vector<Vertex>				m_Vertices;
	std::vector<unsigned int>		m_Indices;

	// renderer
	std::shared_ptr<VertexArray>	m_VAO;
	std::shared_ptr<VertexBuffer>	m_VBO;
	std::shared_ptr<IndexBuffer>	m_IBO;
};

