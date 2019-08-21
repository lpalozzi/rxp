/*
 * SceneNode.cc
 *
 *      Author: leo
 */

#include <iostream>
#include "SceneNode.h"
#include "Actor.h"

SceneNode::SceneNode() : parent_(NULL), actor_(NULL)
{
}

SceneNode::SceneNode(const SceneNode& rhs)
{
	parent_ = NULL;
	actor_ = rhs.actor_;
}

SceneNode::~SceneNode()
{
	std::cout << "~SceneNode " << this << std::endl;
	set_parent(NULL);
	while (!children_.empty()) {
		SceneNode *n = children_.front();
		children_.pop_front();
		std::cout << "removing child node " << n << std::endl;
		n->parent_ = NULL;
		delete n;
	}
}
void SceneNode::attach_actor(Actor *actor)
{
	actor_ = actor;
	actor_->set_parent_scene_node(this);
}

// assign this node to a new parent, remove its current parent
void SceneNode::set_parent (SceneNode *new_parent)
{
	if (parent_)
		parent_->children_.remove(this);
	if (new_parent)
		new_parent->children_.push_back(this);
	parent_ = new_parent;
}

void SceneNode::update_world_matrix ()
{
	if (parent_) {
		world_transform_ = parent_->world_transform_ * transform_;
	} else {
		world_transform_ = transform_;
	}

	for (auto child : children_)
		child->update_world_matrix();
}

SceneNode * SceneNode::find_node_by_name(const std::string& name) const
{
	return find_node(this, name);
}

SceneNode *SceneNode::find_node(const SceneNode *node, const std::string& name) const
{
	if (name_ == name)
		return const_cast<SceneNode*>(node);

	for (auto child : children_) {
		SceneNode *found_node = child->find_node(child, name);
		if (found_node)
			return found_node;
		}
	return NULL;
}


