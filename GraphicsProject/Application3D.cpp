#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {
	camera = Camera(glm::vec3(0, 1, 0));
}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = camera.GetViewMatrix();
	m_projectionMatrix = camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight());

	// load vertex shader from file
	m_shader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	// load fragment shader from file
	m_shader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");
	
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/textured.frag");

	m_simplePhong.loadShader(aie::eShaderStage::VERTEX, "./shaders/simplePhong.vert");
	m_simplePhong.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simplePhong.frag");

	m_classicPhong.loadShader(aie::eShaderStage::VERTEX, "./shaders/classicPhong.vert");
	m_classicPhong.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/classicPhong.frag");
	
	if (m_shader.link() == false)
	{
		printf("Shader Error: %s \n", m_shader.getLastError());
		return false;
	}
	
	if (m_texturedShader.link() == false)
	{
		printf("Textured Shader Error: %s \n", m_texturedShader.getLastError());
		return false;
	}

	if (m_simplePhong.link() == false)
	{
		printf("Phong Shader Error: %s \n", m_simplePhong.getLastError());
		return false;
	}

	if (m_classicPhong.link() == false)
	{
		printf("Phong Shader Error: %s \n", m_classicPhong.getLastError());
		return false;
	}

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load texture!\n");
		return false;
	}

	m_light.diffuse = { 1.f, 1.f, 0.3f };
	m_light.specular = { 1.f, 1.f, 0.3f };
	m_light.direction = { 0, -1, 0 };

	m_ambientLight = { 0.25f, 0.25f, 0.25f };

	m_quadMesh.initialiseQuad();

	//make the quad 10 units wide
	m_quadTransform = {
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1 };

	if (m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	if (m_spearTexture.load("./soulspear/soulspear_diffuse.tga") == false)
	{
		printf("Soulspear Texture Error!\n");
		return false;
	}

	m_spearTransform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// demonstrate a few shapes
	Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(0, 0.5f, 1, 0.25f));
	Gizmos::addSphere(vec3(5, 0, 5), 1, 8, 8, vec4(1, 0, 0, 0.5f));
	Gizmos::addRing(vec3(5, 0, -5), 1, 1.5f, 8, vec4(0, 1, 0, 1));
	Gizmos::addDisk(vec3(-5, 0, 5), 1, 16, vec4(1, 1, 0, 1));
	Gizmos::addArc(vec3(-5, 0, -5), 0, 2, 1, 8, vec4(1, 0, 1, 1));

	mat4 t = glm::rotate(mat4(1), time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);
	Gizmos::addCylinderFilled(vec3(0), 0.5f, 1, 5, vec4(0, 1, 1, 1), &t);

	// demonstrate 2D gizmos
	Gizmos::add2DAABB(glm::vec2(getWindowWidth() / 2, 100),
					  glm::vec2(getWindowWidth() / 2 * (fmod(getTime(), 3.f) / 3), 20),
					  vec4(0, 1, 1, 1));

	camera.Update(deltaTime);

	m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_viewMatrix = camera.GetViewMatrix();

	auto pvm = m_projectionMatrix * m_viewMatrix * m_quadTransform;

	// bind shader
	m_classicPhong.bind();
	// bind transform
	m_classicPhong.bindUniform("ProjectionViewModel", pvm);

	// bind texture location
	m_classicPhong.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_quadTransform)));

	m_classicPhong.bindUniform("CameraPosition", camera.GetPosition());

	m_classicPhong.bindUniform("LightDirection", m_light.direction);
	m_classicPhong.bindUniform("ambientLight", m_ambientLight);
	m_classicPhong.bindUniform("diffuseLight", m_light.diffuse);
	m_classicPhong.bindUniform("specularLight", m_light.specular);

	// draw quad
	m_quadMesh.draw();

	auto pvm2 = m_projectionMatrix * m_viewMatrix * m_spearTransform;

	m_classicPhong.bind();
	m_classicPhong.bindUniform("ProjectionViewModel", pvm2);
	//m_texturedShader.bindUniform("diffuseTexture", 0);
	//m_spearTexture.bind(0);
	m_spearMesh.draw();
	
	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}