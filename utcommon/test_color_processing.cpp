#include <gtest/gtest.h>

#include "color_processing.hpp"
#include "image_aos.hpp"
#include "image_soa.hpp"
#include "vector.hpp"
#include <cmath>
#include <vector>

namespace {

  // Tests para apply_gamma_correction (double)
  TEST(test_color_processing, aplicar_correccion_gamma_double) {
    double const result = render::color_processing::apply_gamma_correction(0.5, 1.0);
    EXPECT_NEAR(result, 0.5, 1e-9);
  }

  TEST(test_color_processing, aplicar_correccion_gamma_double_2_2) {
    double const result   = render::color_processing::apply_gamma_correction(0.5, 2.2);
    double const expected = std::pow(0.5, 1.0 / 2.2);
    EXPECT_NEAR(result, expected, 1e-9);
  }

  TEST(test_color_processing, aplicar_correccion_gamma_double_cero) {
    double const result = render::color_processing::apply_gamma_correction(0.0, 2.2);
    EXPECT_NEAR(result, 0.0, 1e-9);
  }

  TEST(test_color_processing, aplicar_correccion_gamma_double_uno) {
    double const result = render::color_processing::apply_gamma_correction(1.0, 2.2);
    EXPECT_NEAR(result, 1.0, 1e-9);
  }

  TEST(test_color_processing, aplicar_correccion_gamma_double_por_debajo_cero) {
    double const result = render::color_processing::apply_gamma_correction(-0.5, 2.2);
    EXPECT_NEAR(result, 0.0, 1e-9);
  }

  TEST(test_color_processing, aplicar_correccion_gamma_double_por_encima_uno) {
    double const result = render::color_processing::apply_gamma_correction(1.5, 2.2);
    EXPECT_NEAR(result, 1.0, 1e-9);
  }

  TEST(test_color_processing, aplicar_correccion_gamma_double_gamma_1) {
    double const result = render::color_processing::apply_gamma_correction(0.75, 1.0);
    EXPECT_NEAR(result, 0.75, 1e-9);
  }

  TEST(test_color_processing, aplicar_correccion_gamma_double_gamma_2) {
    double const result   = render::color_processing::apply_gamma_correction(0.25, 2.0);
    double const expected = std::pow(0.25, 0.5);
    EXPECT_NEAR(result, expected, 1e-9);
  }

  // Tests para apply_gamma_correction (vector)
  TEST(test_color_processing, aplicar_correccion_gamma_vector) {
    render::vector const color{0.5, 0.5, 0.5};
    render::vector const result = render::color_processing::apply_gamma_correction(color, 1.0);
    EXPECT_NEAR(result.x, 0.5, 1e-9);
    EXPECT_NEAR(result.y, 0.5, 1e-9);
    EXPECT_NEAR(result.z, 0.5, 1e-9);
  }

  TEST(test_color_processing, aplicar_correccion_gamma_vector_diferentes_valores) {
    render::vector const color{0.25, 0.5, 0.75};
    render::vector const result = render::color_processing::apply_gamma_correction(color, 2.2);
    EXPECT_NEAR(result.x, std::pow(0.25, 1.0 / 2.2), 1e-9);
    EXPECT_NEAR(result.y, std::pow(0.5, 1.0 / 2.2), 1e-9);
    EXPECT_NEAR(result.z, std::pow(0.75, 1.0 / 2.2), 1e-9);
  }

  TEST(test_color_processing, aplicar_correccion_gamma_vector_negro) {
    render::vector const color{0.0, 0.0, 0.0};
    render::vector const result = render::color_processing::apply_gamma_correction(color, 2.2);
    EXPECT_NEAR(result.x, 0.0, 1e-9);
    EXPECT_NEAR(result.y, 0.0, 1e-9);
    EXPECT_NEAR(result.z, 0.0, 1e-9);
  }

  TEST(test_color_processing, aplicar_correccion_gamma_vector_blanco) {
    render::vector const color{1.0, 1.0, 1.0};
    render::vector const result = render::color_processing::apply_gamma_correction(color, 2.2);
    EXPECT_NEAR(result.x, 1.0, 1e-9);
    EXPECT_NEAR(result.y, 1.0, 1e-9);
    EXPECT_NEAR(result.z, 1.0, 1e-9);
  }

  TEST(test_color_processing, aplicar_correccion_gamma_vector_sobrepasado) {
    render::vector const color{-0.5, 1.5, 0.5};
    render::vector const result = render::color_processing::apply_gamma_correction(color, 2.2);
    EXPECT_NEAR(result.x, 0.0, 1e-9);
    EXPECT_NEAR(result.z, std::pow(0.5, 1.0 / 2.2), 1e-9);
  }

  // Tests para to_uint8
  TEST(test_color_processing, to_uint8_cero) {
    std::uint8_t const result = render::color_processing::to_uint8(0.0);
    EXPECT_EQ(result, 0);
  }

  TEST(test_color_processing, to_uint8_uno) {
    std::uint8_t const result = render::color_processing::to_uint8(1.0);
    EXPECT_EQ(result, 255);
  }

  TEST(test_color_processing, to_uint8_medio) {
    std::uint8_t const result = render::color_processing::to_uint8(0.5);
    EXPECT_EQ(result, 128);
  }

  TEST(test_color_processing, to_uint8_sobrepasado_debajo) {
    std::uint8_t const result = render::color_processing::to_uint8(-0.5);
    EXPECT_EQ(result, 0);
  }

  TEST(test_color_processing, to_uint8_sobrepasado_encima) {
    std::uint8_t const result = render::color_processing::to_uint8(1.5);
    EXPECT_EQ(result, 255);
  }

  TEST(test_color_processing, to_uint8_redondear_abajo) {
    std::uint8_t const result = render::color_processing::to_uint8(0.1);
    EXPECT_EQ(result, 26);  // 0.1 * 255 = 25.5, rounds to 26
  }

  TEST(test_color_processing, to_uint8_redondear_arriba) {
    std::uint8_t const result = render::color_processing::to_uint8(0.9);
    EXPECT_EQ(result, 230);  // 0.9 * 255 = 229.5, rounds to 230
  }

  TEST(test_color_processing, to_uint8_valor_exacto) {
    std::uint8_t const result = render::color_processing::to_uint8(100.0 / 255.0);
    EXPECT_EQ(result, 100);
  }

  TEST(test_color_processing, to_uint8_varios_valores) {
    EXPECT_EQ(render::color_processing::to_uint8(0.25), 64);
    EXPECT_EQ(render::color_processing::to_uint8(0.75), 191);
    EXPECT_EQ(render::color_processing::to_uint8(0.333), 85);
  }

  // Tests para vector_a_pixel_aos
  TEST(test_color_processing, vector_a_pixel_aos_blanco) {
    render::vector const color{1.0, 1.0, 1.0};
    render::image_aos::pixel const result =
        render::color_processing::vector_to_pixel_aos(color, 1.0);
    EXPECT_EQ(result.r, 255);
    EXPECT_EQ(result.g, 255);
    EXPECT_EQ(result.b, 255);
  }

  TEST(test_color_processing, vector_a_pixel_aos_negro) {
    render::vector const color{0.0, 0.0, 0.0};
    render::image_aos::pixel const result =
        render::color_processing::vector_to_pixel_aos(color, 1.0);
    EXPECT_EQ(result.r, 0);
    EXPECT_EQ(result.g, 0);
    EXPECT_EQ(result.b, 0);
  }

  TEST(test_color_processing, vector_a_pixel_aos_rojo) {
    render::vector const color{1.0, 0.0, 0.0};
    render::image_aos::pixel const result =
        render::color_processing::vector_to_pixel_aos(color, 1.0);
    EXPECT_EQ(result.r, 255);
    EXPECT_EQ(result.g, 0);
    EXPECT_EQ(result.b, 0);
  }

  TEST(test_color_processing, vector_a_pixel_aos_con_gamma) {
    render::vector const color{0.5, 0.5, 0.5};
    render::image_aos::pixel const result =
        render::color_processing::vector_to_pixel_aos(color, 2.2);
    double const expected_value = std::pow(0.5, 1.0 / 2.2);
    auto const expected_uint8   = static_cast<std::uint8_t>(std::round(expected_value * 255.0));
    EXPECT_EQ(result.r, expected_uint8);
    EXPECT_EQ(result.g, expected_uint8);
    EXPECT_EQ(result.b, expected_uint8);
  }

  TEST(test_color_processing, vector_a_pixel_aos_diferentes_valores) {
    render::vector const color{0.2, 0.5, 0.8};
    render::image_aos::pixel const result =
        render::color_processing::vector_to_pixel_aos(color, 1.0);
    EXPECT_EQ(result.r, 51);   // 0.2 * 255 = 51
    EXPECT_EQ(result.g, 128);  // 0.5 * 255 = 127.5 -> 128
    EXPECT_EQ(result.b, 204);  // 0.8 * 255 = 204
  }

  // Tests para vector_a_pixel_soa
  TEST(test_color_processing, vector_a_pixel_soa_blanco) {
    render::vector const color{1.0, 1.0, 1.0};
    render::image_soa::pixel const result =
        render::color_processing::vector_to_pixel_soa(color, 1.0);
    EXPECT_EQ(result.r, 255);
    EXPECT_EQ(result.g, 255);
    EXPECT_EQ(result.b, 255);
  }

  TEST(test_color_processing, vector_a_pixel_soa_negro) {
    render::vector const color{0.0, 0.0, 0.0};
    render::image_soa::pixel const result =
        render::color_processing::vector_to_pixel_soa(color, 1.0);
    EXPECT_EQ(result.r, 0);
    EXPECT_EQ(result.g, 0);
    EXPECT_EQ(result.b, 0);
  }

  TEST(test_color_processing, vector_a_pixel_soa_verde) {
    render::vector const color{0.0, 1.0, 0.0};
    render::image_soa::pixel const result =
        render::color_processing::vector_to_pixel_soa(color, 1.0);
    EXPECT_EQ(result.r, 0);
    EXPECT_EQ(result.g, 255);
    EXPECT_EQ(result.b, 0);
  }

  TEST(test_color_processing, vector_a_pixel_soa_con_gamma) {
    render::vector const color{0.5, 0.5, 0.5};
    render::image_soa::pixel const result =
        render::color_processing::vector_to_pixel_soa(color, 2.2);
    double const expected_value = std::pow(0.5, 1.0 / 2.2);
    auto const expected_uint8   = static_cast<std::uint8_t>(std::round(expected_value * 255.0));
    EXPECT_EQ(result.r, expected_uint8);
    EXPECT_EQ(result.g, expected_uint8);
    EXPECT_EQ(result.b, expected_uint8);
  }

  TEST(test_color_processing, vector_a_pixel_soa_diferentes_valores) {
    render::vector const color{0.3, 0.6, 0.9};
    render::image_soa::pixel const result =
        render::color_processing::vector_to_pixel_soa(color, 1.0);
    EXPECT_EQ(result.r, 77);   // 0.3 * 255 = 76.5 -> 77
    EXPECT_EQ(result.g, 153);  // 0.6 * 255 = 153
    EXPECT_EQ(result.b, 230);  // 0.9 * 255 = 229.5 -> 230
  }

  // Tests para average_samples
  TEST(test_color_processing, average_samples_vacio) {
    std::vector<render::vector> samples;
    render::vector const result = render::color_processing::average_samples(samples);
    EXPECT_EQ(result.x, 0.0);
    EXPECT_EQ(result.y, 0.0);
    EXPECT_EQ(result.z, 0.0);
  }

  TEST(test_color_processing, average_samples_una_sola_muestra) {
    std::vector<render::vector> samples;
    samples.emplace_back(0.5, 0.6, 0.7);
    render::vector const result = render::color_processing::average_samples(samples);
    EXPECT_NEAR(result.x, 0.5, 1e-9);
    EXPECT_NEAR(result.y, 0.6, 1e-9);
    EXPECT_NEAR(result.z, 0.7, 1e-9);
  }

  TEST(test_color_processing, average_samples_dos_identicos) {
    std::vector<render::vector> samples;
    samples.emplace_back(0.4, 0.5, 0.6);
    samples.emplace_back(0.4, 0.5, 0.6);
    render::vector const result = render::color_processing::average_samples(samples);
    EXPECT_NEAR(result.x, 0.4, 1e-9);
    EXPECT_NEAR(result.y, 0.5, 1e-9);
    EXPECT_NEAR(result.z, 0.6, 1e-9);
  }

  TEST(test_color_processing, average_samples_dos_diferentes) {
    std::vector<render::vector> samples;
    samples.emplace_back(0.0, 0.0, 0.0);
    samples.emplace_back(1.0, 1.0, 1.0);
    render::vector const result = render::color_processing::average_samples(samples);
    EXPECT_NEAR(result.x, 0.5, 1e-9);
    EXPECT_NEAR(result.y, 0.5, 1e-9);
    EXPECT_NEAR(result.z, 0.5, 1e-9);
  }

  TEST(test_color_processing, average_samples_multiples) {
    std::vector<render::vector> samples;
    samples.emplace_back(0.1, 0.2, 0.3);
    samples.emplace_back(0.2, 0.4, 0.6);
    samples.emplace_back(0.3, 0.6, 0.9);
    samples.emplace_back(0.4, 0.8, 1.2);
    render::vector const result = render::color_processing::average_samples(samples);
    EXPECT_NEAR(result.x, 0.25, 1e-9);
    EXPECT_NEAR(result.y, 0.5, 1e-9);
    EXPECT_NEAR(result.z, 0.75, 1e-9);
  }

  TEST(test_color_processing, average_samples_todo_negro) {
    std::vector<render::vector> samples;
    samples.emplace_back(0.0, 0.0, 0.0);
    samples.emplace_back(0.0, 0.0, 0.0);
    samples.emplace_back(0.0, 0.0, 0.0);
    render::vector const result = render::color_processing::average_samples(samples);
    EXPECT_NEAR(result.x, 0.0, 1e-9);
    EXPECT_NEAR(result.y, 0.0, 1e-9);
    EXPECT_NEAR(result.z, 0.0, 1e-9);
  }

  TEST(test_color_processing, average_samples_todo_blanco) {
    std::vector<render::vector> samples;
    samples.emplace_back(1.0, 1.0, 1.0);
    samples.emplace_back(1.0, 1.0, 1.0);
    samples.emplace_back(1.0, 1.0, 1.0);
    render::vector const result = render::color_processing::average_samples(samples);
    EXPECT_NEAR(result.x, 1.0, 1e-9);
    EXPECT_NEAR(result.y, 1.0, 1e-9);
    EXPECT_NEAR(result.z, 1.0, 1e-9);
  }

  TEST(test_color_processing, average_samples_numero_grande) {
    std::vector<render::vector> samples;
    samples.reserve(100);
    for (int i = 0; i < 100; ++i) {
      samples.emplace_back(0.5, 0.5, 0.5);
    }
    render::vector const result = render::color_processing::average_samples(samples);
    EXPECT_NEAR(result.x, 0.5, 1e-9);
    EXPECT_NEAR(result.y, 0.5, 1e-9);
    EXPECT_NEAR(result.z, 0.5, 1e-9);
  }

  TEST(test_color_processing, average_samples_diferentes_valores) {
    std::vector<render::vector> samples;
    samples.emplace_back(1.0, 0.0, 0.0);
    samples.emplace_back(0.0, 1.0, 0.0);
    samples.emplace_back(0.0, 0.0, 1.0);
    render::vector const result = render::color_processing::average_samples(samples);
    EXPECT_NEAR(result.x, 1.0 / 3.0, 1e-9);
    EXPECT_NEAR(result.y, 1.0 / 3.0, 1e-9);
    EXPECT_NEAR(result.z, 1.0 / 3.0, 1e-9);
  }

  // Tests de integración final
  TEST(test_color_processing, aplicacion_total_gamma_correction) {
    render::vector const color{0.5, 0.5, 0.5};
    render::image_soa::pixel const pixel_soa =
        render::color_processing::vector_to_pixel_soa(color, 2.2);
    render::image_aos::pixel const pixel_aos =
        render::color_processing::vector_to_pixel_aos(color, 2.2);

    EXPECT_EQ(pixel_soa.r, pixel_aos.r);
    EXPECT_EQ(pixel_soa.g, pixel_aos.g);
    EXPECT_EQ(pixel_soa.b, pixel_aos.b);
  }

  TEST(test_color_processing, consistencia_conversion_soa_aos_) {
    std::vector<render::vector> test_colors = {
      {0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0},
      {0.5, 0.5, 0.5},
      {1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.3, 0.6, 0.9},
      {0.1, 0.5, 0.8}
    };

    for (auto const & color : test_colors) {
      render::image_soa::pixel const pixel_soa =
          render::color_processing::vector_to_pixel_soa(color, 2.2);
      render::image_aos::pixel const pixel_aos =
          render::color_processing::vector_to_pixel_aos(color, 2.2);

      EXPECT_EQ(pixel_soa.r, pixel_aos.r);
      EXPECT_EQ(pixel_soa.g, pixel_aos.g);
      EXPECT_EQ(pixel_soa.b, pixel_aos.b);
    }
  }

}  // namespace
