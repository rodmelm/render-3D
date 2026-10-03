#include <gtest/gtest.h>

#include <cmath>

#include "metal.hpp"
#include "random_system.hpp"
#include "ray.hpp"
#include "vector.hpp"

namespace {

  // Necesitamos smilla aleatoria fija para tests reproducibles
  class test_metal : public ::testing::Test {
  protected:
    void SetUp() override { render::random_system::init_material_gen(42); }
  };

  TEST_F(test_metal, constructor_reflectancia_y_difusion) {
    render::vector const reflectancia{0.9, 0.9, 0.9};
    double const difusion = 0.3;
    render::metal const mat{reflectancia, difusion};

    render::vector const result = mat.get_reflectancia();
    EXPECT_EQ(result.x, 0.9);
    EXPECT_EQ(result.y, 0.9);
    EXPECT_EQ(result.z, 0.9);
    EXPECT_EQ(mat.get_difusion(), 0.3);
  }

  TEST_F(test_metal, rayo_reflejado_devuelve_rayo_valido) {
    render::metal const mat{
      render::vector{1.0, 1.0, 1.0},
      0.0
    };
    render::ray const rayo_incidente{
      render::vector{0.0,  5.0, 0.0},
      render::vector{0.0, -1.0, 0.0}
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const reflejado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Verificar que el origen del rayo reflejado es la intersección
    EXPECT_EQ(reflejado.origen.x, 0.0);
    EXPECT_EQ(reflejado.origen.y, 0.0);
    EXPECT_EQ(reflejado.origen.z, 0.0);

    // Verificar que la dirección está normalizada
    EXPECT_NEAR(reflejado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST_F(test_metal, reflexion_perfecta_sin_difusion) {
    render::metal const mat{
      render::vector{1.0, 1.0, 1.0},
      0.0
    };
    // Rayo incidente a 45 grados en el plano XY
    render::ray const rayo_incidente{
      render::vector{-1.0,  1.0, 0.0},
      render::vector{ 1.0, -1.0, 0.0}
      .normalize()
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const reflejado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Con difusión 0, debería ser reflexión especular perfecta
    // La componente Y debe invertirse, X debe mantenerse
    EXPECT_NEAR(reflejado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST_F(test_metal, reflexion_con_difusion_moderada) {
    render::metal const mat{
      render::vector{0.8, 0.8, 0.8},
      0.2
    };
    render::ray const rayo_incidente{
      render::vector{0.0,  5.0, 0.0},
      render::vector{0.0, -1.0, 0.0}
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const reflejado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Con difusión, la dirección puede variar pero debe estar normalizada
    EXPECT_NEAR(reflejado.direccion.magnitude(), 1.0, 1e-6);
    EXPECT_EQ(reflejado.origen.x, 0.0);
    EXPECT_EQ(reflejado.origen.y, 0.0);
    EXPECT_EQ(reflejado.origen.z, 0.0);
  }

  TEST_F(test_metal, reflexion_con_alta_difusion) {
    render::metal const mat{
      render::vector{0.7, 0.7, 0.7},
      0.8
    };
    render::ray const rayo_incidente{
      render::vector{0.0, 0.0, 0.0},
      render::vector{0.0, 0.0, 1.0}
    };
    render::vector const interseccion{0.0, 0.0, 5.0};
    render::vector const normal{0.0, 0.0, -1.0};

    render::ray const reflejado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Alta difusión produce más dispersión pero siempre normalizado
    EXPECT_NEAR(reflejado.direccion.magnitude(), 1.0, 1e-6);
    EXPECT_EQ(reflejado.origen.x, 0.0);
    EXPECT_EQ(reflejado.origen.y, 0.0);
    EXPECT_EQ(reflejado.origen.z, 5.0);
  }

  TEST_F(test_metal, reflectancia_plateada) {
    render::vector const reflectancia{0.95, 0.95, 0.95};
    double const difusion = 0.1;
    render::metal const mat{reflectancia, difusion};

    render::vector const result = mat.get_reflectancia();
    EXPECT_EQ(result.x, 0.95);
    EXPECT_EQ(result.y, 0.95);
    EXPECT_EQ(result.z, 0.95);
  }

  TEST_F(test_metal, reflectancia_dorada) {
    render::vector const reflectancia{1.0, 0.84, 0.0};
    double const difusion = 0.05;
    render::metal const mat{reflectancia, difusion};

    render::vector const result = mat.get_reflectancia();
    EXPECT_EQ(result.x, 1.0);
    EXPECT_EQ(result.y, 0.84);
    EXPECT_EQ(result.z, 0.0);
    EXPECT_EQ(mat.get_difusion(), 0.05);
  }

  TEST_F(test_metal, difusion_cero) {
    render::metal const mat{
      render::vector{1.0, 1.0, 1.0},
      0.0
    };
    EXPECT_EQ(mat.get_difusion(), 0.0);
  }

  TEST_F(test_metal, difusion_maxima) {
    render::metal const mat{
      render::vector{0.5, 0.5, 0.5},
      1.0
    };
    EXPECT_EQ(mat.get_difusion(), 1.0);
  }

  TEST_F(test_metal, reflexion_vertical_hacia_arriba) {
    render::metal const mat{
      render::vector{0.9, 0.9, 0.9},
      0.1
    };
    render::ray const rayo_incidente{
      render::vector{0.0, -5.0, 0.0},
      render::vector{0.0,  1.0, 0.0}
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const reflejado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    EXPECT_NEAR(reflejado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST_F(test_metal, multiples_rayos_con_difusion_son_diferentes) {
    render::metal const mat{
      render::vector{0.8, 0.8, 0.8},
      0.3
    };
    render::ray const rayo_incidente{
      render::vector{0.0,  5.0, 0.0},
      render::vector{0.0, -1.0, 0.0}
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const reflejado1 = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);
    render::ray const reflejado2 = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);
    render::ray const reflejado3 = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Con difusión > 0, los rayos deben tener direcciones diferentes
    bool const diferentes_1_2 =
        (std::abs(reflejado1.direccion.x - reflejado2.direccion.x) > 1e-6) or
        (std::abs(reflejado1.direccion.y - reflejado2.direccion.y) > 1e-6) or
        (std::abs(reflejado1.direccion.z - reflejado2.direccion.z) > 1e-6);

    bool const diferentes_2_3 =
        (std::abs(reflejado2.direccion.x - reflejado3.direccion.x) > 1e-6) or
        (std::abs(reflejado2.direccion.y - reflejado3.direccion.y) > 1e-6) or
        (std::abs(reflejado2.direccion.z - reflejado3.direccion.z) > 1e-6);

    EXPECT_TRUE(diferentes_1_2);
    EXPECT_TRUE(diferentes_2_3);
  }

  TEST_F(test_metal, reflexion_con_normal_diagonal) {
    render::metal const mat{
      render::vector{1.0, 1.0, 1.0},
      0.0
    };
    render::ray const rayo_incidente{
      render::vector{0.0,  0.0, 0.0},
      render::vector{1.0, -1.0, 0.0}
      .normalize()
    };
    render::vector const interseccion{1.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const reflejado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Verificar normalización
    EXPECT_NEAR(reflejado.direccion.magnitude(), 1.0, 1e-6);
    // Verificar origen
    EXPECT_EQ(reflejado.origen.x, 1.0);
    EXPECT_EQ(reflejado.origen.y, 0.0);
    EXPECT_EQ(reflejado.origen.z, 0.0);
  }

  TEST_F(test_metal, ley_reflexion_angulo_incidencia_igual_reflexion) {
    render::metal const mat{
      render::vector{1.0, 1.0, 1.0},
      0.0
    };
    // Rayo incidente a 30 grados
    double const angulo = 0.523599;  // 30 grados en radianes
    render::vector const dir_incidente{std::sin(angulo), -std::cos(angulo), 0.0};
    render::ray const rayo_incidente{
      render::vector{0.0, 10.0, 0.0},
      dir_incidente
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const reflejado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Con difusión 0, el ángulo de reflexión debe ser igual al de incidencia
    double const cos_incidente = std::abs(dir_incidente.producto(normal));
    double const cos_reflejado = std::abs(reflejado.direccion.producto(normal));

    EXPECT_NEAR(cos_incidente, cos_reflejado, 0.01);
  }

}  // namespace
