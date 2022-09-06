#pragma once

#include <cstddef>
#include <stdexcept>

#include "Matrix3D.hpp"
#include "Vector3D.hpp"
#include "Ray.hpp"

template <typename T> class Camera {
public:
  Vector3D<T> position;
  Vector3D<T> angles; // Euler-Angles: Yaw, Pitch & Roll
  size_t viewportWidth,
      viewportHeight; // The width and height of the viewport, number of rays
                      // being cast: width * height.

public:
  Camera<T>(const Vector3D<T> &position, const Vector3D<T> &angles,
            const size_t &viewportWidth, const size_t &viewportHeight) noexcept
      : position(position), angles(angles),
        viewportWidth(viewportWidth), viewportHeight(viewportHeight) {}

  /// Calculates the origin of the given ray.
  Ray<T> GetRayOrigin(const size_t &n) const {
    // Checks if the n is in the allowed range.
    if (n >= this->RayCount()) {
      throw std::runtime_error("n out of viewport range!");
    }

    // Gets the X and Y in the viewport.
    const size_t viewportY = this->ViewportY(n);
    const size_t viewportX = this->ViewportX(n);

    // Calculates the world coordinates of the ray origin.
    const Vector3D<T> origin(
        static_cast<T>(viewportX) - static_cast<T>(this->viewportWidth / 2),
        static_cast<T>(viewportY) - static_cast<T>(this->viewportHeight / 2),
        0.0);

    // Creates the direction vector.
    const Vector3D<T> direction(0.0, 0.0, 1.0);

    // Calculates the transformation matrix, to rotate the camera.
    Matrix3D<double> transformationMatrix =
        Matrix3D<double>::Rotation(this->angles);

    // Transforms the origin.
    const Vector3D<T> transformedOrigin = transformationMatrix.Multiply(origin).Add(this->position);

    // Transforms the direction vector.
    const Vector3D<T> transformedDirection = transformationMatrix.Multiply(direction);

    // Creates the result ray.
    Ray<T> ray(transformedOrigin, transformedDirection);

    // Returns the ray.
    return ray;
  }

  inline size_t ViewportX(const size_t &n) const noexcept {
    return n % this->viewportWidth;
  }

  inline size_t ViewportY(const size_t &n) const noexcept {
    return n / this->viewportWidth;
  }

  inline size_t RayCount() const noexcept {
    return this->viewportWidth * this->viewportHeight;
  }

  ~Camera<T>() noexcept = default;
};
