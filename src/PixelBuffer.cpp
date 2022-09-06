#include "PixelBuffer.hpp"
#include <stdexcept>

PixelBuffer::PixelBuffer(const int &width, const int &height)
    : pixelBuffer(gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, width, height)) {}

PixelBuffer &PixelBuffer::PutPixel(const int &x, const int &y, const uint8_t r,
                                   const uint8_t g, const uint8_t b,
                                   const uint8_t a) {
  const int width = gdk_pixbuf_get_width(this->pixelBuffer);
  const int height = gdk_pixbuf_get_height(this->pixelBuffer);

  if (y >= height || x >= width) {
    throw std::runtime_error("Invalid coordinates, outside of canvas.");
  }

  const int rowStride = gdk_pixbuf_get_rowstride(this->pixelBuffer);
  const int nChannels = gdk_pixbuf_get_n_channels(this->pixelBuffer);

  uint8_t *pixels =
      reinterpret_cast<uint8_t *>(gdk_pixbuf_get_pixels(this->pixelBuffer));
  uint8_t *p = pixels + y * rowStride + x * nChannels;
  p[0] = r;
  p[1] = g;
  p[2] = b;
  p[3] = a;

  return *this;
}

PixelBuffer::~PixelBuffer() { g_object_unref(this->pixelBuffer); }
