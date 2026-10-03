#ifndef RENDER_VECTOR_HPP
#define RENDER_VECTOR_HPP

#include <cmath>

namespace render {

  class vector {
  public:
    double x, y, z;

    vector(double cx, double cy, double cz) : x{cx}, y{cy}, z{cz} { }

    // Vamos a necesitar que el vector realice operaciones básicas
    // Pre-calcular magnitud al cuadrado cuando sea posible
    [[nodiscard]] constexpr double magnitude_squared() const noexcept {
      return x * x + y * y + z * z;
    }

    // Evitar sqrt cuando sea posible, usar magnitude_squared
    [[nodiscard]] double magnitude() const noexcept { return std::sqrt(magnitude_squared()); }

    // Normalización más rápida con reciprocal
    [[nodiscard]] vector normalize() const noexcept {
      double const mag_sq = magnitude_squared();
      if (mag_sq == 0.0) [[unlikely]] {
        return vector{0.0, 0.0, 0.0};
      }
      double const inv_mag = 1.0 / std::sqrt(mag_sq);
      return vector{x * inv_mag, y * inv_mag, z * inv_mag};
    }

    // Operadores inlined y constexpr
    [[nodiscard]] constexpr vector operator+(vector const & other) const noexcept {
      return {x + other.x, y + other.y, z + other.z};
    }

    [[nodiscard]] constexpr vector operator-(vector const & other) const noexcept {
      return {x - other.x, y - other.y, z - other.z};
    }

    [[nodiscard]] constexpr vector operator*(double scalar) const noexcept {
      return {x * scalar, y * scalar, z * scalar};
    }

    // Producto escalar sin overhead de función
    [[nodiscard]] constexpr double producto(vector const & other) const noexcept {
      return x * other.x + y * other.y + z * other.z;
    }

    // Producto vectorial optimizado
    [[nodiscard]] constexpr vector vectorial(vector const & other) const noexcept {
      return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
    }
  };

}  // namespace render

#endif
