#include "./canvas/Canvas.hpp"
#include "./lights/Lights.hpp"
#include "./ray/Ray.hpp"
#include "./shapes/Sphere.hpp"
#include <SDL2/SDL.h>
#include <glm/geometric.hpp>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

struct Scene {
  std::vector<Sphere> Spheres;
  std::vector<Light> Lights;
  void addSphere(Sphere s) { Spheres.push_back(s); }
  void addLight(Light l) { Lights.push_back(l); }
};

glm::vec3 reflectRay(glm::vec3 ray, glm::vec3 norm) { return 2 * glm::dot(norm, ray) * norm - ray; }

glm::vec2 sphereRayIntersection(glm::vec3 origin, glm::vec3 direction, Sphere& s) {
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

std::pair<Sphere, float> closestIntersection(Scene s, glm::vec3 origin, glm::vec3 direction, float t_min, float t_max) {
  float closest_t = MAXFLOAT;
  Sphere closest_sphere;
  for (auto s : s.Spheres) {
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

glm::vec3 traceRay(Scene s, glm::vec3 origin, glm::vec3 direction, float t_min, float t_max, int reflectRecurse) {
  std::pair<Sphere, float> p_closestSphereFloat = closestIntersection(s, origin, direction, t_min, t_max);
  if (p_closestSphereFloat.first.f_getRadius() == -1) {
    return glm::vec3(0, 0, 0);
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

int main(int argc, char* argv[]) {

  SDL_Window* window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 0);
  SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  bool closed = false;

  Canvas canvas(800, 800);
  Scene scene;

  scene.addSphere(Sphere(1, 500, 0.2f, glm::vec3(0.0f, -1.0f, 3.0f), glm::ivec3(255, 0, 0)));
  scene.addSphere(Sphere(1, 500, 0.3f, glm::vec3(2.0f, 0.0f, 4.0f), glm::ivec3(0, 0, 255)));
  scene.addSphere(Sphere(1, 10, 0.4f, glm::vec3(-2.0f, 0.0f, 4.0f), glm::ivec3(0, 255, 0)));
  scene.addSphere(Sphere(5000, 1000, 0.5f, glm::vec3(0, -5001, 0), glm::vec3(255, 255, 0)));

  scene.addLight((Light(AMBIENT, 0.2)));
  scene.addLight((Light(POINT, 0.6, glm::vec3(2.0f, 1.0f, 0.0f))));
  scene.addLight((Light(DIRECTIONAL, 0.2, glm::vec3(1.0f, 4.0f, 4.0f))));

  SDL_RenderClear(rend);
  glm::vec3 Origin = {0.0f, 0.0f, 0.0f};
  for (int j = -800 / 2; j < 800 / 2; j++) {
    for (int i = -800 / 2; i < 800 / 2; i++) {
      glm::vec3 Direction = canvas.vec_canvasToViewpoint(glm::ivec2(i, j));
      glm::vec3 color = traceRay(scene, Origin, Direction, 1.0f, MAXFLOAT, 1);
      SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, 0);
      glm::ivec2 screenPoint = canvas.vec_canasToScreen(glm::ivec2(i, j));
      SDL_RenderDrawPoint(rend, screenPoint.x, screenPoint.y);
    }
  }

  SDL_RenderPresent(rend);

  while (!closed) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        closed = true;
        break;
      case SDL_KEYDOWN:
        switch (e.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
          closed = true;
          break;
        }
        break;
      }
    }
  }
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}