#include "Raytracer.hpp"
#include <memory>

int main(int argc, char* argv[]) {
  std::unique_ptr<Raytracer> raytracer = std::make_unique<Raytracer>(500, 500);

  raytracer->insertSphere(Sphere(1, 500, 0.2f, glm::vec3(0.0f, -1.0f, 3.0f), glm::ivec3(255, 0, 0)));
  raytracer->insertSphere(Sphere(1, 500, 0.3f, glm::vec3(2.0f, 0.0f, 4.0f), glm::ivec3(0, 0, 255)));

  raytracer->insertLight((Light(AMBIENT, 0.2)));
  raytracer->insertLight((Light(POINT, 0.6, glm::vec3(2.0f, 1.0f, 0.0f))));
  raytracer->insertLight((Light(DIRECTIONAL, 0.2, glm::vec3(1.0f, 4.0f, 4.0f))));

  while (raytracer->running()) {
    raytracer->render();
  }
  return 0;
}
