#pragma once
#include "./canvas/Canvas.hpp"
#include "./lights/Lights.hpp"
#include "./shapes/Sphere.hpp"
#include <SDL2/SDL.h>
#include <glm/geometric.hpp>
#include <utility>
#include <vector>

struct Scene {
  std::vector<Sphere> Spheres;
  std::vector<Light> Lights;
  void addSphere(Sphere s) { Spheres.push_back(s); }
  void addLight(Light l) { Lights.push_back(l); }
};
