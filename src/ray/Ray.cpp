#include "Ray.hpp"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
  m_vecOrigin = origin;
  m_vecDirection = direction;
}

Ray::~Ray() {}

glm::vec3 Ray::vec_pointOnRay(float t) {
  return m_vecOrigin + t * m_vecDirection;
}