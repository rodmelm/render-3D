#include <gtest/gtest.h>

#include <cmath>

#include "matte.hpp"
#include "random_system.hpp"
#include "ray.hpp"
#include "vector.hpp"

namespace {

  // Necesitamos semilla aleatoria fija para tests reproducibles
  class test_matte : public ::testing::Test {
  protected:
    void SetUp() override { render::random_system::init_material_gen(42); }
  };

  TEST_F(test_matte, constructor_reflectancia) {
    render::vector const reflectancia{0.5, 0.6, 0.7};
    render::matte const mat{reflectancia};

    render::vector const result = mat.get_reflectancia();
    EXPECT_EQ(result.x, 0.5);
    EXPECT_EQ(result.y, 0.6);
    EXPECT_EQ(result.z, 0.7);
  }

  TEST_F(test_matte, rayo_reflejado_devuelve_rayo_valido) {
    render::matte const mat{
      render::vector{1.0, 1.0, 1.0}
    };
    render::ray const rayo_incidente{
      render::vector{0.0, 0.0, 0.0},
      render::vector{0.0, 0.0, 1.0}
    };
    render::vector const interseccion{0.0, 0.0, 5.0};
    render::vector const normal{0.0, 0.0, -1.0};

    render::ray const reflejado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Verificar que el origen del rayo reflejado es la intersección
    EXPECT_EQ(reflejado.origen.x, 0.0);
    EXPECT_EQ(reflejado.origen.y, 0.0);
    EXPECT_EQ(reflejado.origen.z, 5.0);

    // Verificar que la dirección está normalizada
    EXPECT_NEAR(reflejado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST_F(test_matte, rayo_reflejado_direccion_en_hemisferio_correcto) {
    render::matte const mat{
      render::vector{0.8, 0.8, 0.8}
    };
    render::ray const rayo_incidente{
      render::vector{0.0, 0.0, 0.0},
      render::vector{0.0, 1.0, 0.0}
    };
    render::vector const interseccion{0.0, 1.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};  // Normal apuntando hacia arriba

    // Generar múltiples rayos y verificar que están en el hemisferio correcto
    int rayos_en_hemisferio_correcto = 0;
    int const num_samples            = 100;

    for (int i = 0; i < num_samples; ++i) {
      render::ray const reflejado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

      // El producto escalar con la normal debe ser >= 0 (mismo hemisferio)
      double const dot = reflejado.direccion.producto(normal);
      if (dot >= -1e-6) {  // Pequeña tolerancia por errores numéricos
        ++rayos_en_hemisferio_correcto;
      }
    }

    // La mayoría de los rayos deben estar en el hemisferio correcto
    EXPECT_GT(rayos_en_hemisferio_correcto, num_samples * 0.8);
  }

  TEST_F(test_matte, caso_degenerado_direccion_casi_cero) {
    render::matte const mat{
      render::vector{1.0, 1.0, 1.0}
    };
    render::ray const rayo_incidente{
      render::vector{0.0, 0.0, 0.0},
      render::vector{0.0, 0.0, 1.0}
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 0.0, 1.0};

    // Aunque es poco probable, si la dirección aleatoria + normal ≈ 0,
    // debe devolver la normal
    render::ray const reflejado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // El rayo debe tener una dirección válida (normalizada)
    EXPECT_NEAR(reflejado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST_F(test_matte, reflectancia_blanca) {
    render::vector const reflectancia{1.0, 1.0, 1.0};
    render::matte const mat{reflectancia};

    render::vector const result = mat.get_reflectancia();
    EXPECT_EQ(result.x, 1.0);
    EXPECT_EQ(result.y, 1.0);
    EXPECT_EQ(result.z, 1.0);
  }

  TEST_F(test_matte, reflectancia_negra) {
    render::vector const reflectancia{0.0, 0.0, 0.0};
    render::matte const mat{reflectancia};

    render::vector const result = mat.get_reflectancia();
    EXPECT_EQ(result.x, 0.0);
    EXPECT_EQ(result.y, 0.0);
    EXPECT_EQ(result.z, 0.0);
  }

  TEST_F(test_matte, reflectancia_coloreada) {
    render::vector const reflectancia{0.2, 0.5, 0.9};
    render::matte const mat{reflectancia};

    render::vector const result = mat.get_reflectancia();
    EXPECT_EQ(result.x, 0.2);
    EXPECT_EQ(result.y, 0.5);
    EXPECT_EQ(result.z, 0.9);
  }

  TEST_F(test_matte, rayo_reflejado_front_face_true) {
    render::matte const mat{
      render::vector{0.5, 0.5, 0.5}
    };
    render::ray const rayo_incidente{
      render::vector{0.0,  5.0, 0.0},
      render::vector{0.0, -1.0, 0.0}
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const reflejado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    EXPECT_EQ(reflejado.origen.x, 0.0);
    EXPECT_EQ(reflejado.origen.y, 0.0);
    EXPECT_EQ(reflejado.origen.z, 0.0);
    EXPECT_NEAR(reflejado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST_F(test_matte, rayo_reflejado_front_face_false) {
    render::matte const mat{
      render::vector{0.5, 0.5, 0.5}
    };
    render::ray const rayo_incidente{
      render::vector{0.0, -5.0, 0.0},
      render::vector{0.0,  1.0, 0.0}
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, -1.0, 0.0};

    render::ray const reflejado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, false);

    EXPECT_EQ(reflejado.origen.x, 0.0);
    EXPECT_EQ(reflejado.origen.y, 0.0);
    EXPECT_EQ(reflejado.origen.z, 0.0);
    EXPECT_NEAR(reflejado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST_F(test_matte, multiples_rayos_tienen_direcciones_diferentes) {
    render::matte const mat{
      render::vector{0.8, 0.8, 0.8}
    };
    render::ray const rayo_incidente{
      render::vector{0.0, 0.0, 0.0},
      render::vector{0.0, 0.0, 1.0}
    };
    render::vector const interseccion{0.0, 0.0, 5.0};
    render::vector const normal{0.0, 0.0, -1.0};

    render::ray const reflejado1 = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);
    render::ray const reflejado2 = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);
    render::ray const reflejado3 = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Los rayos deben tener direcciones diferentes (por la aleatoriedad)
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

}  // namespace
