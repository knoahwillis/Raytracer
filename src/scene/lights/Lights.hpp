#pragma once
#include <glm/vec3.hpp>

enum _LIGHTTYPE_ { AMBIENT, POINT, DIRECTIONAL };

class Light {
  protected:
  float m_fIntensity;
  _LIGHTTYPE_ m_lightType;
  glm::vec3 m_vector;

  public:
  Light(_LIGHTTYPE_ type, float intensity, glm::vec3 vec = glm::vec3(3.40282e+038, 3.40282e+038, 3.40282e+038));
  _LIGHTTYPE_ getLightType();
  float getIntensity();
  glm::vec3 getVec();
};
