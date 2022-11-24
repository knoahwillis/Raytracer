#include "Canvas.hpp"


Canvas::Canvas() {
  m_vecCanvasDim = {0, 0};
}

Canvas::Canvas(int w, int h) {
  m_vecCanvasDim.x = w;
  m_vecCanvasDim.y = h;
}


glm::vec3 Canvas::vec_canvasToViewpoint(glm::ivec2 pos) {
  return glm::vec3(
    pos.x * (m_vecViewpoint.x / m_vecCanvasDim.x),
    pos.y * (m_vecViewpoint.y / m_vecCanvasDim.y),
    1.0f
  );
}

glm::ivec2 Canvas::vec_canasToScreen(glm::ivec2 pos) {
  return glm::ivec2(
    (m_vecCanvasDim.x / 2) + pos.x,
    (m_vecCanvasDim.y / 2) - pos.y
  );
}