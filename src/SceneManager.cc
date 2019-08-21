/*
 * SceneManager.cc
 *
 *      Author: leo
 */

#include "Camera.h"
#include "Mesh.h"
#include "Pinball.h"
#include "Flipper.h"
#include "SceneManager.h"
#include "SceneLoader.h"
#include "SceneNode.h"
#include "Camera.h"

SceneManager::SceneManager() {
	// TODO Auto-generated constructor stub
	camera_ = new Camera;
	ball_node_ = NULL;
}

SceneManager::~SceneManager() {
	// TODO Auto-generated destructor stub
}

void SceneManager::init_physics_world()
{
	collision_config_ = new btDefaultCollisionConfiguration();
	dispatcher_ = new btCollisionDispatcher(collision_config_);
	overlapping_pair_cache_ = new btDbvtBroadphase();
	solver_ = new btSequentialImpulseConstraintSolver();
	dynamics_world_ = new btDiscreteDynamicsWorld(dispatcher_, overlapping_pair_cache_, solver_, collision_config_);
	dynamics_world_->setGravity(btVector3(0, 0, -980.0f));

	//table plane
	float table_angle = 6.5f * (M_PI/180.0f);
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 0, 1), 1);
	btQuaternion plane_quat;
	plane_quat.setRotation(btVector3(1,0,0), table_angle);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(plane_quat, btVector3(0, 0, -1)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI (0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	groundRigidBody->setRestitution(1.0);
    dynamics_world_->addRigidBody(groundRigidBody);

}

void SceneManager::scene_init()
{
	SceneNode *table_node = get_scene_node("table_surface");
	if (table_node) {
		float table_angle = 6.5f * (M_PI/180.0f);
		table_node->set_transform(glm::rotate(glm::mat4(1), table_angle, glm::vec3(1,0,0)));
	}

	ball_node_ = scene_root_->find_node_by_name("pinball");
}

SceneNode *SceneManager::get_root_scene_node()
{
	return scene_root_;
}

SceneNode *SceneManager::get_scene_node(const char *name)
{
	return scene_root_->find_node_by_name(name);
}

Camera *SceneManager::get_camera()
{
	return camera_;
}

void SceneManager::update(float dt)
{
	dynamics_world_->stepSimulation(dt);
	SceneNode *cube = get_scene_node("Cube");
	if (cube) {
		float table_angle = 1.0f * (M_PI/180.0f);
		cube->set_transform(glm::rotate(cube->get_transform(), table_angle, glm::vec3(0,0,1)));
	}

}

void SceneManager::update_scene_graph()
{
	scene_root_->update_world_matrix();

	if (ball_node_) {
		glm::mat4 transform = ball_node_->get_world_transform();
		camera_->set_look_at(transform[3]);
	} else
		camera_->set_look_at(glm::vec3(0,0,0));
/*
	SceneNode *cube = get_scene_node("Cube");
	if (cube) {
		glm::mat4 xform = cube->get_world_transform();
		camera_->set_look_at(xform[3]);
	}
*/
}

void SceneManager::set_root_scene_node(SceneNode *node)
{
	scene_root_ = node;
}

void SceneManager::add_rigid_body(btRigidBody *rigid_body)
{
	dynamics_world_->addRigidBody(rigid_body);
}

void SceneManager::load_scene(const char *name)
{
	init_physics_world();

	SceneLoader loader;
	loader.load(name, this);
}

