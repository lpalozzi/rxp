/*
 * Mesh.h
 *
 *      Author: leo
 */

#ifndef SRC_MESH_H_
#define SRC_MESH_H_

#include <vector>
#include <SDL2/SDL_opengles2.h>
#include <glm/vec4.hpp>

#include "Shader.h"

typedef struct {
	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_ibo_cnt;
	glm::vec4 diffuse_color_;
} MeshPrimitive;

class Mesh
{
public:
	//void setMaterial(Material *material);

	std::vector<MeshPrimitive> mesh_primitives_;
};

#endif /* SRC_MESH_H_ */
