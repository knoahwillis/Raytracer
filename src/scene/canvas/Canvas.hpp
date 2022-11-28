#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Canvas {
  protected:
  glm::ivec2 m_vecCanvasDim;
  glm::vec3 m_vecViewpoint = {1.0f, 1.0f, 1.0f}; // viewpoint through which we can see
  public:
  Canvas();
  Canvas(int w, int h);
  ~Canvas() {}
  glm::vec3 canvasToViewpoint(glm::ivec2 pos); // translate a point on the canvas to a point on the viewpoint (2D -> 3D)
  glm::ivec2 canvasToScreen(glm::ivec2 pos);    // translate a point from canvas coordinates to screen coordinates
};