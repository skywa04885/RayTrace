#pragma once

#include <cmath>
#include <iostream>

#include "Colors.hpp"

template <typename T> class Vector3D {
public:
  T x, y, z;

public:
  Vector3D<T>(const T &x, const T &y, const T &z) noexcept : x(x), y(y), z(z) {}

  Vector3D<T> Clone() const noexcept {
    return Vector3D<T>(this->x, this->y, this->z);
  }

  Vector3D<T> Add(const Vector3D<T> &other) const noexcept {
    return Vector3D<T>(this->x + other.x, this->y + other.y, this->z + other.z);
  }

  Vector3D<T> Subtract(const Vector3D<T> &other) const noexcept {
    return Vector3D<T>(this->x - other.x, this->y - other.y, this->z - other.z);
  }

  Vector3D<T> Multiply(const T &multipler) const noexcept {
    return Vector3D<T>(this->x * multipler, this->y * multipler,
                       this->z * multipler);
  }

  Vector3D<T> Divide(const T &divisor) const noexcept {
    return Vector3D<T>(this->x / divisor, this->y / divisor, this->z / divisor);
  }

  T Dot(const Vector3D<T> &other) const noexcept {
    return this->x * other.x + this->y * other.y + this->z * other.z;
  }

  Vector3D<T> Normalize() const noexcept {
    const T magnitude = this->Magnitude();
    return Vector3D<T>(this->x / magnitude, this->y / magnitude,
                       this->z / magnitude);
  }

  T Magnitude() const noexcept {
    return static_cast<T>(std::sqrt(
        std::pow(this->x, 2) + std::pow(this->y, 2) + std::pow(this->z, 2)));
  }

  static Vector3D<T> Mix(const T &p1, const Vector3D<T> &v1, const T &p2,
                         const Vector3D<T> &v2) {
    return v1.Add(v2).Divide(p1 + p2);
  }

  ~Vector3D<T>() noexcept = default;
};

typedef Vector3D<double> DVector3D;

template <typename T>
std::ostream &operator<<(std::ostream &stream, const Vector3D<T> &vector) {
  stream << '<' << vector.x << ", " << vector.y << ", " << vector.z << '>';
  return stream;
}
