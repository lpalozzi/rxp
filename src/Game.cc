/*
 * Game.cc
 *
 *      Author: leo
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <glm/gtc/quaternion.hpp>

#include "SceneNode.h"
#include "Game.h"

#include "Pinball.h"


void reshape_window(int width, int height)
{

}

#if 0
void checkGLError()
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "glGetError: %#x\n", error);
}

void setupOrthographic2D()
{
/*
	int width, height;
	SDL_GL_GetDrawableSize(SDL_GL_GetCurrentWindow(), &width, &height);
	float half_width = width * 0.5f;
	float half_height = height * 0.5f;

	gluMatrixMode(PROJECTION_MATRIX);
	gluLoadIdentity();
	gluSetOrthographic2d(-half_width, half_width, -half_height, half_height);
	gluTranslate(-half_width, -half_height, 0.0f);
*/

//	glDisable(GL_DEPTH_TEST);
//	glDepthMask(GL_FALSE);
}

#endif

void printGLConnectionInfo()
{
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GL_VENDOR: %s\n", glGetString(GL_VENDOR));
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GL_RENDERER: %s\n", glGetString(GL_RENDERER));
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	//SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GL_EXTENSIONS: %s\n", glGetString(GL_EXTENSIONS));

	SDL_RendererInfo info;
	SDL_GetRendererInfo(SDL_GetRenderer(SDL_GL_GetCurrentWindow()), &info);
	if (info.flags & SDL_RENDERER_TARGETTEXTURE)
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "renderer supports 'render to texture.'\n");
	if (info.flags & SDL_RENDERER_ACCELERATED)
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "renderer is hw accelerated.'\n");
	if (info.flags & SDL_RENDERER_SOFTWARE)
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "renderer is sw.'\n");

	int value;
	SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &value);
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_ACCELERATED_VISUAL: %d\n", value);
	SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value);
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_DOUBLEBUFFER: %d\n", value);

	if (SDL_GL_ExtensionSupported("GL_EXT_framebuffer_blit"))
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "GL_EXT_framebuffer_blit: 1\n");

}



int Game::init_sdl()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_Window *sdl_window = SDL_CreateWindow("Demo",
			//0, 0, 640, 480,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768,
		SDL_WINDOW_OPENGL
		//| SDL_WINDOW_FULLSCREEN_DESKTOP
		);


	SDL_GLContext gl_context = SDL_GL_CreateContext(sdl_window);
	//SDL_GL_SetSwapInterval(1);
	//printGLConnectionInfo();
	//IMG_Init(IMG_INIT_PNG);
	return 0;
}



int Game::init()
{
	init_sdl();
	renderer_.init();

	shader_ = Shader::load("color");

	scene_manager_ = new SceneManager;
	scene_manager_->load_scene("cube-blender_v2.80.gltf");

	scene_manager_->scene_init();
	//scene_root_ = load_gltf_scene("flipper.gltf");

	//SceneNode *ball_node = scene_manager_->get_scene_node("pinball");
	//Pinball *pinball_ = dynamic_cast<Pinball*>( ball_node->get_object());
	//pinball_->setup_rigid_body();

	//ball_->set_transform(glm::translate(ball_->get_transform(), glm::vec3(0,-0.25,-0.15)));
	//ModelNode *new_ball = new ModelNode(*ball_);
	//new_ball->set_transform(glm::translate(ball_->get_transform(), glm::vec3(0,0,0.135)));
	//new_ball->set_parent(scene_root_->find_node_by_name("table"));

	//ball_object_ = new BallObject;
	//ModelNode *ball_model = (ModelNode *)( scene_root_->find_node_by_name("ball"));
	//ball_object_->set_model(ball_model);
	//ball_object_->setup_rigid_body();

    //dynamics_world_->addRigidBody(ball_object_->rigid_body());
    //dynamics_world_->addRigidBody(pinball_->rigid_body());

	return 0;
}

void Game::update(float dt)
{
	scene_manager_->update(dt);
}

void Game::render()
{
	scene_manager_->update_scene_graph();
	renderer_.render(scene_manager_->get_root_scene_node(), scene_manager_->get_camera(), shader_);
}

void Game::loop()
{
	size_t prevTime = 0;
	int done = 0;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_MOUSEBUTTONDOWN) {
				done = 1;
			}
			else if (event.type == SDL_WINDOWEVENT) {
				switch (event.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					reshape_window(event.window.data1, event.window.data2);
				}
			}
		}

		size_t now = SDL_GetTicks();
		float dt = (now - prevTime)/1000.0f;
		prevTime = now;

		if (dt<=0) dt = .001f;

		update(dt);
		render();
		//std::cout << dt <<std::endl;

		SDL_GL_SwapWindow(SDL_GL_GetCurrentWindow());
		SDL_Delay(0);
	}

	SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());
	SDL_DestroyWindow(SDL_GL_GetCurrentWindow());
	//IMG_Quit();
	SDL_Quit();
}




