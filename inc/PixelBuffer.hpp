#pragma once

#include <cstdint>
#include <gtk/gtk.h>

#include "Vector3D.hpp"

class PixelBuffer {
public:
  GdkPixbuf *pixelBuffer;

public:
  PixelBuffer(const int &width, const int &height);

  PixelBuffer &PutPixel(const int &x, const int &y, const uint8_t r,
                        const uint8_t g, const uint8_t b, const uint8_t a);

  inline PixelBuffer &Fill(const uint8_t r, const uint8_t g, const uint8_t b,
                           const uint8_t a) {
    gdk_pixbuf_fill(this->pixelBuffer, static_cast<uint32_t>(r) << 24 |
                                           static_cast<uint32_t>(g) << 16 |
                                           static_cast<uint32_t>(b) << 8 |
                                           static_cast<uint32_t>(a) << 0);
    return *this;
  }

  ~PixelBuffer();
};
