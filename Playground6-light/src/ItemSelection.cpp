#include "ItemSelection.h"

ItemSelection::ItemSelection()
{
	m_XAxis.SetColor(glm::vec4(1.f, 0.f, 0.f, 1.f));
	m_XAxis.SetScale(glm::vec3(0.6f, 0.01f, 0.01f));

	m_YAxis.SetColor(glm::vec4(0.f, 1.f, 0.f, 1.f));
	m_YAxis.SetScale(glm::vec3(0.01f, 0.6f, 0.01f));

	m_ZAxis.SetColor(glm::vec4(0.f, 0.f, 1.f, 1.f));
	m_ZAxis.SetScale(glm::vec3(0.01f, 0.01f, 0.6f));
}

void ItemSelection::OnUpdate(Node& node, Shader& shader)
{
	glDepthFunc(GL_ALWAYS);
	auto quat = node.GetObjectTransform().GetQuaternion();
	auto pos = node.GetObjectTransform().GetTranslation();
	m_XAxis.SetQuaternion(quat);
	m_XAxis.SetTranslation(pos);
	m_YAxis.SetQuaternion(quat);
	m_YAxis.SetTranslation(pos);
	m_ZAxis.SetQuaternion(quat);
	m_ZAxis.SetTranslation(pos);

	shader.Bind();
	shader.SetUniformMat4f("u_ModelMatrix", m_XAxis.GetModelMatrix());
	shader.SetUniform4f("u_LightColor", m_XAxis.GetColor());
	shader.Unbind();

	Renderer::Draw(m_XAxis.GetMesh(), shader);

	shader.Bind();
	shader.SetUniformMat4f("u_ModelMatrix", m_YAxis.GetModelMatrix());
	shader.SetUniform4f("u_LightColor", m_YAxis.GetColor());
	shader.Unbind();

	Renderer::Draw(m_YAxis.GetMesh(), shader);

	shader.Bind();
	shader.SetUniformMat4f("u_ModelMatrix", m_ZAxis.GetModelMatrix());
	shader.SetUniform4f("u_LightColor", m_ZAxis.GetColor());
	shader.Unbind();

	Renderer::Draw(m_ZAxis.GetMesh(), shader);
	glDepthFunc(GL_LESS);
}
