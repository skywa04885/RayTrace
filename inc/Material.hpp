#pragma once

#include "Vector3D.hpp"
template <typename T> class Material {
public:
  Vector3D<T> color;
  T reflectivity;

public:
  Material<T>(const Vector3D<T> &color, const T &reflectivity)
      : color(color), reflectivity(reflectivity) {}

  ~Material<T>() = default;
};
