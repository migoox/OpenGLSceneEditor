#include "Cube.h"

Cube::Cube()
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
	float length = 1.f, width = 1.f, height = 1.f;

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
		0, 2, 1,
		0, 3, 2,

		// right
		4, 6, 5,
		4, 7, 6,

		// back
		8, 10, 9,
		8, 11, 10,

		// left
		12, 14, 13,
		12, 15, 14,

		// top
		16, 18, 17,
		16, 19, 18,

		// bottom
		20, 22, 21,
		20, 23, 22
	};

	m_Mesh.Set(vertices, indices);

	m_Color = glm::vec4(1.0f);
}

Cube::~Cube()
{
}
