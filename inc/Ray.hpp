#pragma once

#include "Vector3D.hpp"

template <typename T> class Ray {
public:
  Vector3D<T> origin;
  Vector3D<T> direction;

public:
  Ray<T>(const Vector3D<T> &origin, const Vector3D<T> &direction) noexcept
      : origin(origin), direction(direction) {}

  Ray<T> Reflect(const Vector3D<T> &position, const Vector3D<T> &normal) {
    Ray<T> ray(position, this->direction.Subtract(normal.Multiply(2.0 * this->direction.Dot(normal))));
    return ray;
  }

  ~Ray<T>() noexcept = default;
};

template <typename T> class RayHitResult {
public:
  Vector3D<T> point;
  Vector3D<T> normal;
  T distance;

public:
  RayHitResult<T>(const Vector3D<T> &point, const Vector3D<T> &normal,
                  const T &distance) noexcept
      : point(point), normal(normal), distance(distance) {}

  ~RayHitResult<T>() noexcept = default;
};

