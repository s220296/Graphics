#pragma once

#include "Application.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "OBJMesh.h"
#include <glm/mat4x4.hpp>

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	Camera camera;

	aie::Texture m_gridTexture;

	aie::ShaderProgram m_shader;
	aie::ShaderProgram m_texturedShader;
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;

	aie::ShaderProgram m_simplePhong;

	aie::Texture m_spearTexture;
	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	struct Light {
		glm::vec3 direction;
		glm::vec4 color;
	};

	Light m_light;
};