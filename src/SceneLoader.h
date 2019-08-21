/*
 * SceneImporter.h
 *
 *      Author: leo
 */

#ifndef SRC_SCENEIMPORTER_H_
#define SRC_SCENEIMPORTER_H_

#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define STBI_ONLY_PNG
#include "tiny_gltf.h"

#include "StaticMeshNode.h"

class SceneNode;
class SceneManager;
class Pinball;
class Flipper;
class StaticMesh;
class PhysicalActor;
class btConvexHullShape;
class btCollisionShape;

class SceneLoader {
public:
	SceneLoader();
	virtual ~SceneLoader();

	void load (const char *name, SceneManager *scene_manager);
	SceneNode* load_node (SceneNode *parent, tinygltf::Node &node, std::string path);
	Mesh *load_mesh (const tinygltf::Mesh &gltfmesh);

	SceneNode *create_SceneNode(const char *object_name, char *object_type, SceneNode *scene_node, tinygltf::Node& node);
	Actor *create_PhysicalActor(SceneNode *scene_node, tinygltf::Node& node);
	Pinball *create_Pinball(SceneNode *scene_node, tinygltf::Node& node);
	Flipper *create_Flipper(SceneNode *scene_node, tinygltf::Node& node);
	StaticMeshNode *create_StaticMeshNode(SceneNode *scene_node, tinygltf::Node& node);

	btCollisionShape *build_collisionShape(tinygltf::Node& node);
	btConvexHullShape *create_btConvexHullShape(const tinygltf::Mesh &gltfmesh);
	btCollisionShape *create_traingle_mesh_shape(const tinygltf::Mesh &gltfmesh);

protected:
	tinygltf::TinyGLTF loader;
	tinygltf::Model model;
	SceneManager *scene_manager_;
};

#endif /* SRC_SCENEIMPORTER_H_ */
