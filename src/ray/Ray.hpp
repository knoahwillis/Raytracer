#pragma once
#include <glm/vec3.hpp>

class Ray{
protected:
  glm::vec3 m_vecOrigin;
  glm::vec3 m_vecDirection;

public:
  Ray(glm::vec3 origin, glm::vec3 direction);
  ~Ray();

public:
  glm::vec3 vec_pointOnRay(float t);
};