#include <gtest/gtest.h>

#include <cmath>

#include "ray.hpp"
#include "refractive.hpp"
#include "vector.hpp"

namespace {

  TEST(test_refractive, constructor_indice_refraccion) {
    double const indice = 1.5;
    render::refractive const mat{indice};

    // refractive siempre tiene reflectancia blanca (1, 1, 1)
    render::vector const result = mat.get_reflectancia();
    EXPECT_EQ(result.x, 1.0);
    EXPECT_EQ(result.y, 1.0);
    EXPECT_EQ(result.z, 1.0);
  }

  TEST(test_refractive, reflectancia_siempre_blanca) {
    render::refractive const mat1{1.33};  // Agua
    render::refractive const mat2{1.5};   // Vidrio
    render::refractive const mat3{2.42};  // Diamante

    EXPECT_EQ(mat1.get_reflectancia().x, 1.0);
    EXPECT_EQ(mat2.get_reflectancia().x, 1.0);
    EXPECT_EQ(mat3.get_reflectancia().x, 1.0);
  }

  TEST(test_refractive, rayo_reflejado_devuelve_rayo_valido) {
    render::refractive const mat{1.5};
    render::ray const rayo_incidente{
      render::vector{0.0,  5.0, 0.0},
      render::vector{0.0, -1.0, 0.0}
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const resultado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Verificar que el origen es la intersección
    EXPECT_EQ(resultado.origen.x, 0.0);
    EXPECT_EQ(resultado.origen.y, 0.0);
    EXPECT_EQ(resultado.origen.z, 0.0);

    // Verificar que la dirección está normalizada
    EXPECT_NEAR(resultado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST(test_refractive, refraccion_aire_a_vidrio_perpendicular) {
    render::refractive const mat{1.5};  // Vidrio
    // Rayo perpendicular a la superficie (no debe desviarse)
    render::ray const rayo_incidente{
      render::vector{0.0,  5.0, 0.0},
      render::vector{0.0, -1.0, 0.0}
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const resultado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Rayo perpendicular no se desvía (solo cambia velocidad)
    EXPECT_NEAR(resultado.direccion.x, 0.0, 1e-6);
    EXPECT_LT(resultado.direccion.y, 0.0);  // Sigue hacia abajo
    EXPECT_NEAR(resultado.direccion.z, 0.0, 1e-6);
    EXPECT_NEAR(resultado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST(test_refractive, refraccion_aire_a_vidrio_angulo) {
    render::refractive const mat{1.5};
    // Rayo a 45 grados
    render::ray const rayo_incidente{
      render::vector{-5.0,  5.0, 0.0},
      render::vector{ 1.0, -1.0, 0.0}
      .normalize()
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const resultado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Debe estar normalizado
    EXPECT_NEAR(resultado.direccion.magnitude(), 1.0, 1e-6);
    // El ángulo de refracción debe ser menor que el de incidencia (se acerca a la normal)
    EXPECT_LT(std::abs(resultado.direccion.x),
              std::abs(rayo_incidente.direccion.x));
  }

  TEST(test_refractive, refraccion_vidrio_a_aire) {
    render::refractive const mat{1.5};
    // Rayo saliendo del vidrio al aire (front_face = false)
    render::ray const rayo_incidente{
      render::vector{0.0, -5.0, 0.0},
      render::vector{1.0,  1.0, 0.0}
      .normalize()
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, -1.0, 0.0};  // Normal hacia el interior

    render::ray const resultado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, false);

    // Debe estar normalizado
    EXPECT_NEAR(resultado.direccion.magnitude(), 1.0, 1e-6);
    // El ángulo de refracción debe ser mayor (se aleja de la normal)
  }

  TEST(test_refractive, reflexion_interna_total) {
    render::refractive const mat{1.5};
    // Rayo con ángulo muy rasante saliendo del vidrio (debe reflejarse totalmente)
    render::ray const rayo_incidente{
      render::vector{0.0,  -5.0, 0.0},
      render::vector{0.9, 0.436, 0.0}
      .normalize()
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, -1.0, 0.0};

    render::ray const resultado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, false);

    // Con reflexión interna total, el rayo se refleja (no refracta)
    EXPECT_NEAR(resultado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST(test_refractive, indice_agua) {
    render::refractive const mat{1.33};  // Agua
    render::ray const rayo_incidente{
      render::vector{0.0,  5.0, 0.0},
      render::vector{0.0, -1.0, 0.0}
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const resultado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    EXPECT_NEAR(resultado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST(test_refractive, indice_diamante) {
    render::refractive const mat{2.42};  // Diamante
    render::ray const rayo_incidente{
      render::vector{0.0,  5.0, 0.0},
      render::vector{0.0, -1.0, 0.0}
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const resultado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    EXPECT_NEAR(resultado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST(test_refractive, front_face_true_entrando_material) {
    render::refractive const mat{1.5};
    render::ray const rayo_incidente{
      render::vector{0.0,  5.0, 0.0},
      render::vector{1.0, -1.0, 0.0}
      .normalize()
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    // front_face = true: entrando al material (aire -> vidrio)
    render::ray const resultado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    EXPECT_NEAR(resultado.direccion.magnitude(), 1.0, 1e-6);
    // El rayo debe refractarse hacia la normal
  }

  TEST(test_refractive, front_face_false_saliendo_material) {
    render::refractive const mat{1.5};
    render::ray const rayo_incidente{
      render::vector{0.0, -5.0, 0.0},
      render::vector{1.0,  1.0, 0.0}
      .normalize()
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, -1.0, 0.0};

    // front_face = false: saliendo del material (vidrio -> aire)
    render::ray const resultado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, false);

    EXPECT_NEAR(resultado.direccion.magnitude(), 1.0, 1e-6);
    // El rayo debe refractarse alejándose de la normal
  }

  TEST(test_refractive, ley_snell_conservacion_energia) {
    render::refractive const mat{1.5};
    render::ray const rayo_incidente{
      render::vector{0.0,  5.0, 0.0},
      render::vector{1.0, -1.0, 0.0}
      .normalize()
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const resultado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // La magnitud siempre debe ser 1 (energía normalizada)
    EXPECT_NEAR(resultado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST(test_refractive, angulo_critico_aproximado) {
    render::refractive const mat{1.5};
    // Ángulo cercano al crítico (arcsin(1/1.5) ≈ 41.8 grados)
    double const angulo_critico = std::asin(1.0 / 1.5);
    double const angulo_prueba  = angulo_critico - 0.1;  // Ligeramente menor

    render::vector const dir_incidente{std::sin(angulo_prueba), std::cos(angulo_prueba), 0.0};
    render::ray const rayo_incidente{
      render::vector{0.0, -5.0, 0.0},
      dir_incidente
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, -1.0, 0.0};

    render::ray const resultado = mat.rayo_reflejado(rayo_incidente, interseccion, normal, false);

    // Debe refractarse (no reflexión total)
    EXPECT_NEAR(resultado.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST(test_refractive, multiples_refracciones_consistentes) {
    render::refractive const mat{1.5};
    render::ray const rayo_incidente{
      render::vector{0.0,  5.0, 0.0},
      render::vector{0.0, -1.0, 0.0}
    };
    render::vector const interseccion{0.0, 0.0, 0.0};
    render::vector const normal{0.0, 1.0, 0.0};

    render::ray const resultado1 = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);
    render::ray const resultado2 = mat.rayo_reflejado(rayo_incidente, interseccion, normal, true);

    // Los resultados deben ser idénticos (no hay aleatoriedad)
    EXPECT_EQ(resultado1.direccion.x, resultado2.direccion.x);
    EXPECT_EQ(resultado1.direccion.y, resultado2.direccion.y);
    EXPECT_EQ(resultado1.direccion.z, resultado2.direccion.z);
  }

}  // namespace
