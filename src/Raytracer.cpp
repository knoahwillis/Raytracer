#include "Raytracer.hpp"

Raytracer::Raytracer(int w, int h) {
  width = w;
  height = h;
  window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
  rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  closed = false;
  canvas = Canvas(w, h);
  scene = Scene();
  Origin = {0.0f, 0.0f, 0.0f};
}

Raytracer::~Raytracer() {
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Raytracer::insertLight(Light l) { scene.addLight(l); }

void Raytracer::insertSphere(Sphere s) { scene.addSphere(s); }

void Raytracer::raytrace() {
  for (int j = -height / 2; j < height / 2; j++) {
    for (int i = -width / 2; i < width / 2; i++) {
      glm::vec3 Direction = canvas.vec_canvasToViewpoint(glm::ivec2(i, j));
      glm::vec3 color = traceRay(scene, Origin, Direction, 1.0f, MAXFLOAT, 1);
      SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, 0);
      glm::ivec2 screenPoint = canvas.vec_canasToScreen(glm::ivec2(i, j));
      SDL_RenderDrawPoint(rend, screenPoint.x, screenPoint.y);
    }
  }
}

void Raytracer::render() {
  while (!closed) {
    SDL_RenderClear(rend);
    raytrace();
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
  }
}

bool Raytracer::running() { return !closed; }