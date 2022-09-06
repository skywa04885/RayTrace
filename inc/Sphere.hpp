#pragma once

#include <cmath>
#include <limits>
#include <optional>

#include "Geometry.hpp"
#include "Material.hpp"
#include "Vector3D.hpp"

template <typename T> class Sphere : public Geometry<T> {
public:
  T radius;

public:
  Sphere<T>(const Vector3D<T> &position, const Material<T> &material,
            const T &radius)
      : Geometry<T>::Geometry(position, material), radius(radius) {}

  ~Sphere<T>() = default;

public:
  virtual std::optional<RayHitResult<T>> RayHit(const Ray<T> &ray) {
    // Reference: https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
    // The mathematics used here is directly yanked from this source...

    // Calculates the delta value, this will initially indicate if there are any
    // interceptions
    //  with this figure at all, if so, this will help calculate the points of
    //  interception.
    const T delta =
        std::pow(ray.direction.Dot(ray.origin.Subtract(this->position)), 2) -
        (std::pow(ray.origin.Subtract(this->position).Magnitude(), 2) -
         std::pow(this->radius, 2));

    // We'll assume we either have two intersections, or none. Because floats
    // are imperfect
    //  trying to compare to 0.0 will be retarded.
    if (delta < static_cast<T>(0.0)) {
      return std::nullopt;
    }

    // Calculates the base of the distance, then the alpha and beta distance.
    //  the alpha and beta distance, will be the distances from the origin at
    //  which the given ray intercepts the spherical figure.
    const T d =
        -std::abs(ray.direction.Dot(ray.origin.Subtract(this->position)));
    const T distanceAlpha = d + std::sqrt(delta);
    const T distanceBeta = d - std::sqrt(delta);
    const T distanceAlphaAbs = std::abs(distanceAlpha);
    const T distanceBetaAbs = std::abs(distanceBeta);

    // Uses the alpha intersection, if it's nearer by than the beta distance,
    // and if the alpha distance is not behind the camera.
    if (distanceAlphaAbs < distanceBetaAbs) {
      // Calculates the interception alpha vector.
      const Vector3D<T> interceptionAlphaVector =
          ray.origin.Add(ray.direction.Multiply(distanceAlpha));

      // Calculates the normal vector.
      const Vector3D<T> normalAlphaVector =
          interceptionAlphaVector.Subtract(this->position).Normalize();

      // Returns the ray hit result.
      return RayHitResult<T>(interceptionAlphaVector, normalAlphaVector,
                             distanceAlphaAbs);
    }

    // TODO; add check so it's not behind origin.

    // Uses the beta intersection if it's nearer by than the alpha intersection,
    // and it is not behind the origin.
    if (distanceBetaAbs >= distanceAlphaAbs) {
      // Calculates the interception beta vector.
      const Vector3D<T> interceptionBetaVector =
          ray.origin.Add(ray.direction.Multiply(distanceBeta));

      // Calculates the normal vector.
      const Vector3D<T> normalBetaVector =
          interceptionBetaVector.Subtract(this->position).Normalize();

      // Returns the result.
      return RayHitResult<T>(interceptionBetaVector, normalBetaVector,
                             distanceBetaAbs);
    }

    return std::nullopt;
  }
};
