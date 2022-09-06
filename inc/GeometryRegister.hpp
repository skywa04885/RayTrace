#pragma once

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <tuple>

#include "Geometry.hpp"
#include "Ray.hpp"

template <typename T> class GeometryRegister {
public:
  std::list<std::shared_ptr<Geometry<T>>> geometries;

public:
  GeometryRegister<T>() : geometries({}) {}

  GeometryRegister<T> &Register(std::shared_ptr<Geometry<T>> geometry) {
    this->geometries.push_back(geometry);
    return *this;
  }

  GeometryRegister<T> &Print() {
    std::for_each(this->geometries.begin(), this->geometries.end(),
                  [](std::shared_ptr<Geometry<T>> geometry) {
                    std::cout << geometry << std::endl;
                  });
    return *this;
  }

  /// Casts an ray against all the geometry in the register, and returns a
  /// possible hit.
  std::optional<std::tuple<std::shared_ptr<Geometry<T>>, RayHitResult<T>>>
  CastRay(const Ray<T> &ray) {
    std::optional<RayHitResult<T>> nearestHitResult = std::nullopt;
    std::optional<std::shared_ptr<Geometry<T>>> nearestGeometry =
        std::nullopt;

    // Loops over all the pieces of geometry, and performs the ray casting on
    // them.
    std::for_each(this->geometries.begin(), this->geometries.end(),
                  [&](std::shared_ptr<Geometry<T>> geometry) {
                    // Checks if the casted ray hits the object.
                    std::optional<RayHitResult<T>> hitResult =
                        geometry->RayHit(ray);

                    // If the casted ray did not hit the object, simply return.
                    if (!hitResult.has_value()) {
                      return;
                    }

                    // If there is no existing hit result, set this one as the
                    // new result.
                    if (!nearestHitResult.has_value()) {
                      nearestHitResult = hitResult;
                      nearestGeometry = geometry;
                      return;
                    }

                    // Since there already is an nearest hit result, only modify
                    // it if the current hit is closer to the origin of the ray.
                    if (hitResult->distance < nearestHitResult->distance) {
                      nearestHitResult = hitResult;
                      nearestGeometry = geometry;
                      return;
                    }
                  });

    // Checks if there is any result in the first place.
    if (!nearestHitResult.has_value() || !nearestGeometry.has_value()) {
      return std::nullopt;
    }

    // Returns the nearest hit result, may be nullopt if nothing was hit.
    return std::tuple(*nearestGeometry, *nearestHitResult);
  }

  ~GeometryRegister<T>() = default;
};
