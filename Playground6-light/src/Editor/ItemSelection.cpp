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

void ItemSelection::OnUpdate(Node& node)
{
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);	// replaces if stencil passes
	glStencilFunc(GL_ALWAYS, 1, 0xFF);				// draws fragment always and sets it's stencil value to 1
	glStencilMask(0xFF);							// enables updating stencil buffer

	if (node.GetObjectType() == typeid(Cube).hash_code())
	{
		auto cube = static_cast<Cube*>(node.GetObject().get());

		// draw cube and prepare stencil buffer
		ResourceManager::GetShader("ObjectShader")->Bind();
		ResourceManager::GetShader("ObjectShader")->SetUniformMat4f("u_ModelMatrix", node.GetObjectTransform().GetModelMatrix());
		ResourceManager::GetShader("ObjectShader")->SetUniformMat3f("u_NormalMatrix", node.GetObjectTransform().GetNormalMatrix());
		ResourceManager::GetShader("ObjectShader")->Unbind();

		Renderer::Draw(cube->GetMesh(), *ResourceManager::GetShader("ObjectShader"), cube->GetMaterial());

		// draw outline 
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);		// draws fragment if it's stencil value is 0
		glStencilMask(0x00);						// disable updating stencil buffer
		glDisable(GL_DEPTH_TEST);					// we want to see outline even though it's behind some other object
		
		ResourceManager::GetShader("OutlineShader")->Bind();
		ResourceManager::GetShader("OutlineShader")->SetUniformMat4f("u_ModelMatrix", node.GetObjectTransform().GetModelMatrix());
		ResourceManager::GetShader("OutlineShader")->SetUniformMat3f("u_NormalMatrix", node.GetObjectTransform().GetNormalMatrix());
		ResourceManager::GetShader("OutlineShader")->SetUniform4f("u_OutlineColor", glm::vec4(1.f, 0.f, 1.f, 1.f));
		ResourceManager::GetShader("OutlineShader")->Unbind();
		
		Renderer::Draw(cube->GetMesh(), *ResourceManager::GetShader("OutlineShader"));

		// restore depth and stencil buffer
		glStencilMask(0xFF);						// enable updating stencil buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFF);			// enable drawing no matter what
		glEnable(GL_DEPTH_TEST);
	}

	glDepthFunc(GL_ALWAYS);
	auto quat = node.GetObjectTransform().GetQuaternion();
	auto pos = node.GetObjectTransform().GetTranslation();
	m_XAxis.SetQuaternion(quat);
	m_XAxis.SetTranslation(pos);
	m_YAxis.SetQuaternion(quat);
	m_YAxis.SetTranslation(pos);
	m_ZAxis.SetQuaternion(quat);
	m_ZAxis.SetTranslation(pos);

	ResourceManager::GetShader("LightSourceShader")->Bind();
	ResourceManager::GetShader("LightSourceShader")->SetUniformMat4f("u_ModelMatrix", m_XAxis.GetModelMatrix());
	ResourceManager::GetShader("LightSourceShader")->SetUniform4f("u_LightColor", m_XAxis.GetColor());
	ResourceManager::GetShader("LightSourceShader")->Unbind();

	Renderer::Draw(m_XAxis.GetMesh(), *ResourceManager::GetShader("LightSourceShader"));

	ResourceManager::GetShader("LightSourceShader")->Bind();
	ResourceManager::GetShader("LightSourceShader")->SetUniformMat4f("u_ModelMatrix", m_YAxis.GetModelMatrix());
	ResourceManager::GetShader("LightSourceShader")->SetUniform4f("u_LightColor", m_YAxis.GetColor());
	ResourceManager::GetShader("LightSourceShader")->Unbind();

	Renderer::Draw(m_YAxis.GetMesh(), *ResourceManager::GetShader("LightSourceShader"));

	ResourceManager::GetShader("LightSourceShader")->Bind();
	ResourceManager::GetShader("LightSourceShader")->SetUniformMat4f("u_ModelMatrix", m_ZAxis.GetModelMatrix());
	ResourceManager::GetShader("LightSourceShader")->SetUniform4f("u_LightColor", m_ZAxis.GetColor());
	ResourceManager::GetShader("LightSourceShader")->Unbind();

	Renderer::Draw(m_ZAxis.GetMesh(), *ResourceManager::GetShader("LightSourceShader"));
	glDepthFunc(GL_LESS);
}
