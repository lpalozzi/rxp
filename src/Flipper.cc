/*
 * Flipper.cc
 *
 *      Author: leo
 */
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/btBulletCollisionCommon.h>

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

#include "Flipper.h"

void Flipper::setup_rigid_body(btCollisionShape *collision_shape)
{
    btScalar mass = 0;
    btVector3 inertia(0, 0, 0);
    collision_shape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo constr_info(mass, this, collision_shape, inertia);
    rigid_body_ = new btRigidBody(constr_info);
    rigid_body_->setCollisionFlags(
    		rigid_body_->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
}

void Flipper::update(float dt)
{
}

void Flipper::getWorldTransform(btTransform& wt) const
{
	glm::mat4 transform = get_transform();
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(transform, scale, rotation, translation, skew, perspective);
	rotation=glm::conjugate(rotation); //fix?
	wt.setOrigin(btVector3(translation.x, translation.y, translation.z));
	wt.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
}

void Flipper::setWorldTransform(const btTransform& bt_world_transform)
{
	btVector3 bt_pos = bt_world_transform.getOrigin();
	btQuaternion bt_rot = bt_world_transform.getRotation();
	glm::quat qrot = glm::quat((float)bt_rot.w(), (float)bt_rot.x(), (float)bt_rot.y(), (float)bt_rot.z());
	set_transform(glm::translate(glm::mat4(1), glm::vec3(bt_pos.x(), bt_pos.y(), bt_pos.z())) * glm::mat4_cast(qrot));
}
