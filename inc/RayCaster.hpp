#pragma once

#include "Camera.hpp"
#include "Geometry.hpp"
#include "GeometryRegister.hpp"
#include "PixelBuffer.hpp"
#include "Ray.hpp"
#include "Vector3D.hpp"
#include <cmath>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <thread>
#include <tuple>

template <typename T> class RayCaster {
public:
  std::optional<std::thread> thread;
  PixelBuffer &pixelBuffer;
  Camera<T> &camera;
  std::shared_ptr<GeometryRegister<T>> geometryRegister;
  const size_t castFrom, castTo;

public:
  RayCaster<T>(PixelBuffer &pixelBuffer, Camera<T> &camera,
               std::shared_ptr<GeometryRegister<T>> geometryRegister,
               const size_t &castFrom, const size_t &castTo) noexcept
      : thread(std::nullopt), pixelBuffer(pixelBuffer), camera(camera),
        geometryRegister(geometryRegister), castFrom(castFrom), castTo(castTo) {
  }

  RayCaster<T> &CreateThread() {
    if (this->thread.has_value()) {
      throw std::runtime_error("Thread is already created!");
    }

    this->thread = std::thread([&]() {
      for (size_t n = this->castFrom; n < this->castTo; ++n) {
        // Gets the ray we should cast.
        Ray<T> ray = this->camera.GetRayOrigin(n);

        // Keeps track of the previous objects reflectivities product.
        T reflectivityProduct = 1.0;

        // The actual color we will paint.
        std::optional<Vector3D<T>> color = std::nullopt;

        // Starts casting the ray.
        for (size_t rayNo = 0; rayNo < 8; ++rayNo) {
          // Casts the ray onto the geometry registry.. We will either get
          // an hit result, or nullopt.
          std::optional<
              std::tuple<std::shared_ptr<Geometry<T>>, RayHitResult<T>>>
              hitResult = geometryRegister->CastRay(ray);

          // Checks if we got an nullopt, if so, just draw the background.
          if (!hitResult.has_value()) {
            if (color.has_value()) {
              color = Vector3D<T>::Mix(1.0, *color, reflectivityProduct,
                                       Vector3D<T>(0.9, 0.9, 0.9));
            }
            break;
          }

          // Gets the result and the geometry.
          std::shared_ptr<Geometry<T>> geometry = std::get<0>(*hitResult);
          RayHitResult<T> result = std::get<1>(*hitResult);

          // Reflects the ray.
          ray = ray.Reflect(result.point, result.normal);

          // If the color has not been set yet, initialize it... Else perform an
          // mix with the existing one.
          color = color.has_value()
                      ? Vector3D<T>::Mix(1.0, *color, reflectivityProduct,
                                         geometry->material.color)
                      : geometry->material.color;

          // Checks the type of object we've hit.
          reflectivityProduct *= geometry->material.reflectivity;
        }

        // If the color is not present, make it the default environment color.
        if (!color.has_value()) {
          color = Vector3D<T>(0.9, 0.9, 0.9);
        }

        // Gets the viewport X and Y for pixel drawing.
        const size_t x = this->camera.ViewportX(n);
        const size_t y = this->camera.ViewportY(n);

        // Draws the pixel.
        this->pixelBuffer.PutPixel(x, y, color->x * 255.0, color->y * 255.0,
                                   color->z * 255.9, 255);
      }
    });

    return *this;
  }

  RayCaster<T> &JoinThread() {
    if (!this->thread.has_value()) {
      throw std::runtime_error("Thread does not exist yet!");
    }

    this->thread->join();

    return *this;
  }

  ~RayCaster<T>() noexcept = default;
};
