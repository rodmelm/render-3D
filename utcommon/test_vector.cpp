#include <gtest/gtest.h>

#include "vector.hpp"

namespace {

  TEST(test_vector, magnitude_zero) {
    render::vector const vec{0.0, 0.0, 0.0};
    EXPECT_EQ(vec.magnitude(), 0.0);
  }

  TEST(test_vector, magnitude_positive) {
    render::vector const vec{3.0, 4.0, 0.0};
    EXPECT_EQ(vec.magnitude(), 5.0);
  }

  TEST(test_vector, normalize_nonzero) {
    render::vector const vec{3.0, 0.0, 4.0};
    render::vector const norm_vec   = vec.normalize();
    double const expected_magnitude = 1.0;
    EXPECT_NEAR(norm_vec.magnitude(), expected_magnitude, 1e-9);
  }

  TEST(test_vector, normalize_zero) {
    render::vector const vec{0.0, 0.0, 0.0};
    render::vector const norm_vec = vec.normalize();
    EXPECT_EQ(norm_vec.x, 0.0);
    EXPECT_EQ(norm_vec.y, 0.0);
    EXPECT_EQ(norm_vec.z, 0.0);
  }

  TEST(test_vector, addition) {
    render::vector const vec1{1.0, 2.0, 3.0};
    render::vector const vec2{4.0, 5.0, 6.0};
    render::vector const result = vec1 + vec2;
    EXPECT_EQ(result.x, 5.0);
    EXPECT_EQ(result.y, 7.0);
    EXPECT_EQ(result.z, 9.0);
  }

  TEST(test_vector, subtraction) {
    render::vector const vec1{5.0, 7.0, 9.0};
    render::vector const vec2{1.0, 2.0, 3.0};
    render::vector const result = vec1 - vec2;
    EXPECT_EQ(result.x, 4.0);
    EXPECT_EQ(result.y, 5.0);
    EXPECT_EQ(result.z, 6.0);
  }

  TEST(test_vector, scalar_multiplication) {
    render::vector const vec{2.0, 3.0, 4.0};
    render::vector const result = vec * 2.0;
    EXPECT_EQ(result.x, 4.0);
    EXPECT_EQ(result.y, 6.0);
    EXPECT_EQ(result.z, 8.0);
  }

  TEST(test_vector, scalar_multiplication_zero) {
    render::vector const vec{2.0, 3.0, 4.0};
    render::vector const result = vec * 0.0;
    EXPECT_EQ(result.x, 0.0);
    EXPECT_EQ(result.y, 0.0);
    EXPECT_EQ(result.z, 0.0);
  }

  TEST(test_vector, dot_product_perpendicular) {
    render::vector const vec1{1.0, 0.0, 0.0};
    render::vector const vec2{0.0, 1.0, 0.0};
    double const result = vec1.producto(vec2);
    EXPECT_EQ(result, 0.0);
  }

  TEST(test_vector, dot_product_parallel) {
    render::vector const vec1{2.0, 0.0, 0.0};
    render::vector const vec2{3.0, 0.0, 0.0};
    double const result = vec1.producto(vec2);
    EXPECT_EQ(result, 6.0);
  }

  TEST(test_vector, dot_product_general) {
    render::vector const vec1{1.0, 2.0, 3.0};
    render::vector const vec2{4.0, 5.0, 6.0};
    double const result = vec1.producto(vec2);
    EXPECT_EQ(result, 32.0);
  }

  TEST(test_vector, cross_product_i_j) {
    render::vector const vec1{1.0, 0.0, 0.0};
    render::vector const vec2{0.0, 1.0, 0.0};
    render::vector const result = vec1.vectorial(vec2);
    EXPECT_EQ(result.x, 0.0);
    EXPECT_EQ(result.y, 0.0);
    EXPECT_EQ(result.z, 1.0);
  }

  TEST(test_vector, cross_product_parallel) {
    render::vector const vec1{2.0, 0.0, 0.0};
    render::vector const vec2{4.0, 0.0, 0.0};
    render::vector const result = vec1.vectorial(vec2);
    EXPECT_EQ(result.x, 0.0);
    EXPECT_EQ(result.y, 0.0);
    EXPECT_EQ(result.z, 0.0);
  }

  TEST(test_vector, cross_product_general) {
    render::vector const vec1{1.0, 2.0, 3.0};
    render::vector const vec2{4.0, 5.0, 6.0};
    render::vector const result = vec1.vectorial(vec2);
    EXPECT_EQ(result.x, -3.0);
    EXPECT_EQ(result.y, 6.0);
    EXPECT_EQ(result.z, -3.0);
  }

  TEST(test_vector, getters) {
    render::vector const vec{1.5, 2.5, 3.5};
    EXPECT_EQ(vec.x, 1.5);
    EXPECT_EQ(vec.y, 2.5);
    EXPECT_EQ(vec.z, 3.5);
  }

  TEST(test_vector, magnitude_negative_components) {
    render::vector const vec{-3.0, -4.0, 0.0};
    EXPECT_EQ(vec.magnitude(), 5.0);
  }

  TEST(test_vector, normalize_negative_components) {
    render::vector const vec{-3.0, -4.0, 0.0};
    render::vector const norm_vec = vec.normalize();
    EXPECT_NEAR(norm_vec.magnitude(), 1.0, 1e-9);
    EXPECT_NEAR(norm_vec.x, -0.6, 1e-9);
    EXPECT_NEAR(norm_vec.y, -0.8, 1e-9);
  }

}  // namespace
