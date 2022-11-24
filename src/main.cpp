#include "./canvas/Canvas.hpp"
#include "./lights/Lights.hpp"
#include "./ray/Ray.hpp"
#include "./shapes/Sphere.hpp"
#include <SDL2/SDL.h>
#include <glm/geometric.hpp>
#include <iostream>
#include <memory>
#include <vector>

float f_computeLighting(std::vector<Light> lights, glm::vec3 intersectionPoint, glm::vec3 intersectionNorm, glm::vec3 V, float s) {
  float i = 0;
  glm::vec3 L;
  for (auto& l : lights) {
    if (l.getLightType() == AMBIENT) {
      i += l.f_getIntensity();
    } else {
      if (l.getLightType() == POINT) {
        L = l.vec_getVec() - intersectionPoint;
      } else {
        L = l.vec_getVec();
      }
    }

    if (glm::dot(intersectionNorm, L) > 0) {
      i += l.f_getIntensity() * (glm::dot(intersectionNorm, L) / (glm::length(intersectionNorm) * glm::length(L)));
    }

    if (s != -1) {
      glm::vec3 R = (2.0f * intersectionNorm * glm::dot(intersectionNorm, L)) - L;
      if (glm::dot(R, V) > 0) {
        i += l.f_getIntensity() * pow((glm::dot(R, V) / (glm::length(R) * glm::length(V))), s);
      }
    }
  }
  return i;
}

glm::vec2 vec_sphereRayIntersection(glm::vec3 origin, glm::vec3 direction, Sphere& s) {
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

glm::ivec3 vec_traceRay(std::vector<Light> lights, std::vector<Sphere> spheres, glm::vec3 origin, glm::vec3 direction, float t_min, float t_max) {
  float closest_t = MAXFLOAT;
  Sphere closest_sphere;
  for (auto s : spheres) {
    glm::vec2 vec_t = vec_sphereRayIntersection(origin, direction, s);
    if (vec_t.x >= t_min && vec_t.x <= t_max && vec_t.x < closest_t) {
      closest_t = vec_t.x;
      closest_sphere = s;
    }
    if (vec_t.y >= t_min && vec_t.y <= t_max && vec_t.y < closest_t) {
      closest_t = vec_t.y;
      closest_sphere = s;
    }
  }
  if (closest_sphere.f_getRadius() < 0) {
    return glm::vec3(255, 255, 255);
  }

  glm::vec3 point = origin + closest_t * direction;
  glm::vec3 norm = glm::normalize(point - closest_sphere.vec_getCenter());
  glm::ivec3 tester = closest_sphere.vec_getColor() * f_computeLighting(lights, point, norm, -direction, closest_sphere.f_getSpecular());
  if(tester.x > 255) {
    tester.x = 255;
  }
  if(tester.y > 255) {
    tester.y = 255;
  }
  if(tester.z > 255) {
    tester.z = 255;
  }
  return tester;
}

int main(int argc, char* argv[]) {

  SDL_Window* window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 0);
  SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  bool closed = false;

  Canvas canvas(800, 800);
  std::vector<Sphere> Spheres;
  std::vector<Light> Lights;

  Spheres.push_back(Sphere(1, 500, glm::vec3(0.0f, -1.0f, 3.0f), glm::ivec3(255, 0, 0)));
  Spheres.push_back(Sphere(1, 500, glm::vec3(2.0f, 0.0f, 4.0f), glm::ivec3(0, 0, 255)));
  Spheres.push_back(Sphere(1, 10, glm::vec3(-2.0f, 0.0f, 4.0f), glm::ivec3(0, 255, 0)));
  Spheres.push_back(Sphere(5000, 1000, glm::vec3(0, -5001, 0), glm::vec3(255, 255, 0)));

  Lights.push_back(Light(AMBIENT, 0.2));
  Lights.push_back(Light(POINT, 0.6, glm::vec3(2.0f, 1.0f, 0.0f)));
  Lights.push_back(Light(DIRECTIONAL, 0.2, glm::vec3(1.0f, 4.0f, 4.0f)));

  SDL_RenderClear(rend);
  glm::vec3 Origin = {0.0f, 0.0f, 0.0f};
  for (int j = -800 / 2; j < 800 / 2; j++) {
    for (int i = -800 / 2; i < 800 / 2; i++) {
      glm::vec3 Direction = canvas.vec_canvasToViewpoint(glm::ivec2(i, j));
      glm::ivec3 color = vec_traceRay(Lights, Spheres, Origin, Direction, 1.0f, MAXFLOAT);
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