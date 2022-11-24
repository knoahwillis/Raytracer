#pragma once
#include <glm/vec3.hpp>

enum _LIGHTTYPE_ { AMBIENT, POINT, DIRECTIONAL };

class Light{
protected:
  float m_fIntensity;
  _LIGHTTYPE_ m_lightType;
  glm::vec3 m_vector;
public:
  Light(_LIGHTTYPE_ type, float intensity, glm::vec3 vec = glm::vec3(3.40282e+038, 3.40282e+038, 3.40282e+038));
public:
  _LIGHTTYPE_ getLightType() { return m_lightType; }
  float f_getIntensity() { return m_fIntensity; }
  glm::vec3 vec_getVec() { return m_vector; }
};
