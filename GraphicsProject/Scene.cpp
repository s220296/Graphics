#include "Scene.h"
#include "Camera.h"
#include "Instance.h"

Scene::Scene(Camera* _camera, glm::vec2 _windowSize, Light& _globalLight)
	: m_mainCamera(_camera), m_windowSize(_windowSize), 
	m_globalLight(_globalLight)
{
}

Scene::~Scene()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		delete* it;
	}
}

void Scene::AddInstance(Instance* _instance)
{
	m_instances.push_back(_instance);
}

void Scene::Update(float _dt)
{
	m_mainCamera->Update(_dt);


	// The last function of this loop
	ImGUI_Functions();
}

void Scene::Draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < GetNumberOfLights(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColors[i] = m_pointLights[i].color;
	}

	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}
}

void Scene::ImGUI_Functions()
{
}
