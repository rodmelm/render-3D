#ifndef RENDER_COLOR_PROCESSING_HPP
#define RENDER_COLOR_PROCESSING_HPP

#include "image_aos.hpp"
#include "image_soa.hpp"
#include "vector.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace render {

  class color_processing {
  public:
    [[nodiscard]] static double apply_gamma_correction(double value, double gamma) noexcept {
      value = std::clamp(value, 0.0, 1.0);
      return std::pow(value, 1.0 / gamma);
    }

    [[nodiscard]] static vector apply_gamma_correction(vector const & color,
                                                       double gamma) noexcept {
      double const r = apply_gamma_correction(color.x, gamma);
      double const g = apply_gamma_correction(color.y, gamma);
      double const b = apply_gamma_correction(color.z, gamma);
      return {r, g, b};
    }

    [[nodiscard]] static std::uint8_t to_uint8(double value) noexcept {
      value = std::clamp(value, 0.0, 1.0);
      return static_cast<std::uint8_t>(std::round(value * 255.0));
    }

    [[nodiscard]] static render::image_aos::pixel vector_to_pixel_aos(vector const & color,
                                                                      double gamma) noexcept {
      vector const corrected = apply_gamma_correction(color, gamma);
      return {to_uint8(corrected.x), to_uint8(corrected.y), to_uint8(corrected.z)};
    }

    [[nodiscard]] static render::image_soa::pixel vector_to_pixel_soa(vector const & color,
                                                                      double gamma) noexcept {
      vector const corrected = apply_gamma_correction(color, gamma);
      return {to_uint8(corrected.x), to_uint8(corrected.y), to_uint8(corrected.z)};
    }

    [[nodiscard]] static vector average_samples(std::vector<vector> const & samples) noexcept {
      if (samples.empty()) {
        return {0.0, 0.0, 0.0};
      }
      vector sum(0.0, 0.0, 0.0);
      for (auto const & sample : samples) {
        sum = sum + sample;
      }
      return sum * (1.0 / static_cast<double>(samples.size()));
    }
  };

}  // namespace render

#endif
