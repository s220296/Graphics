#pragma once
#include <glm/glm.hpp>

class Camera;
class Scene;
class Mesh;
struct Light;

namespace aie {
	class OBJMesh;
	class ShaderProgram;
}

class Instance
{
public:
	Instance(glm::mat4 transform, 
		aie::OBJMesh* mesh,
		aie::ShaderProgram* shader,
		bool isUntextured = false);
	~Instance() {}

	void Draw(Scene* scene);

protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	Mesh* m_simpleMesh;
	aie::ShaderProgram* m_shader;

	bool m_isUntextured;
};

