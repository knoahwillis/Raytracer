#include "Lights.hpp"

Light::Light(_LIGHTTYPE_ type, float intensity, glm::vec3 vec) {
  m_lightType = type;
  m_fIntensity = intensity;
  if(type != AMBIENT) {
    m_vector = vec;
  }
}