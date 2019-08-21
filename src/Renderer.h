/*
 * Renderer.h
 *
 *  Created on: 2018-03-26
 *      Author: leo
 */

#ifndef SRC_RENDERER_H_
#define SRC_RENDERER_H_

class Shader;
class SceneNode;
class Camera;

class Renderer {

	public:
		Renderer() {};
		void init();
		void render(SceneNode *node, Camera *camera, Shader *shader);

	protected:
		void renderNode(SceneNode *node, Camera *camera, Shader *shader);
};



#endif /* SRC_RENDERER_H_ */
