#pragma once
#include <glm/vec3.hpp>

class Sphere {
  protected:
  float m_fRadius;
  float m_fSpecular;
  float m_fReflective;
  glm::vec3 m_vecCenter;
  glm::ivec3 m_vecColor;

  public:
  Sphere() {
    m_fRadius = -1;
    m_fSpecular = -1;
    m_fReflective = -1;
    m_vecCenter = glm::vec3(0, 0, 0);
    m_vecColor = glm::ivec3(0, 0, 0);
  }
  Sphere(float radius, float spec, float ref, glm::vec3 center, glm::ivec3 color) {
    m_fRadius = radius;
    m_fSpecular = spec;
    m_fReflective = ref;
    m_vecCenter = center;
    m_vecColor = color;
  }
  ~Sphere() {}
  glm::vec3 getCenter() { return m_vecCenter; }
  glm::vec3 getColor() { return m_vecColor; }
  float getRadius() { return m_fRadius; }
  float getSpecular() { return m_fSpecular; }
  float getReflective() { return m_fReflective; }
};