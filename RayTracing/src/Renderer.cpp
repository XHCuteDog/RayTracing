#include "Renderer.h"

#include "Walnut/Random.h"

void Renderer::Render() {
  // render every pixel
  for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
    for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
      glm::vec2 coord = {x / (float)m_FinalImage->GetWidth(),
                         y / (float)m_FinalImage->GetHeight()};
      coord = coord * 2.0f - 1.0f; // -1 -- 1 
      m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);
    }
  }

  m_FinalImage->SetData(m_ImageData);
}
void Renderer::OnResize(uint32_t width, uint32_t height) {
  if (!m_FinalImage) {
    m_FinalImage = std::make_shared<Walnut::Image>(width, height,
                                                   Walnut::ImageFormat::RGBA);
  } else {
    // not necessary to resize.
    if (m_FinalImage->GetHeight() == height &&
        m_FinalImage->GetWidth() == width)
      return;
    m_FinalImage->Resize(width, height);
  }

  delete[] m_ImageData;
  m_ImageData = new uint32_t[width * height];
}

uint32_t Renderer::PerPixel(glm::vec2 coord) {
  uint8_t r = (uint8_t)(coord.x * 255.0f);
  uint8_t g = (uint8_t)(coord.y * 255.0f);

  glm::vec3 rayOrigin(0.0f, 0.0f, -1.0f);
  glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
  float radius = 0.5f;
  rayDirection = glm::normalize(rayDirection);
  // For a circle, the radius is R and the center is at the origin, 
  // we emit a ray from a fixed point in a determined direction, does this ray intersect with this circle? 
  // Suppose the coordinates of the point are (ax, ay) and the direction is (bx, by)
  // len of the ray on the corresponding axis is bx*t (by*t) or hit distance
  // (bx^2 + by^2)t^2 + (2 (axbx + ayby))t + (ax^2 + ay^2 - R^2) = 0
  float a = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y +
            rayDirection.z * rayDirection.z;
  // equal to: float a = glm::dot(rayDirection, rayDirection);
  float b = 2.0f * glm::dot(rayOrigin, rayDirection);
  float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

  float discriminant = b * b - 4.0f * a * c;

  if (discriminant >= 0.0f) return 0xffff00ff;
  return 0xff000000;
  //return 0xff000000 | (g << 8) | r;
}
