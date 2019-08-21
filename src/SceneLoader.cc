/*
 * SceneImporter.cc
 *
 *      Author: leo
 */
#include <iostream>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <bullet/BulletCollision/btBulletCollisionCommon.h>

#include "Camera.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "SceneLoader.h"
#include "SceneManager.h"

#include "Pinball.h"
#include "Flipper.h"
#include "StaticMesh.h"
#include "PhysicalActor.h"

#define FILE_PATH_ROOT "assets/"
#define MAX_CHAR	64
#define MAX_PATH	256

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tiny_gltf.h"

SceneLoader::SceneLoader() {
	// TODO Auto-generated constructor stub

}

SceneLoader::~SceneLoader() {
	// TODO Auto-generated destructor stub
}

glm::mat4 build_transform_from_gltf_node (tinygltf::Node &node)
{
	glm::vec3 scale_v(1.0, 1.0, 1.0);
	glm::vec3 translation_v(0.0);
	glm::vec4 rotation_v(0,0,0,0);

	//scale = glm::scale(glm::mat4(), glm::vec3(node.scale[0], node.scale[1], node.scale[2]));
	if (!node.scale.empty())
		scale_v = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
	if (!node.translation.empty())
		translation_v = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
	if (!node.rotation.empty())
		rotation_v = glm::vec4((float)node.rotation[3], (float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2]);

	glm::mat4 scale = glm::scale(glm::mat4(), scale_v);
	glm::mat4 translation = glm::translate(glm::mat4(), translation_v);
	glm::quat qrot = glm::quat(rotation_v.w, rotation_v.x, rotation_v.y, rotation_v.z);
	return translation * glm::mat4_cast(qrot) * scale;
}

void load_gltf_camera (tinygltf::Model& model, Camera *camera)
{
	tinygltf::Scene scene = model.scenes[model.defaultScene];
	for (size_t i = 0; i < model.nodes.size(); i++) {
		if (model.nodes[i].name == "Camera") {
			tinygltf::Node &node = model.nodes[i];
			glm::vec3 camera_position(node.translation[0], node.translation[1], node.translation[2]);
			camera->set_position(camera_position);
			return;
		}
	}
}

Mesh *SceneLoader::load_mesh (const tinygltf::Mesh &gltfmesh)
{
	Mesh *mesh = new Mesh();
	for (size_t i=0; i<gltfmesh.primitives.size(); ++i) {
		const tinygltf::Primitive &primitive = gltfmesh.primitives[i];
		auto it_position = primitive.attributes.find("POSITION");
		const tinygltf::Accessor &accessor = model.accessors[it_position->second];
		const tinygltf::BufferView buffer_view = model.bufferViews[accessor.bufferView];
		const tinygltf::Buffer &buffer = model.buffers[buffer_view.buffer];

		MeshPrimitive mesh_primitive;

		//color
		tinygltf::Material material = model.materials[primitive.material];
		auto it = material.values.find("baseColorFactor");
		mesh_primitive.diffuse_color_ = glm::vec4(it->second.number_array[0], it->second.number_array[1],
				it->second.number_array[2], it->second.number_array[3]);

		//positions
		glGenBuffers(1, &mesh_primitive.m_vbo);
		glBindBuffer(buffer_view.target, mesh_primitive.m_vbo);
		glBufferData(buffer_view.target, buffer_view.byteLength,
				&buffer.data.at(0) + buffer_view.byteOffset, GL_STATIC_DRAW);

		//indices
		const tinygltf::Accessor &accessor_indices = model.accessors[primitive.indices];
		const tinygltf::BufferView buffer_view_i = model.bufferViews[accessor_indices.bufferView];
		const tinygltf::Buffer &buffer_i = model.buffers[buffer_view_i.buffer];
		glGenBuffers(1, &mesh_primitive.m_ibo);
		glBindBuffer(buffer_view_i.target, mesh_primitive.m_ibo);
		glBufferData(buffer_view_i.target, buffer_view_i.byteLength,
				&buffer_i.data.at(0) + buffer_view_i.byteOffset, GL_STATIC_DRAW);
		//std::cout << "buff_i: " << buffer_view_i.byteLength << std::endl;
		mesh_primitive.m_ibo_cnt = accessor_indices.count;

		mesh->mesh_primitives_.push_back(mesh_primitive);
	}
	return mesh;
}
#if 0
glm::mat4 aiMatrixToGLM(aiMatrix4x4& from)
{
	glm::mat4 to;
	to[0][0] = from.a1; to[1][0] = from.a2;
	to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2;
	to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2;
	to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2;
	to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}
#endif
#if 0
glm::mat4 build_transform_from_gltf_matrix(tinygltf::Node& node)
{
	glm::mat4 to;
	to[0][0] = node.matrix[0]; to[1][0] = node.matrix[1];
	to[2][0] = node.matrix[2]; to[3][0] = node.matrix[3];
	to[0][1] = node.matrix[4]; to[1][1] = node.matrix[5];
	to[2][1] = node.matrix[6]; to[3][1] = node.matrix[7];
	to[0][2] = node.matrix[8]; to[1][2] = node.matrix[9];
	to[2][2] = node.matrix[10]; to[3][2] = node.matrix[11];
	to[0][3] = node.matrix[12]; to[1][3] = node.matrix[13];
	to[2][3] = node.matrix[14]; to[3][3] = node.matrix[15];
	return to;
}
#endif

glm::mat4 build_transform_from_gltf_matrixx(tinygltf::Node& node)
{
	glm::mat4 to;
	to[0][0] = node.matrix[0]; to[0][1] = node.matrix[1];
	to[0][2] = node.matrix[2]; to[0][3] = node.matrix[3];
	to[1][0] = node.matrix[4]; to[1][1] = node.matrix[5];
	to[1][2] = node.matrix[6]; to[1][3] = node.matrix[7];
	to[2][0] = node.matrix[8]; to[2][1] = node.matrix[9];
	to[2][2] = node.matrix[10]; to[2][3] = node.matrix[11];
	to[3][0] = node.matrix[12]; to[3][1] = node.matrix[13];
	to[3][2] = node.matrix[14]; to[3][3] = node.matrix[15];
	return to;
}

void glm_to_bt_transform(glm::mat4& glm_transform, btTransform& bt_transform)
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(glm_transform, scale, rotation, translation, skew, perspective);
	rotation=glm::conjugate(rotation); //fix?
	bt_transform.setOrigin(btVector3(translation.x, translation.y, translation.z));
	bt_transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
}


btConvexHullShape *SceneLoader::create_btConvexHullShape(const tinygltf::Mesh &gltfmesh)
{
	const tinygltf::Primitive &primitive = gltfmesh.primitives[0];
	auto it_position = primitive.attributes.find("POSITION");
	const tinygltf::Accessor &accessor = model.accessors[it_position->second];
	const tinygltf::BufferView buffer_view = model.bufferViews[accessor.bufferView];
	const tinygltf::Buffer &buffer = model.buffers[buffer_view.buffer];

	int num_points = buffer_view.byteLength/12;
	btConvexHullShape *shape = new btConvexHullShape(
			(btScalar*)(&buffer.data.at(0) + buffer_view.byteOffset), num_points);
	//shape->recalcLocalAabb();
	return shape;
}

#if 0
btCollisionShape *SceneLoader::create_traingle_mesh_shape(const tinygltf::Mesh &gltfmesh)
{
	const tinygltf::Primitive &primitive = gltfmesh.primitives[0];
	auto it_position = primitive.attributes.find("POSITION");
	const tinygltf::Accessor &accessor = model.accessors[it_position->second];
	const tinygltf::BufferView buffer_view = model.bufferViews[accessor.bufferView];
	const tinygltf::Buffer &buffer = model.buffers[buffer_view.buffer];

	const tinygltf::Accessor &accessor_indices = model.accessors[primitive.indices];
	const tinygltf::BufferView buffer_view_i = model.bufferViews[accessor_indices.bufferView];
	const tinygltf::Buffer &buffer_i = model.buffers[buffer_view_i.buffer];

	btTriangleMesh *tri_mesh = new btTriangleMesh();
	//int num_tris = buffer_view.byteLength/sizeof(btVector3)/3;
	struct Vertex { float x; float y; float z; };
	Vertex *vertices = (Vertex *)(&buffer.data.at(0) + buffer_view.byteOffset);
	unsigned short *indices = (unsigned short *)(&buffer_i.data.at(0) + buffer_view_i.byteOffset);
	for (int i=0; i<accessor_indices.count; i+=3) {
		std::cout << i << ":  "<<indices[i] << " " <<indices[i+1]<<" "<<indices[i+2]<<std::endl;
		Vertex v0 = vertices[indices[i]];
		Vertex v1 = vertices[indices[i+1]];
		Vertex v2 = vertices[indices[i+2]];
		btVector3 btv0(v0.x, v0.y, v0.z);
		btVector3 btv1(v1.x, v1.y, v1.z);
		btVector3 btv2(v2.x, v2.y, v2.z);
		tri_mesh->addTriangle(btv0, btv1, btv2);
	}

	btCollisionShape *shape = new btBvhTriangleMeshShape(tri_mesh, false);
	return shape;
}
#endif

Pinball *SceneLoader::create_Pinball(SceneNode *scene_node, tinygltf::Node& node)
{
#if 0
	const tinygltf::Mesh &gltfmesh = model.meshes[node.mesh];
	Mesh *mesh = load_mesh(gltfmesh);
	Pinball *pinball = new Pinball(mesh);
	pinball->set_parent_scene_node(scene_node);
	pinball->setup_rigid_body();
	scene_manager_->add_rigid_body(pinball->rigid_body());
	return pinball;
#endif
	return 0;
}

btCollisionShape *SceneLoader::build_collisionShape(tinygltf::Node& node)
{

	return NULL;
}

Actor *SceneLoader::create_PhysicalActor(SceneNode *scene_node, tinygltf::Node& node)
{
	tinygltf::Value value;
	tinygltf::Value shape;

	PhysicalActor *actor = new PhysicalActor();
    actor->set_parent_scene_node(scene_node);

	btCompoundShape *compound_shape = new btCompoundShape();

	for (size_t i = 0; i < node.children.size(); i++) {
		tinygltf::Node& child = model.nodes[node.children[i]];
		value = child.extensions.at("BLENDER_physics");
		if (!value.IsObject()) continue;

		float mass = value.Get("mass").Get<double>();
		int staticc = value.Get("static").Get<bool>();
		shape = value.Get("collisionShapes").Get(0);
		std::string shape_type = shape.Get("shapeType").Get<std::string>();
		std::string primary_axis = shape.Get("primaryAxis").Get<std::string>();
		tinygltf::Value array = shape.Get("boundingBox");
		btVector3 bounding_box = btVector3(array.Get(0).Get<double>(),
				array.Get(1).Get<double>(), array.Get(2).Get<double>());
		bounding_box *= .5f;
		btCollisionShape *btshape;
		if (shape_type == "CYLINDER") {
			btshape = new btCylinderShape(bounding_box);
		}
		else { // BOX
			btshape = new btBoxShape(bounding_box);
		}


		btTransform bt_transform;
		glm::mat4 glm_transform = build_transform_from_gltf_node(child);
		glm_to_bt_transform(glm_transform, bt_transform);
		std::cout << "shape margin: " << btshape->getMargin() << std::endl;
		btshape->setMargin(0.0);
		compound_shape->addChildShape(bt_transform, btshape);
		std::cout << "shape margin: " << btshape->getMargin() << std::endl;
	}

    btScalar mass = 0;
    btVector3 inertia(0, 0, 0);
    compound_shape->calculateLocalInertia(mass, inertia);
    std::cout << "compound_shape margin: " << compound_shape->getMargin() << std::endl;
    compound_shape->setMargin(0.0f);
    std::cout << "compound_shape margin: " << compound_shape->getMargin() << std::endl;
    btRigidBody::btRigidBodyConstructionInfo constr_info(mass, actor, compound_shape, inertia);
    btRigidBody *rigid_body = new btRigidBody(constr_info);
    actor->set_rigid_body(rigid_body);
    scene_manager_->add_rigid_body(rigid_body);
	return actor;
}

Flipper *SceneLoader::create_Flipper(SceneNode *scene_node, tinygltf::Node& node)
{
	const tinygltf::Mesh &gltfmesh = model.meshes[node.mesh];
	Mesh *mesh = load_mesh(gltfmesh);
	Flipper *flipper = new Flipper(mesh);
	flipper->set_parent_scene_node(scene_node);
	//btConvexHullShape *collision_shape = create_btConvexHullShape(gltfmesh);
	//btBoxShape *collision_shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
	//collision_shape->setMargin(0.01);
	//collision_shape->setLocalScaling(btVector3(.8, .8, .8));
	//btCollisionShape *collision_shape = create_traingle_mesh_shape(gltfmesh);
	//collision_shape->setMargin(1.4f);
	//collision_shape->recalcLocalAabb ();

	//flipper->setup_rigid_body(collision_shape);
	//scene_manager_->add_rigid_body(flipper->rigid_body());
	return flipper;
}

StaticMeshNode *SceneLoader::create_StaticMeshNode(SceneNode *parent_scene_node, tinygltf::Node& node)
{
	Mesh *mesh = load_mesh(model.meshes[node.mesh]);
	StaticMeshNode *static_mesh = new StaticMeshNode(mesh);
	return static_mesh;
}

SceneNode *SceneLoader::create_SceneNode(const char *actor_name, char *actor_type, SceneNode *parent_scene_node, tinygltf::Node& node)
{
	SceneNode *scene_node = NULL;
	//if (0 == strcmp(actor_type, "Pinball"))
	//	actor = create_Pinball(parent_scene_node, node);
	//else if (0 == strcmp(actor_type, "Flipper"))
	//	actor = create_Flipper(parent_scene_node, node);
	if (0 == strcmp(actor_type, "StaticMeshNode"))
		scene_node = create_StaticMeshNode(parent_scene_node, node);
//	else if (0 == strcmp(actor_type, "PhysicalMeshNode"))
//		scene_node = create_PhysicalActor(parent_scene_node, node);
	else {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SceneLoader: Actor type '%s' unknown", actor_type);
		assert(0);
	}
	return scene_node;
}


SceneNode* SceneLoader::load_node (SceneNode *parent, tinygltf::Node &node, std::string path)
{
	char actor_name[32];
	char actor_type[32];

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading node %s.", path.c_str());

	SceneNode *scene_node = NULL;
	int nscanned = sscanf(node.name.c_str(), "%32[a-z-A-Z-0-9_].%[a-z-A-Z_]", actor_name, actor_type);
	if (nscanned == 2) {
		printf("name: %s, type: %s\n", actor_name, actor_type);
		scene_node = create_SceneNode(actor_name, actor_type, scene_node, node);
		scene_node->set_name (std::string(actor_name));
	} else if (node.mesh >= 0) {
		scene_node = create_SceneNode(node.name.c_str(), "StaticMeshNode", scene_node, node);
		scene_node->set_name (std::string(actor_name));
	} else if (node.camera >= 0) {
		scene_node = new SceneNode;
		Camera *camera = scene_manager_->get_camera();
		load_gltf_camera(model, camera);
	} else {
		scene_node = new SceneNode;
		scene_node->set_name (node.name);
	}

	scene_node->set_transform (build_transform_from_gltf_node(node));
	scene_node->set_parent(parent);
	scene_node->update_world_matrix();

	//already processed PhysicalActor child nodes
	if (0 == strcmp("PhysicalActor", actor_type))
		return scene_node;

	for (size_t i = 0; i < node.children.size(); i++) {
		tinygltf::Node& child = model.nodes[node.children[i]];
		SceneNode *snode = load_node(scene_node, child, path + "/" + child.name);
	}

	return scene_node;
}

void SceneLoader::load(const char *name, SceneManager *scene_manager)
{
	char path[MAX_CHAR];
	snprintf(path, MAX_CHAR, FILE_PATH_ROOT"%s", name);

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loading scene %s.", path);

	std::string error;
	std::string warning;

	if (!loader.LoadASCIIFromFile(&model, &error, &warning, path)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to load scene: %s", error.c_str());
		scene_manager->set_root_scene_node(NULL);
		return;
	}

	tinygltf::Scene scene = model.scenes[model.defaultScene];
	scene_manager_ = scene_manager;
	SceneNode *scene_root = new SceneNode;
	std::string mesh_path = "/";
	scene_root->set_name("scene_root");
	scene_root->update_world_matrix();
	for (size_t i = 0; i < scene.nodes.size(); i++) {
		tinygltf::Node &child = model.nodes[scene.nodes[i]];
		SceneNode *node = load_node(scene_root, child, mesh_path + child.name);
	}

	scene_manager_->set_root_scene_node(scene_root);
}
