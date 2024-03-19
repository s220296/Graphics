#pragma once

#include "Application.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "OBJMesh.h"
#include "Scene.h"
#include "Instance.h"
#include "RenderTarget.h"
#include <glm/mat4x4.hpp>

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	bool ObjLoader(aie::OBJMesh& __objMesh, glm::mat4& _transform, 
		const char* _filepath, std::string _filename, 
		bool _flipTextures, float _scale, glm::vec3 _position);

protected:
	Camera camera;

	aie::Texture m_gridTexture;

	aie::ShaderProgram m_shader;
	aie::ShaderProgram m_texturedShader;
	// simple quad mesh
	Mesh m_quadMesh;
	Mesh m_screenQuad;
	glm::mat4 m_quadTransform;

	aie::RenderTarget m_renderTarget;

	aie::ShaderProgram m_simplePhong;
	aie::ShaderProgram m_classicPhong;
	aie::ShaderProgram m_texturedPhong;
	aie::ShaderProgram m_normalMapPhong;

	aie::ShaderProgram m_boundTexture;
	aie::ShaderProgram m_postProcess;

	aie::OBJMesh m_dragonMesh;
	glm::mat4 m_dragTransform;

	aie::Texture m_spearTexture;
	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	struct OldLight {
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	OldLight m_light;
	glm::vec3 m_ambientLight;

	Scene* m_scene;
};