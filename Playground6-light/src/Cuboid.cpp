#include "Cuboid.h"

Cuboid::Cuboid(float width, float length, float height)
{
	//    3--------2
	//   /|       /|
	//  / |      / |						   top  
	// 7--------6  | height						|  front
	// |  |     |  |							| /	
	// |  0-----|--1				  left _____|/_____ right
	// | /      | /							   /|
	// |/       |/ width					  /	|
	// 4--------5						  back	|
	//   length									bottom
	//
	// Model space is right-handed which means that:


	std::vector<Vertex> vertices =
	{
		// front (0 1 2 3)
		{ glm::vec3(0.f,	0.f,	width),   glm::vec2(1.f / 3.f, 1.f / 2.f),  glm::vec3(0.f, 0.f, 1.f)}, // 0 -> 0
		{ glm::vec3(length,	0.f,	width),   glm::vec2(0.f, 1.f / 2.f),        glm::vec3(0.f, 0.f, 1.f)}, // 1 -> 1
		{ glm::vec3(length,	height,	width),   glm::vec2(0.f, 1.f),			    glm::vec3(0.f, 0.f, 1.f)}, // 2 -> 2
		{ glm::vec3(0.f,	height,	width),	  glm::vec2(1.f / 3.f, 1.f),        glm::vec3(0.f, 0.f, 1.f)}, // 3 -> 3

		// right (1 5 6 2)
		{ glm::vec3(length,	0.f,	width),   glm::vec2(2.f / 3.f, 0.f),	    glm::vec3(1.f, 0.f, 0.f)}, // 1 -> 4
		{ glm::vec3(length,	0.f,	0.f),	  glm::vec2(1.f / 3.f, 0.f),	    glm::vec3(1.f, 0.f, 0.f)}, // 5 -> 5
		{ glm::vec3(length,	height,	0.f),     glm::vec2(1.f / 3.f, 1.f / 2.f),  glm::vec3(1.f, 0.f, 0.f)}, // 6 -> 6
		{ glm::vec3(length,	height,	width),   glm::vec2(2.f / 3.f, 1.f / 2.f),  glm::vec3(1.f, 0.f, 0.f)}, // 2 -> 7

		// back (5 4 7 6)
		{ glm::vec3(length,	0.f,	0.f),     glm::vec2(2.f / 3.f, 1.f / 2.f),  glm::vec3(0.f, 0.f, -1.f)}, // 5 -> 8
		{ glm::vec3(0.f,	0.f,	0.f),	  glm::vec2(1.f / 3.f, 1.f / 2.f),  glm::vec3(0.f, 0.f, -1.f)}, // 4 -> 9
		{ glm::vec3(0.f,	height,	0.f),     glm::vec2(1.f / 3.f, 1.f),	    glm::vec3(0.f, 0.f, -1.f)}, // 7 -> 10
		{ glm::vec3(length,	height,	0.f),     glm::vec2(2.f / 3.f, 1.f),	    glm::vec3(0.f, 0.f, -1.f)}, // 6 -> 11

		// left (4 0 3 7)
		{ glm::vec3(0.f,	0.f,	0.f),     glm::vec2(1.f / 3.f, 0.f),	    glm::vec3(-1.f, 0.f, 0.f)}, // 4 -> 12
		{ glm::vec3(0.f,	0.f,	width),	  glm::vec2(0.f, 0.f),			    glm::vec3(-1.f, 0.f, 0.f)}, // 0 -> 13
		{ glm::vec3(0.f,	height,	width),   glm::vec2(0.f, 1.f / 2.f),	    glm::vec3(-1.f, 0.f, 0.f)}, // 3 -> 14
		{ glm::vec3(0.f,	height,	0.f),     glm::vec2(1.f / 3.f, 1.f / 2.f),  glm::vec3(-1.f, 0.f, 0.f)}, // 7 -> 15

		// top (3 2 6 7)
		{ glm::vec3(0.f,	height,	width),   glm::vec2(1.f, 1.f / 2.f),		glm::vec3(0.f, 1.f, 0.f)},  // 3 -> 16
		{ glm::vec3(length,	height,	width),	  glm::vec2(2.f / 3.f, 1.f / 2.f),	glm::vec3(0.f, 1.f, 0.f)},  // 2 -> 17
		{ glm::vec3(length,	height,	0.f),     glm::vec2(2.f / 3.f, 1.f),		glm::vec3(0.f, 1.f, 0.f)},  // 6 -> 18
		{ glm::vec3(0.f,	height,	0.f),     glm::vec2(1.f, 1.f),				glm::vec3(0.f, 1.f, 0.f)},  // 7 -> 19

		// bottom (4 5 1 0)
		{ glm::vec3(0.f,	0.f,	0.f),     glm::vec2(2.f / 3.f, 1.f / 2.f),  glm::vec3(0.f, -1.f, 0.f)}, // 4 -> 20
		{ glm::vec3(length,	0.f,	0.f),	  glm::vec2(1.f, 1.f / 2.f),		glm::vec3(0.f, -1.f, 0.f)}, // 5 -> 21
		{ glm::vec3(length,	0.f,	width),   glm::vec2(1.f, 0.f),				glm::vec3(0.f, -1.f, 0.f)}, // 1 -> 22
		{ glm::vec3(0.f,	0.f,    width),   glm::vec2(2.f / 3.f, 0.f),		glm::vec3(0.f, -1.f, 0.f)}, // 0 -> 23

	};
	std::vector<unsigned int> indices =
	{
		// front
		0, 1, 2,
		2, 3, 0,

		// right
		4, 5, 6,
		6, 7, 4,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,

		// top
		16, 17, 18,
		18, 19, 16,

		// bottom
		20, 21, 22,
		22, 23, 20
	};

	m_Mesh.Set(vertices, indices);

	m_Color = glm::vec4(1.0f);
}

Cuboid::~Cuboid()
{
}
