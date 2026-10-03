#include <gtest/gtest.h>

#include "ray.hpp"
#include "vector.hpp"

namespace {

  TEST(test_ray, constructor) {
    render::vector const origen{0.0, 0.0, 0.0};
    render::vector const direccion{1.0, 0.0, 0.0};
    render::ray const rayo{origen, direccion};

    EXPECT_EQ(rayo.origen.x, 0.0);
    EXPECT_EQ(rayo.origen.y, 0.0);
    EXPECT_EQ(rayo.origen.z, 0.0);
    EXPECT_EQ(rayo.direccion.x, 1.0);
    EXPECT_EQ(rayo.direccion.y, 0.0);
    EXPECT_EQ(rayo.direccion.z, 0.0);
  }

  TEST(test_ray, punto_at_zero) {
    render::vector const origen{1.0, 2.0, 3.0};
    render::vector const direccion{0.0, 0.0, 1.0};
    render::ray const rayo{origen, direccion};

    render::vector const punto = rayo.punto_at(0.0);
    EXPECT_EQ(punto.x, 1.0);
    EXPECT_EQ(punto.y, 2.0);
    EXPECT_EQ(punto.z, 3.0);
  }

  TEST(test_ray, punto_at_positive) {
    render::vector const origen{0.0, 0.0, 0.0};
    render::vector const direccion{1.0, 0.0, 0.0};
    render::ray const rayo{origen, direccion};

    render::vector const punto = rayo.punto_at(5.0);
    EXPECT_EQ(punto.x, 5.0);
    EXPECT_EQ(punto.y, 0.0);
    EXPECT_EQ(punto.z, 0.0);
  }

  TEST(test_ray, punto_at_negative) {
    render::vector const origen{0.0, 0.0, 0.0};
    render::vector const direccion{1.0, 0.0, 0.0};
    render::ray const rayo{origen, direccion};

    render::vector const punto = rayo.punto_at(-3.0);
    EXPECT_EQ(punto.x, -3.0);
    EXPECT_EQ(punto.y, 0.0);
    EXPECT_EQ(punto.z, 0.0);
  }

  TEST(test_ray, punto_at_diagonal) {
    render::vector const origen{1.0, 1.0, 1.0};
    render::vector const direccion{1.0, 1.0, 1.0};
    render::ray const rayo{origen, direccion};

    render::vector const punto = rayo.punto_at(2.0);
    EXPECT_EQ(punto.x, 3.0);
    EXPECT_EQ(punto.y, 3.0);
    EXPECT_EQ(punto.z, 3.0);
  }

  TEST(test_ray, punto_at_multiple_components) {
    render::vector const origen{2.0, 3.0, 4.0};
    render::vector const direccion{1.0, -1.0, 2.0};
    render::ray const rayo{origen, direccion};

    render::vector const punto = rayo.punto_at(3.0);
    EXPECT_EQ(punto.x, 5.0);
    EXPECT_EQ(punto.y, 0.0);
    EXPECT_EQ(punto.z, 10.0);
  }

  TEST(test_ray, punto_at_fractional_t) {
    render::vector const origen{0.0, 0.0, 0.0};
    render::vector const direccion{2.0, 4.0, 6.0};
    render::ray const rayo{origen, direccion};

    render::vector const punto = rayo.punto_at(0.5);
    EXPECT_EQ(punto.x, 1.0);
    EXPECT_EQ(punto.y, 2.0);
    EXPECT_EQ(punto.z, 3.0);
  }

  TEST(test_ray, getorigen) {
    render::vector const origen{10.0, 20.0, 30.0};
    render::vector const direccion{1.0, 0.0, 0.0};
    render::ray const rayo{origen, direccion};

    render::vector const result = rayo.origen;
    EXPECT_EQ(result.x, 10.0);
    EXPECT_EQ(result.y, 20.0);
    EXPECT_EQ(result.z, 30.0);
  }

  TEST(test_ray, getdireccion) {
    render::vector const origen{0.0, 0.0, 0.0};
    render::vector const direccion{0.5, 0.5, 0.5};
    render::ray const rayo{origen, direccion};

    render::vector const result = rayo.direccion;
    EXPECT_EQ(result.x, 0.5);
    EXPECT_EQ(result.y, 0.5);
    EXPECT_EQ(result.z, 0.5);
  }

  TEST(test_ray, punto_at_with_zero_direction) {
    render::vector const origen{5.0, 5.0, 5.0};
    render::vector const direccion{0.0, 0.0, 0.0};
    render::ray const rayo{origen, direccion};

    render::vector const punto = rayo.punto_at(10.0);
    EXPECT_EQ(punto.x, 5.0);
    EXPECT_EQ(punto.y, 5.0);
    EXPECT_EQ(punto.z, 5.0);
  }

  TEST(test_ray, punto_at_negative_origin) {
    render::vector const origen{-3.0, -2.0, -1.0};
    render::vector const direccion{1.0, 1.0, 1.0};
    render::ray const rayo{origen, direccion};

    render::vector const punto = rayo.punto_at(4.0);
    EXPECT_EQ(punto.x, 1.0);
    EXPECT_EQ(punto.y, 2.0);
    EXPECT_EQ(punto.z, 3.0);
  }

}  // namespace
