#include "Scene.h"
#include "Camera.h"
#include "Instance.h"
#include "Gizmos.h"
#include <imgui.h>
#include <string>

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
		glm::vec3 gizmoPos(m_pointLights[i].direction.x, 
			m_pointLights[i].direction.y, 
			m_pointLights[i].direction.z);

		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColors[i] = m_pointLights[i].GetColor();
		aie::Gizmos::addSphere(gizmoPos, m_pointLights[i].intensity * 0.01f, 5, 5,
			glm::vec4(m_pointLights[i].color, 1));
	}

	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}
}

void Scene::ImGUI_Functions()
{
	ImGui::Begin("Lights");

	ImGui::Text("Global Light");
	ImGui::SliderFloat3("Direction: Global", &GetGlobalLight().direction[0], -1.f, 1.f);
	ImGui::ColorEdit3("Color: Global", &GetGlobalLight().color[0]);
	ImGui::Spacing();

	ImGui::Text("Point Lights");
	for (int i = 0; i < m_pointLights.size() && i < MAX_LIGHTS; i++)
	{
		std::string name = "Color: ";
		name += i + 1 + 48;
		ImGui::ColorEdit3(name.c_str(), &m_pointLights[i].color.x);

		name = "Position: ";
		name += i + 1 + 48;
		ImGui::SliderFloat3(name.c_str(), &m_pointLights[i].direction.x, -10.f, 10.f);

		name = "Intensity: ";
		name += i + 1 + 48;
		ImGui::SliderFloat(name.c_str(), &m_pointLights[i].intensity, 0.f, 50.f);

		ImGui::Spacing();
	}

	ImGui::End();

	ImGui::Begin("Post Process Settings");
	ImGui::DragInt("Active Post Processing Effect", &m_postProcessEffect, 0.05f, 0, 10);
	ImGui::End();
}
