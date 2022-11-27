#include "Ray.hpp"

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
  m_origin = origin;
  m_direction = direction;
}

Ray::~Ray() {}

glm::vec3 Ray::reflectRay(glm::vec3 norm) {
  return 2 * glm::dot(norm, m_direction) * norm - m_direction;
}

glm::vec2 Ray::sphereRayIntersection(Sphere s) {
  float r = s.getRadius();
  glm::vec3 CO = m_origin - s.getCenter();

  float a = glm::dot(m_direction, m_direction);
  float b = 2 * glm::dot(CO, m_direction);
  float c = glm::dot(CO, CO) - r * r;

  float discriminant = b * b - 4 * a * c;

  if (discriminant < 0) {
    return glm::vec2(MAXFLOAT, MAXFLOAT);
  }

  return glm::vec2((-b + sqrt(discriminant)) / (2 * a), (-b - sqrt(discriminant)) / (2 * a));
}

std::pair<Sphere, float> Ray::closestIntersection(Scene sc, float t_min, float t_max) {
  float closest_t = MAXFLOAT;
  Sphere closest_sphere;
  for (auto& s : sc.Spheres) {
    glm::vec2 vec_t = this->sphereRayIntersection(s);
    if (vec_t.x >= t_min && vec_t.x <= t_max && vec_t.x < closest_t) {
      closest_t = vec_t.x;
      closest_sphere = s;
    }
    if (vec_t.y >= t_min && vec_t.y <= t_max && vec_t.y < closest_t) {
      closest_t = vec_t.y;
      closest_sphere = s;
    }
  }
  return std::make_pair(closest_sphere, closest_t);
}

float Ray::computeLighting(Scene s, glm::vec3 intersectionPoint, glm::vec3 intersectionNorm, glm::vec3 V, float spec) {
  float i = 0;
  float t_max;
  glm::vec3 L;
  for (auto& l : s.Lights) {
    if (l.getLightType() == AMBIENT) {
      i += l.getIntensity();
    } else {
      if (l.getLightType() == POINT) {
        L = l.getVec() - intersectionPoint;
        t_max = 1;
      } else {
        L = l.getVec();
        t_max = MAXFLOAT;
      }
    }

    // shadow check
    std::pair<Sphere, float> p_shadow = Ray(intersectionPoint, L).closestIntersection(s, .001f, t_max);
    if (p_shadow.first.getRadius() > -1) {
      return i;
    }

    // diffuse lighting
    if (glm::dot(intersectionNorm, L) > 0) {
      i += l.getIntensity() * (glm::dot(intersectionNorm, L) / (glm::length(intersectionNorm) * glm::length(L)));
    }

    // specular lighting
    if (spec != -1) {
      glm::vec3 R = (2.0f * intersectionNorm * glm::dot(intersectionNorm, L)) - L;
      if (glm::dot(R, V) > 0) {
        i += l.getIntensity() * pow((glm::dot(R, V) / (glm::length(R) * glm::length(V))), spec);
      }
    }
  }
  return i;
}

glm::vec3 Ray::traceRay(Scene s, float t_min, float t_max, int reflectRecurse) {
  std::pair<Sphere, float> p_closestSphereFloat = this->closestIntersection(s, t_min, t_max);
  if (p_closestSphereFloat.first.getRadius() == -1) {
    return glm::vec3(255, 255, 255);
  }

  glm::vec3 P = m_origin + p_closestSphereFloat.second * m_direction;
  glm::vec3 N = glm::normalize(P - p_closestSphereFloat.first.getCenter());

  glm::vec3 color = p_closestSphereFloat.first.getColor() * computeLighting(s, P, N, -m_direction, p_closestSphereFloat.first.getSpecular());

  float r = p_closestSphereFloat.first.getReflective();
  if (r <= 0 || reflectRecurse <= 0) {
    if (color.x > 255) {
      color.x = 255;
    }
    if (color.y > 255) {
      color.y = 255;
    }
    if (color.z > 255) {
      color.z = 255;
    }
    return color;
  }
  glm::vec3 R = Ray(m_origin, -m_direction).reflectRay(N);
  glm::vec3 reflectedColor = Ray(P, R).traceRay(s, 0.001f, MAXFLOAT, reflectRecurse - 1);

  glm::vec3 retColor = color * (1 - r) + reflectedColor * r;
  if (retColor.x > 255) {
    retColor.x = 255;
  }
  if (retColor.y > 255) {
    retColor.y = 255;
  }
  if (retColor.z > 255) {
    retColor.z = 255;
  }
  return retColor;
}