#include "Mesh.h"

Mesh::Mesh()
{
	m_VAO = std::unique_ptr<VertexArray>(new VertexArray());
	m_VAO->Bind();

	m_VBO = std::unique_ptr<VertexBuffer>(new VertexBuffer(NULL, 0));
	m_VBO->Bind();

	m_IBO = std::unique_ptr<IndexBuffer>(new IndexBuffer(NULL, 0));
	m_IBO->Bind();

	VertexBufferLayout layout;
	layout.AddFloat(3);
	layout.AddFloat(2);
	layout.AddFloat(3);
	m_VAO->AddBuffer(*m_VBO.get(), layout);

	m_VBO->Unbind();
	m_IBO->Unbind();
	m_VAO->Unbind();
}

Mesh::~Mesh()
{
}

void Mesh::Set(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	m_Vertices = vertices;
	m_Indices = indices;

	m_VAO->Bind();

	m_VBO->Bind();
	m_VBO->BufferData(m_Vertices.data(), m_Vertices.size() * (sizeof(Vertex)));
	m_VBO->Unbind();

	m_IBO->Bind();
	m_IBO->BufferData(m_Indices.data(), m_Indices.size());
	m_IBO->Unbind();

	m_VAO->Unbind();
}

void Mesh::Set(float* vertices, unsigned int vcount, unsigned int* indices, unsigned int icount)
{
	// TO DO
}

void Mesh::AddVertex(glm::vec3 vertex, unsigned int index0, unsigned int index1)
{
	// TO DO
}

void Mesh::Bind()   const
{
	m_VAO->Bind();
	m_IBO->Bind();
}

void Mesh::Unbind() const
{
	m_VBO->Unbind();
	m_IBO->Unbind();
	m_VAO->Unbind();
}
