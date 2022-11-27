#pragma once
#include "../scene/Scene.hpp"

class Ray {
  protected:
  glm::vec3 m_origin;
  glm::vec3 m_direction;

  protected:
  // compute the reflection of a ray
  // parameters:
  // glm::vec3 norm -- norm of the ray that is reflecting and the object it is reflecting off of
  // return:
  // glm::vec3 -- reflected ray
  glm::vec3 reflectRay(glm::vec3 norm);

  // figures out if a ray intersects with a given sphere
  // parameters:
  // Sphere s -- sphere that is being checked
  // return:
  // glm::vec2 -- vector holds both solutions to the quadratic equation if
  //              there are real solutions, hold {INF, INF} if there are no
  // .            real solutions
  glm::vec2 sphereRayIntersection(Sphere s);

  // finds the closest sphere that a ray intersects with as well as the distance
  // parameters:
  // Scene s -- the scene containing all of the objects and lights
  // float t_min -- closest distance that a sphere can be
  // float t_max -- furthest distance that a sphere can be
  // return:
  // std::pair<Sphere, float> -- pair containing the closest sphere that a ray intersects with
  //                             and the distance it is from the ray origin
  std::pair<Sphere, float> closestIntersection(Scene sc, float t_min, float t_max);

  // compute the intensity of the lighting at an intersection point (accounts for ambient light,
  // shadows, diffuse lighting, and specular lighting)
  // parameters:
  // Scene s -- the scene containing all of the objects and lights
  // glm::vec3 intersectionPoint -- the point where the ray intersects and object
  // glm::vec3 intersectionNorm -- the norm of the vector and the object it intersects
  // glm::vec3 V -- the ray bouncing from the object to the camera
  // float spec -- the specular strength of the sphere that is being intersected
  // return:
  // float -- float of the intensity of the lighting at a given intersection point
  float computeLighting(Scene s, glm::vec3 intersectionPoint, glm::vec3 intersectionNorm, glm::vec3 V, float spec);

  public:
  Ray(glm::vec3 origin, glm::vec3 direction);
  ~Ray();

  // function that makes everything work. traces a ray, finds intersections, and calculates
  // the color at the given point
  // parameters:
  // Scene s -- the scene containing all of the objects and lights
  // float t_min -- closest distance that an object can be found
  // float t_max -- furthest distance that an object can be found
  // int reflectRecurse -- the number of times we recursively look for reflections (works best with 2 or 3, defaulted to 2)
  // returns:
  // glm::vec3 -- vector representing the rgb values of a given pixel on the screen
  glm::vec3 traceRay(Scene s, float t_min, float t_max, int reflectRecurse);
};