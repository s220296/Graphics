#include "Instance.h"
#include "Texture.h"
#include "OBJMesh.h"
#include "Scene.h"
#include "Camera.h"
#include "Shader.h"
#include <glm/ext.hpp>

Instance::Instance(glm::mat4 transform, aie::OBJMesh* mesh, 
	aie::ShaderProgram* shader, bool isUntextured)
	: m_transform(transform), m_mesh(mesh), m_shader(shader),
	m_isUntextured(isUntextured)
{
}

Instance::Instance(glm::vec3 _position, glm::vec3 _eulerAngles, 
	glm::vec3 _scale, aie::OBJMesh* mesh, 
	aie::ShaderProgram* shader, bool isUntextured)
	: m_mesh(mesh), m_shader(shader), m_isUntextured(isUntextured)
{
	m_transform = MakeTransform(_position, _eulerAngles, _scale);
}

void Instance::Draw(Scene* scene)
{
	m_shader->bind();

	auto pv = scene->GetCamera()->GetProjectionMatrix(
		scene->GetWindowSize().x, scene->GetWindowSize().y) * 
		scene->GetCamera()->GetViewMatrix();

	m_shader->bindUniform("ProjectionViewModel", pv * m_transform);
	m_shader->bindUniform("ModelMatrix", m_transform);
	m_shader->bindUniform("diffuseTexture", 0);
	m_shader->bindUniform("specularTexture", 0);
	m_shader->bindUniform("normalTexture", 0);

	m_shader->bindUniform("CameraPosition", scene->GetCamera()->GetPosition());

	m_shader->bindUniform("LightDirection", scene->GetGlobalLight().direction);
	m_shader->bindUniform("ambientLight", scene->GetAmbientLightColor());
	m_shader->bindUniform("diffuseLight", scene->GetAmbientLightColor());

	int numberOfLights = scene->GetNumberOfLights();
	m_shader->bindUniform("NumberOfLights", numberOfLights);
	m_shader->bindUniform("PointLightPositions", numberOfLights, scene->GetPointLightPositions());
	m_shader->bindUniform("PointLightColors", numberOfLights, scene->GetPointLightColors());

	m_mesh->draw();
}

glm::mat4 Instance::MakeTransform(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale)
{
	return glm::translate(glm::mat4(1), _position)
		* glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.z),
			glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.y),
			glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.x),
			glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), _scale);
}
