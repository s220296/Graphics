#pragma once
#include <glm/glm.hpp>
#include <list>
#include <vector>

class Camera;
class Instance;

const int MAX_LIGHTS = 4;

struct Light
{
	Light()
	{
		direction = glm::vec3(0);
		color = glm::vec3(1);
		intensity = 1;
	}

	Light(glm::vec3 _position, 
		glm::vec3 _color,
		float _intensity = 1)
	{
		direction = _position;
		color = _color;
		intensity = _intensity;
	}

	glm::vec3 direction;
	glm::vec3 color;
	float intensity;

	glm::vec3 GetColor(){
		return color * intensity;
	}
};

class Scene
{
public:
	Scene(Camera* _camera,
		glm::vec2 _windowSize,
		Light& _globalLight);
	~Scene();
	void AddInstance(Instance* _instance);
	void Update(float _dt);
	void Draw();
	void DrawDepth();
	void ImGUI_Functions();
	
	void AddPointLights(Light _light) 
	{ m_pointLights.push_back(_light); }

	Camera* GetCamera() 
	{ return m_mainCamera;					}

	glm::vec2 GetWindowSize()
	{ return m_windowSize;					}

	glm::vec3 GetAmbientLightColor()
	{ return m_globalLight.color;			}

	void SetAmbientLightColor(glm::vec3 _color)
	{ m_globalLight.color = _color;			}

	Light& GetGlobalLight()
	{ return m_globalLight;					}

	std::vector<Light>& GetPointLights()
	{ return m_pointLights;					}

	int GetNumberOfLights()
	{ return m_pointLights.size();			}

	glm::vec3* GetPointLightPositions()
	{ return &m_pointLightPositions[0];		}

	glm::vec3* GetPointLightColors()
	{ return &m_pointLightColors[0];		}

	int GetPostProcessValue()
	{ return m_postProcessEffect;			}

protected:
	Camera*					m_mainCamera;
	glm::vec2				m_windowSize;
	std::list<Instance*>	m_instances;
	Light					m_globalLight;
	std::vector<Light>		m_pointLights;
	glm::vec3				m_pointLightPositions[MAX_LIGHTS];
	glm::vec3				m_pointLightColors[MAX_LIGHTS];

	int						m_postProcessEffect = 0;
};

