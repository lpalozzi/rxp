/*
 * Game.h
 *
 *      Author: leo
 */

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

#include <bullet/btBulletDynamicsCommon.h>

#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Pinball.h"
#include "Table.h"
#include "SceneManager.h"

class Game
{
public:

	int init();
	void loop();


protected:

	int init_sdl();
	int init_physics();

	void update(float dt);
	void render();

	SceneNode* load_gltf_scene(const char *name);

	//Camera camera_;
	Renderer renderer_;
	//SceneNode *scene_root_;
	Shader *shader_;



	SceneManager *scene_manager_;
	//Table *table_;
	//Pinball *pinball_;
	//btRigidBody* ball_rigid_body_;
};

#endif /* SRC_GAME_H_ */
