#pragma once

#include <algorithm>
#include <list>
#include <memory>
#include <optional>

#include "Material.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"

template <typename T> class Geometry {
public:
  Vector3D<T> position;
  Material<T> material;

public:
  Geometry<T>(const Vector3D<T> &position, const Material<T> &material)
      : position(position), material(material) {}

  virtual ~Geometry<T>() = default;

public:
  /// Performs an ray cast... If we've hit this target, we'll return the
  /// specifications of the hit item, else we'll return an nullopt.
  virtual std::optional<RayHitResult<T>> RayHit(const Ray<T> &ray) {
    return std::nullopt;
  }
};
