/*
 * Shader.h
 *
 *      Author: leo
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

class Shader
{
	public:
		~Shader();

		GLuint id;

		static Shader *load (const char *name);

	private:
		Shader (GLuint id);
		static GLuint compile (const char *name, GLenum shader_type);
		static GLuint link (GLuint vertex_obj, GLuint fragment_obj);
};



#endif /* SHADER_H_ */
