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

	// Ambient light
	Light light;
	light.color = { 1, 1, 1 };
	light.direction = { 1, -1, 1 };
	// this will create one scene to use
	m_scene = new Scene(&camera, glm::vec2(getWindowWidth(), getWindowHeight()), light);

	m_scene->GetPointLights().push_back(Light(glm::vec3(5, 3, 0), 
		glm::vec3(1, 0, 0), 50.f));
	m_scene->GetPointLights().push_back(Light(glm::vec3(-5, 3, 0), 
		glm::vec3(0, 1, 0), 50.f));

	// load vertex shader from file
	m_shader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/color.vert");
	// load fragment shader from file
	m_shader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/color.frag");
	
	m_boundTexture.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/textured.vert");
	m_boundTexture.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/textured.frag");

	m_simplePhong.loadShader(aie::eShaderStage::VERTEX, "./shaders/simplePhong.vert");
	m_simplePhong.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simplePhong.frag");

	m_classicPhong.loadShader(aie::eShaderStage::VERTEX, "./shaders/classicPhong.vert");
	m_classicPhong.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/classicPhong.frag");
	
	m_texturedPhong.loadShader(aie::eShaderStage::VERTEX, "./shaders/texturedPhong.vert");
	m_texturedPhong.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/texturedPhong.frag");

	m_normalMapPhong.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapPhong.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");

	m_postProcess.loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
	m_postProcess.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/post.frag");

	if (m_shader.link() == false)
	{
		printf("Shader Error: %s \n", m_shader.getLastError());
		return false;
	}
	
	if (m_boundTexture.link() == false)
	{
		printf("Textured Shader Error: %s \n", m_boundTexture.getLastError());
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

	if (m_texturedPhong.link() == false)
	{
		printf("Textured Phong Shader Error: %s \n", m_texturedPhong.getLastError());
		return false;
	}

	if (m_normalMapPhong.link() == false)
	{
		printf("normal map Phong Shader Error: %s \n", m_normalMapPhong.getLastError());
		return false;
	}

	if (m_postProcess.link() == false)
	{
		printf("post process Shader Error: %s \n", m_postProcess.getLastError());
		return false;
	}

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load texture!\n");
		return false;
	}

	m_light.diffuse = { 1.f, 1.f, 0.3f };
	m_light.specular = { 1.f, 1.f, 0.3f };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };

	m_light.direction = { 0, -1, 0 };

	if (m_renderTarget.initialise(1, getWindowWidth(), getWindowHeight()) == false)
	{
		printf("Render Target has an error!!\n");
		return false;
	}

#pragma region Quad Data

	m_quadMesh.initialiseQuad();
	m_screenQuad.initialiseFullscreenQuad();

	//make the quad 10 units wide
	m_quadTransform = {
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1 };

#pragma endregion
	// Load spear mesh
	if (m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh Error!\n");
		return false;
	}
	// Load spear texture
	if (m_spearTexture.load("./soulspear/soulspear_diffuse.tga") == false)
	{
		printf("Soulspear Texture Error!\n");
		return false;
	}
	// Create spear transform
	m_spearTransform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	// Load dragon mesh and create dragon transform
	ObjLoader(m_dragonMesh, m_dragTransform, "./dragon/dragon.obj", "Dragon", false, 0.2f, { 1.f,0,1.f });

	// Add instances after this line
	for (int i = 0; i < 10; i++)
	{
		m_scene->AddInstance(new Instance(glm::vec3(i * 2, 0, 0),
			glm::vec3(0, i * 30, 0), glm::vec3(1),
			&m_spearMesh, &m_normalMapPhong));
	}

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
	delete m_scene;
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


#pragma region ShapesAndShit
	/*
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
					  */
#pragma endregion

	camera.Update(deltaTime);

	//m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	m_scene->Update(deltaTime);
}

void Application3D::draw() {

	// Bind to the render target
	m_renderTarget.bind();

	// wipe the screen to the background colour
	clearScreen();

	m_scene->Draw();
	m_projectionMatrix = camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_viewMatrix = camera.GetViewMatrix();

	auto pv = m_projectionMatrix * m_viewMatrix;

	/*
	// update perspective in case window resized
	

#pragma region ClassicPhong

	// bind shader
	m_classicPhong.bind();
	// bind transform
	m_classicPhong.bindUniform("ProjectionViewModel", pv * m_quadTransform);

	// bind texture location
	m_classicPhong.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_quadTransform)));

	m_classicPhong.bindUniform("CameraPosition", camera.GetPosition());

	m_classicPhong.bindUniform("LightDirection", m_light.direction);
	m_classicPhong.bindUniform("ambientLight", m_ambientLight);
	m_classicPhong.bindUniform("diffuseLight", m_light.diffuse);
	m_classicPhong.bindUniform("specularLight", m_light.specular);

	// draw quad
	//m_quadMesh.draw();

	//m_classicPhong.bind();
	//m_classicPhong.bindUniform("ProjectionViewModel", pv * m_spearTransform);
	//m_texturedShader.bindUniform("diffuseTexture", 0);
	//m_spearTexture.bind(0);
	//m_spearMesh.draw();
	
	m_shader.bind();
	m_shader.bindUniform("ProjectionViewModel", pv * m_dragTransform);
	m_dragonMesh.draw();

#pragma endregion
	
#pragma region TexturedPhong

	// Phong stuff
	
	m_texturedPhong.bind();
	m_texturedPhong.bindUniform("ProjectionViewModel", pv * m_spearTransform);
	m_texturedPhong.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	m_texturedPhong.bindUniform("CameraPosition", camera.GetPosition());
	m_texturedPhong.bindUniform("LightDirection", m_light.direction);
	m_texturedPhong.bindUniform("ambientLight", m_ambientLight);
	m_texturedPhong.bindUniform("diffuseLight", m_light.diffuse);
	m_texturedPhong.bindUniform("specularLight", m_light.specular);
	m_texturedPhong.bindUniform("diffuseTexture", 0);
	m_spearTexture.bind(0);
	m_spearMesh.draw();
	
#pragma endregion

#pragma region NormalMapPhong

	m_normalMapPhong.bind();
	m_normalMapPhong.bindUniform("ProjectionViewModel", pv * m_spearTransform);
	m_normalMapPhong.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	m_normalMapPhong.bindUniform("CameraPosition", camera.GetPosition());
	m_normalMapPhong.bindUniform("LightDirection", m_light.direction);
	m_normalMapPhong.bindUniform("ambientLight", m_ambientLight);
	m_normalMapPhong.bindUniform("diffuseLight", m_light.diffuse);
	m_normalMapPhong.bindUniform("specularLight", m_light.specular);
	m_normalMapPhong.bindUniform("diffuseTexture", 0);
	m_normalMapPhong.bindUniform("specularTexture", 0);
	// no need for texture.bind(0), obj file handles that for us, along with OBJMesh.cpp

	m_spearMesh.draw();

#pragma endregion

*/

	// draw 3D gizmos
	Gizmos::draw(pv);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());

	// unbind the target from the back buffer

	m_renderTarget.unbind();

	//clear the back buffer
	clearScreen();

	m_postProcess.bind();
	m_postProcess.bindUniform("colorTarget", 0);
	m_postProcess.bindUniform("postProcessTarget", m_scene->GetPostProcessValue());
	m_postProcess.bindUniform("fTime", getTime());
	m_renderTarget.getTarget(0).bind(0);

	m_screenQuad.draw();
}

bool Application3D::ObjLoader(aie::OBJMesh& __objMesh, glm::mat4& _transform, const char* _filepath, std::string _filename, bool _flipTextures, float _scale, glm::vec3 _position)
{
	if (__objMesh.load(_filepath, true, _flipTextures) == false)
	{
		printf("Object Mesh loading had an error");
		return false;
	}

	_transform = {
		_scale, 0,0,0,
		0, _scale, 0,0,
		0,0,_scale, 0,
		_position.x, _position.y, _position.z, 1
	};

	return false;
}

