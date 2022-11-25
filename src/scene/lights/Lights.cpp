#include "Lights.hpp"

Light::Light(_LIGHTTYPE_ type, float intensity, glm::vec3 vec) {
  m_lightType = type;
  m_fIntensity = intensity;
  if (type != AMBIENT) {
    m_vector = vec;
  }
}

_LIGHTTYPE_ Light::getLightType() { return m_lightType; }

float Light::getIntensity() { return m_fIntensity; }

glm::vec3 Light::getVec() { return m_vector; }