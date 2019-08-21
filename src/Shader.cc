/*
 * Shader.cc
 *
 *      Author: leo
 */
#include <stdio.h>
#include "Shader.h"

#define FILE_PATH_ROOT "assets/"
#define MAX_CHAR	64
#define MAX_PATH	256
#define SHADER_ERROR_LEN 1024

/* load a file and return its contents in a buffer null terminated */
char *load_text(const char *name)
{
	char path[MAX_CHAR];
	snprintf(path, MAX_CHAR, FILE_PATH_ROOT"%s", name);

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading text file %s.", path);

	SDL_RWops *file = SDL_RWFromFile(path, "rb");
	if (!file)
		return NULL;
	SDL_RWseek(file, 0L, RW_SEEK_END);
	int filesize = SDL_RWtell(file);
	SDL_RWseek(file, 0L, RW_SEEK_SET);
	if (filesize < 0) {
		SDL_RWclose(file);
		return NULL;
	}
	char *buf = (char *) malloc(filesize+1);
	SDL_RWread(file, buf, filesize, 1);
	SDL_RWclose(file);
	buf[filesize] = '\0';
	return buf;
}

Shader::Shader(GLuint id) : id(id)
{
}

Shader::~Shader ()
{
	glDeleteProgram(this->id);
}

Shader * Shader::load(const char *name)
{
	Shader *shader = NULL;
	char path[MAX_CHAR];
	snprintf(path, MAX_CHAR, "%s.vert", name);
	GLuint vertex_obj = Shader::compile(path, GL_VERTEX_SHADER);
	snprintf(path, MAX_CHAR, "%s.frag", name);
	GLuint fragment_obj   = Shader::compile(path, GL_FRAGMENT_SHADER);
	GLuint id = Shader::link(vertex_obj, fragment_obj);
	if (id) shader = new Shader(id);
	return shader;
}

GLuint Shader::compile(const char *name, GLenum shader_type)
{
	char error_msg[SHADER_ERROR_LEN];
	int error_len;
	GLuint shader_ref;

	const GLchar *buf = load_text(name);
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Compiling shader %s.", name);
	shader_ref = glCreateShader(shader_type);
	if (shader_ref) {
		glShaderSource(shader_ref, 1, &buf, NULL);
		glCompileShader(shader_ref);
		GLint result;
		glGetShaderiv(shader_ref, GL_COMPILE_STATUS, &result);
		if (GL_FALSE == result) {
			glGetShaderInfoLog(shader_ref, SHADER_ERROR_LEN, &error_len, error_msg);
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to compile shader '%s': %s\n", name, error_msg);
			glDeleteShader(shader_ref);
		}
	}
	free((void*)buf);
	return shader_ref;
}

GLuint Shader::link(GLuint vertex_obj, GLuint fragment_obj)
{
	char error_msg[SHADER_ERROR_LEN];
	int error_len;

	SDL_assert(vertex_obj != 0);
	SDL_assert(fragment_obj != 0);

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Linking shader program.");

	GLuint program = glCreateProgram();
	if (program) {
		glAttachShader(program, vertex_obj);
		glAttachShader(program, fragment_obj);
		glLinkProgram(program);
		GLint result;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (GL_FALSE == result) {
			glGetShaderInfoLog(program, SHADER_ERROR_LEN, &error_len, error_msg);
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to link program. %s\n", error_msg);
			//glGetProgramInfoLog(program);
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}



