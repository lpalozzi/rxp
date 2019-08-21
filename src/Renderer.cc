/*
 * Renderer.cc
 *
 *      Author: leo
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "Camera.h"

#include "Actor.h"
#include "Shader.h"
#include "Renderer.h"
#include "SceneNode.h"
#include "StaticMeshNode.h"

void Renderer::init()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::render(SceneNode *node, Camera *camera, Shader *shader)
{
	glClearColor(0, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderNode(node, camera, shader);

	//objx.render_printf("%0.3f", gElapsedTime);
	SDL_GL_SwapWindow(SDL_GL_GetCurrentWindow());
}

void Renderer::renderNode (SceneNode *node, Camera *camera, Shader *shader)
{

#if 0
	Actor *object = node->get_object();
	if (object) {
		Mesh *mesh = object->get_mesh();
		if (mesh) {
#endif
	Mesh *mesh = node->get_mesh();
	if (mesh) {

		glUseProgram(shader->id);

		glm::mat4 Projection = camera->projection();
		glm::mat4 View = camera->view();

		GLint view_handle = glGetUniformLocation(shader->id, "uView");
		glUniformMatrix4fv(view_handle, 1, GL_FALSE, &View[0][0]);
		GLint proj_handle = glGetUniformLocation(shader->id, "uProjection");
		glUniformMatrix4fv(proj_handle, 1, GL_FALSE, &Projection[0][0]);
		GLint model_handle = glGetUniformLocation(shader->id, "uModel");
		glm::mat4 transform = node->get_world_transform();
		glUniformMatrix4fv(model_handle, 1, GL_FALSE, &transform[0][0]);

		//material->bind();
		for (size_t i=0; i < mesh->mesh_primitives_.size(); ++i) {

				GLint colorAttrib = glGetUniformLocation(shader->id, "vColor");
				glm::vec4 color = mesh->mesh_primitives_[i].diffuse_color_;
				glUniform4f(colorAttrib, color.r, color.g, color.b, color.a);

				GLint pos_handle = glGetAttribLocation(shader->id, "vPosition");
				glBindBuffer(GL_ARRAY_BUFFER, mesh->mesh_primitives_[i].m_vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mesh_primitives_[i].m_ibo);

				glEnableVertexAttribArray(pos_handle);
				glVertexAttribPointer(pos_handle, 3, GL_FLOAT, GL_FALSE, 0, NULL);

				glDrawElements(GL_TRIANGLES, mesh->mesh_primitives_[i].m_ibo_cnt, GL_UNSIGNED_SHORT, NULL);

				glDisableVertexAttribArray(pos_handle);
		}

	}

	for (auto child : *node->get_children()) {
		renderNode(child, camera, shader);
	}
}




