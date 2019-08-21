/*
 * Actor.h
 *
 *      Author: leo
 */

#ifndef ACTOR_H_
#define ACTOR_H_

#include <string>
#include <glm/mat4x4.hpp>

#include "SceneNode.h"

class Mesh;
class Actor;

class Actor {

public:
	virtual ~Actor() {};

	virtual void update(float dt) = 0;

	glm::mat4 get_transform() const { return parent_scene_node_->get_transform(); }
	glm::mat4 get_world_transform() const { return parent_scene_node_->get_world_transform(); }
	void set_transform(glm::mat4 transform) { parent_scene_node_->set_transform(transform); }

	void set_parent_scene_node(SceneNode *node) { parent_scene_node_ = node; }
	SceneNode *parent_scene_node() const { return parent_scene_node_; }

	virtual Mesh* get_mesh() = 0;

protected:

	SceneNode *parent_scene_node_;
};

#endif /* ACTOR_H_ */
