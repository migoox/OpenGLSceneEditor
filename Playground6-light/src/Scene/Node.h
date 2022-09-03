#pragma once
#include "Transfrom.h"
#include "Material.h"
#include "Cube.h"
#include "Model.h"
#include <iostream>

class Node
{
public:
	Node() : m_Blank(true), m_Name("no name"), m_Visibilty(true) { }
	~Node() { }

	// TO DO: parent/child

	// obj must be dynamicaly allocated
	template<typename T>
	void Create()
	{
		if (!std::is_base_of<Transform, T>::value)
		{
			std::cout << "[Scene Error]: Transform is not a base of " << typeid(T).name() << "." << std::endl;

			return;
		}

		m_SceneObject = std::shared_ptr<Transform>(new T());
		m_ObjectType = typeid(T).hash_code();
		m_Blank = false;
	}

	bool HasMaterial()
	{
		if (m_ObjectType == typeid(Cube).hash_code() ||
			m_ObjectType == typeid(Model).hash_code())
		{
			return true;
		}
		return false;
	}
	Material GetMaterial()
	{
		if (m_ObjectType == typeid(Cube).hash_code())
		{
			auto cube = static_cast<Cube*>(m_SceneObject.get());
			return cube->GetMaterial();
		}
		if (m_ObjectType == typeid(Model).hash_code())
		{
			auto model = static_cast<Model*>(m_SceneObject.get());
			return model->GetMaterial();
		}
		return { };
	}

	void SetMaterial(Material material)
	{
		if (m_ObjectType == typeid(Cube).hash_code())
		{
			auto cube = static_cast<Cube*>(m_SceneObject.get());
			cube->SetMaterial(material);
		}
		if (m_ObjectType == typeid(Model).hash_code())
		{
			auto model = static_cast<Model*>(m_SceneObject.get());
			model->SetMaterial(material);
		}
	}

	std::shared_ptr<Transform>& GetObject() { return m_SceneObject; }
	const std::shared_ptr<Transform>& GetObject() const { return m_SceneObject; }
	
	bool IsBlank() const { return m_Blank; }
	size_t GetObjectType() const { return m_ObjectType; }

	void SetName(const std::string& name) { m_Name = name; }
	std::string GetName() const { return m_Name; }

	Transform& GetObjectTransform() { return *m_SceneObject; }

	bool IsVisible() const { return m_Visibilty; }
	void SetVisibility(bool visibility) { m_Visibilty = visibility; }

private:
	std::shared_ptr<Transform> m_SceneObject;
	std::string m_Name;
	size_t m_ObjectType;
	bool m_Blank;
	bool m_Visibilty;
};