#pragma once
#include "./ray/Ray.hpp"

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
  void insertLight(Light l);
  void insertSphere(Sphere s);
  void render();
  bool running();
};