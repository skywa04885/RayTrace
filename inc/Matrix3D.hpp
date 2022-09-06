#pragma once

#include <iostream>

#include "Vector3D.hpp"

template <typename T> class Matrix3D {
public:
  Vector3D<T> c1, c2, c3;

public:
  Matrix3D<T>(const Vector3D<T> &c1, const Vector3D<T> &c2,
              const Vector3D<T> &c3) noexcept
      : c1(c1), c2(c2), c3(c3) {}

  ~Matrix3D<T>() noexcept = default;
public:
  Vector3D<T> Multiply(const Vector3D<T> &vector) const noexcept {
    return Vector3D<T>(
        vector.x * this->c1.x + vector.y * this->c2.x + vector.z * this->c3.x,
        vector.x * this->c1.y + vector.y * this->c2.y + vector.z * this->c3.y,
        vector.x * this->c1.z + vector.y * this->c2.z + vector.z * this->c3.z
    );
  }

public:
  static constexpr Matrix3D<T> Create(const T data[3][3]) noexcept {
    return Matrix3D<T>(Vector3D<T>(data[0][0], data[1][0], data[2][0]),
                       Vector3D<T>(data[0][1], data[1][1], data[2][1]),
                       Vector3D<T>(data[0][2], data[1][2], data[2][2]));
  }

  static Matrix3D<T> Rotation(const Vector3D<T> &angles) noexcept {
    const T &alpha = angles.x; // Yaw
    const T &beta = angles.y;  // Pitch
    const T &gamma = angles.z; // Roll
    
    const T data[3][3] = {{std::cos(beta) * std::cos(gamma),
                    std::sin(alpha) * std::sin(beta) * std::cos(gamma) -
                        std::cos(alpha) * std::sin(gamma),
                    std::cos(alpha) * std::sin(beta) * std::cos(gamma) +
                        std::sin(alpha) * std::sin(gamma)},
                   {std::cos(beta) * std::sin(gamma),
                    std::sin(alpha) * std::sin(beta) * std::sin(gamma) +
                        std::cos(alpha) * std::cos(gamma),
                    std::cos(alpha) * std::sin(beta) * std::sin(gamma) -
                        std::sin(alpha) * std::cos(gamma)},
                   {-std::sin(beta), std::sin(alpha) * std::cos(beta),
                    std::cos(alpha) * std::cos(beta)}};

    return Create(data);
  }
};

template<typename T>
std::ostream &operator << (std::ostream &stream, const Matrix3D<T> &matrix) {
  stream << '{' << std::endl;
  stream << "\t{" << matrix.c1.x << ", " << matrix.c2.x << ", " << matrix.c3.x << '}' << std::endl;
  stream << "\t{" << matrix.c1.y << ", " << matrix.c2.y << ", " << matrix.c3.y << '}' << std::endl;
  stream << "\t{" << matrix.c1.z << ", " << matrix.c2.z << ", " << matrix.c3.z << '}' << std::endl;
  stream << '}' << std::endl;

  return stream;
}
