/*
 * Camera.h
 *
 *      Author: leo
 */

#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera() {}

	void set_position (glm::vec3 position) { position_ = position; }

	glm::mat4 projection() const {
	    return glm::perspective(glm::radians(45.0f), 640.0f/480.0f, 0.01f, 400.0f);
	}

	glm::mat4 view() const {
	    return glm::lookAt(position_, look_at_, glm::vec3(0,0,1));
	}

	void set_look_at (glm::vec3 look_at) { look_at_ = look_at; }

protected:
	glm::vec3 position_;
	glm::vec3 look_at_;
};



#endif /* SRC_CAMERA_H_ */
