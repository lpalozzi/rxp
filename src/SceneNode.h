/*
 * SceneNode.h
 *
 *      Author: leo
 */

#ifndef SRC_SCENENODE_H_
#define SRC_SCENENODE_H_

#include <string>
#include <list>
#include <glm/mat4x4.hpp>


class Actor;
class Mesh;

class SceneNode
{
public:
	SceneNode();
	virtual ~SceneNode();

	SceneNode(const SceneNode &rhs);

	//void update(float dt);
	//virtual void render() {};

	glm::mat4              get_transform() const { return transform_; }
	glm::mat4              get_world_transform() const { return world_transform_; }
	std::list<SceneNode*> *get_children() { return &children_; }
	SceneNode *            find_node_by_name(const std::string& name) const;

	void set_transform (glm::mat4 transform) { transform_ = transform; }
	void set_parent (SceneNode *parent);
	void set_name (std::string name) { name_ = name; }


	void attach_actor (Actor *obj);
	Actor *get_object() { return actor_; }

	void update_world_matrix();

	virtual Mesh *get_mesh() const { return NULL; }

protected:
	SceneNode *find_node(const SceneNode *node, const std::string& name) const;

	std::string name_;
	glm::mat4 transform_;
	glm::mat4 world_transform_;
	SceneNode *parent_;
	Actor *actor_;
	std::list<SceneNode*> children_;
};


#endif /* SRC_SCENENODE_H_ */
