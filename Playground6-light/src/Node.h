#pragma once
#include "Transfrom.h"
#include <iostream>

class Node
{
public:
	Node() : m_Blank(true), m_Name("no name") { }
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

	std::shared_ptr<Transform>& GetObject() { return m_SceneObject; }
	const std::shared_ptr<Transform>& GetObject() const { return m_SceneObject; }
	
	bool IsBlank() const { return m_Blank; }
	size_t GetObjectType() const { return m_ObjectType; }

	void SetName(const std::string& name) { m_Name = name; }
	std::string GetName() const { return m_Name; }

	Transform& GetObjectTransform() { return *m_SceneObject; }

private:
	std::shared_ptr<Transform> m_SceneObject;
	std::string m_Name;
	size_t m_ObjectType;
	bool m_Blank;
};