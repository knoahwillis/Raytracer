#include "Math.hpp"

// compute the reflection of a ray
glm::vec3 reflectRay(glm::vec3 ray, glm::vec3 norm) { return 2 * glm::dot(norm, ray) * norm - ray; }

// figures out if a ray intersects with a given sphere
glm::vec2 sphereRayIntersection(glm::vec3 origin, glm::vec3 direction, Sphere s) {
  float r = s.f_getRadius();
  glm::vec3 CO = origin - s.vec_getCenter();

  float a = glm::dot(direction, direction);
  float b = 2 * glm::dot(CO, direction);
  float c = glm::dot(CO, CO) - r * r;

  float discriminant = b * b - 4 * a * c;

  if (discriminant < 0) {
    return glm::vec2(MAXFLOAT, MAXFLOAT);
  }

  glm::vec2 ret = {(-b + sqrt(discriminant)) / (2 * a), (-b - sqrt(discriminant)) / (2 * a)};

  return ret;
}

// finds the closest sphere that a given ray intersects with as well as the distance
std::pair<Sphere, float> closestIntersection(Scene s, glm::vec3 origin, glm::vec3 direction, float t_min, float t_max) {
  float closest_t = MAXFLOAT;
  Sphere closest_sphere;
  for (auto& s : s.Spheres) {
    glm::vec2 vec_t = sphereRayIntersection(origin, direction, s);
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

// compute the intensity of the lighting at an intersection point (accounts for ambient light,
// shadows, diffuse lighting, and specular lighting)
float computeLighting(Scene s, glm::vec3 intersectionPoint, glm::vec3 intersectionNorm, glm::vec3 V, float spec) {
  float i = 0;
  float t_max;
  glm::vec3 L;
  for (auto& l : s.Lights) {
    if (l.getLightType() == AMBIENT) {
      i += l.f_getIntensity();
    } else {
      if (l.getLightType() == POINT) {
        L = l.vec_getVec() - intersectionPoint;
        t_max = 1;
      } else {
        L = l.vec_getVec();
        t_max = MAXFLOAT;
      }
    }

    // shadow check
    std::pair<Sphere, float> p_shadow = closestIntersection(s, intersectionPoint, L, .001, t_max);
    if (p_shadow.first.f_getRadius() > -1) {
      return i;
    }

    // diffuse lighting
    if (glm::dot(intersectionNorm, L) > 0) {
      i += l.f_getIntensity() * (glm::dot(intersectionNorm, L) / (glm::length(intersectionNorm) * glm::length(L)));
    }

    // specular lighting
    if (spec != -1) {
      glm::vec3 R = (2.0f * intersectionNorm * glm::dot(intersectionNorm, L)) - L;
      if (glm::dot(R, V) > 0) {
        i += l.f_getIntensity() * pow((glm::dot(R, V) / (glm::length(R) * glm::length(V))), spec);
      }
    }
  }
  return i;
}

// function that makes everything work. traces a ray, finds intersections, and calculates
// the color at the given point
glm::vec3 traceRay(Scene s, glm::vec3 origin, glm::vec3 direction, float t_min, float t_max, int reflectRecurse) {
  std::pair<Sphere, float> p_closestSphereFloat = closestIntersection(s, origin, direction, t_min, t_max);
  if (p_closestSphereFloat.first.f_getRadius() == -1) {
    return glm::vec3(255, 255, 255);
  }

  glm::vec3 P = origin + p_closestSphereFloat.second * direction;
  glm::vec3 N = glm::normalize(P - p_closestSphereFloat.first.vec_getCenter());

  glm::vec3 color = p_closestSphereFloat.first.vec_getColor() * computeLighting(s, P, N, -direction, p_closestSphereFloat.first.f_getSpecular());

  float r = p_closestSphereFloat.first.f_getReflective();
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
  glm::vec3 R = reflectRay(-direction, N);
  glm::vec3 reflectedColor = traceRay(s, P, R, 0.001f, MAXFLOAT, reflectRecurse - 1);

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