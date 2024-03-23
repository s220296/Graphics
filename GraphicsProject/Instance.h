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
	Instance(glm::vec3 _position,
		glm::vec3 _eulerAngles,
		glm::vec3 _scale,
		aie::OBJMesh* mesh,
		aie::ShaderProgram* shader,
		bool isUntextured = false);
	~Instance() {}

	void Draw(Scene* scene);
	void DrawDepth(Scene* scene);

	static glm::mat4 MakeTransform(
		glm::vec3 _position,
		glm::vec3 _eulerAngles,
		glm::vec3 _scale);

protected:
	bool LoadDepthShader();

protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	Mesh* m_simpleMesh;
	aie::ShaderProgram* m_shader;
	aie::ShaderProgram* m_depthShader;

	bool m_isUntextured;
};

