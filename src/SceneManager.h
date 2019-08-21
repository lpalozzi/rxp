/*
 * SceneManager.h
 *
 *      Author: leo
 */

#ifndef SRC_SCENEMANAGER_H_
#define SRC_SCENEMANAGER_H_

#include <string>
#include <bullet/btBulletDynamicsCommon.h>

class SceneNode;
class Camera;

class SceneManager {
public:
	SceneManager();
	virtual ~SceneManager();

	void load_scene(const char *name);
	void scene_init();

	SceneNode *get_root_scene_node();
	SceneNode *get_scene_node(const char *name);
	Camera *get_camera();

	void set_root_scene_node(SceneNode *node);
	void add_rigid_body(btRigidBody *rigid_body);

	void update(float dt);
	void update_scene_graph();

protected:

	void init_physics_world();

	Camera *camera_;
	SceneNode *scene_root_;

	btDefaultCollisionConfiguration *collision_config_;
	btCollisionDispatcher *dispatcher_;
	btBroadphaseInterface *overlapping_pair_cache_;
	btSequentialImpulseConstraintSolver *solver_;
	btDiscreteDynamicsWorld *dynamics_world_;

	SceneNode *ball_node_;
};

#endif /* SRC_SCENEMANAGER_H_ */
