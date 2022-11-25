#pragma once
#include "./math/Math.hpp"

class Raytracer {
  protected:
  int width;
  int height;
  SDL_Window* window;
  SDL_Renderer* rend;
  bool closed;
  Canvas canvas;
  Scene scene;
  glm::vec3 Origin;

  public:
  Raytracer(int w, int h);
  ~Raytracer();

  public:
  void insertLight(Light l);
  void insertSphere(Sphere s);
  void raytrace();
  void render();
  bool running();
};