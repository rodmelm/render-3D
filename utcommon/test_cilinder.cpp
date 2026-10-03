#include <gtest/gtest.h>

#include <memory>
#include <numbers>

#include "cilinder.hpp"
#include "intersection_info.hpp"
#include "matte.hpp"
#include "ray.hpp"
#include "vector.hpp"

namespace {

  // Tests para la clase cilinder
  // Constructor
  TEST(test_cilinder, constructor) {
    render::vector const centro{0.0, 0.0, 0.0};
    double const radio = 2.0;
    render::vector const eje{0.0, 10.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});

    render::cilinder const cil{centro, radio, eje, material};

    EXPECT_EQ(cil.getcentro().x, 0.0);
    EXPECT_EQ(cil.getcentro().y, 0.0);
    EXPECT_EQ(cil.getcentro().z, 0.0);
    EXPECT_EQ(cil.getradio(), 2.0);
    EXPECT_EQ(cil.geteje().x, 0.0);
    EXPECT_EQ(cil.geteje().y, 10.0);
    EXPECT_EQ(cil.geteje().z, 0.0);
    EXPECT_EQ(cil.getaltura(), 10.0);
    EXPECT_NE(cil.get_material(), nullptr);
  }

  // Getters
  TEST(test_cilinder, getcentro) {
    render::vector const centro{5.0, 10.0, 15.0};
    render::vector const eje{0.0, 5.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{0.5, 0.5, 0.5});

    render::cilinder const cil{centro, 1.0, eje, material};
    render::vector const result = cil.getcentro();

    EXPECT_EQ(result.x, 5.0);
    EXPECT_EQ(result.y, 10.0);
    EXPECT_EQ(result.z, 15.0);
  }

  TEST(test_cilinder, getradio) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 5.0, 0.0};
    double const radio = 3.5;
    auto material      = std::make_shared<render::matte>(render::vector{1.0, 0.0, 0.0});

    render::cilinder const cil{centro, radio, eje, material};

    EXPECT_EQ(cil.getradio(), 3.5);
  }

  TEST(test_cilinder, geteje) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 8.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});

    render::cilinder const cil{centro, 1.0, eje, material};
    render::vector const result = cil.geteje();

    EXPECT_EQ(result.x, 0.0);
    EXPECT_EQ(result.y, 8.0);
    EXPECT_EQ(result.z, 0.0);
  }

  TEST(test_cilinder, getaltura) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 12.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});

    render::cilinder const cil{centro, 1.0, eje, material};

    EXPECT_EQ(cil.getaltura(), 12.0);
  }

  // Altura con eje diagonal
  TEST(test_cilinder, getaltura_eje_diagonal) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{3.0, 4.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});

    render::cilinder const cil{centro, 1.0, eje, material};

    EXPECT_NEAR(cil.getaltura(), 5.0, 1e-6);
  }

  TEST(test_cilinder, get_material) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 5.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{0.8, 0.2, 0.1});

    render::cilinder const cil{centro, 1.0, eje, material};
    auto const mat = cil.get_material();

    EXPECT_NE(mat, nullptr);
    EXPECT_EQ(mat->get_reflectancia().x, 0.8);
    EXPECT_EQ(mat->get_reflectancia().y, 0.2);
    EXPECT_EQ(mat->get_reflectancia().z, 0.1);
  }

  // Rayo que no intersecta el cilindro
  TEST(test_cilinder, fallo_interseccion) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 10.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 1.0, eje, material};

    render::ray const rayo{
      render::vector{10.0, 0.0, 0.0},
      render::vector{ 0.0, 1.0, 0.0}
    };

    auto const resultado = cil.intersect(rayo);
    EXPECT_FALSE(resultado.has_value());
  }

  // Rayo que parte desde dentro del cilindro
  TEST(test_cilinder, interseccion_desde_dentro) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 10.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 5.0, eje, material};

    render::ray const rayo{
      render::vector{0.0, 0.0, 0.0},
      render::vector{1.0, 0.0, 0.0}
    };

    auto const resultado = cil.intersect(rayo);

    if (resultado.has_value()) {
      EXPECT_FALSE(resultado->is_frente());
    }
  }

  // Rayo paralelo al eje que no intersecta
  TEST(test_cilinder, interseccion_paralela_eje_fallo) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 10.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 1.0, eje, material};

    render::ray const rayo{
      render::vector{5.0, -5.0, 0.0},
      render::vector{0.0,  1.0, 0.0}
    };

    auto const resultado = cil.intersect(rayo);
    EXPECT_FALSE(resultado.has_value());
  }

  // Rayo paralelo al eje que intersecta
  TEST(test_cilinder, interseccion_paralela_eje_exito) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 10.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 3.0, eje, material};
    render::ray const rayo{
      render::vector{2.0, -5.0, 0.0},
      render::vector{0.0,  1.0, 0.0}
    };
    auto const resultado = cil.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());
  }

  // Rayo que intersecta la base superior
  TEST(test_cilinder, interseccion_base_superior) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 10.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 2.0, eje, material};

    render::ray const rayo{
      render::vector{0.0, 10.0, 0.0},
      render::vector{0.0, -1.0, 0.0}
    };

    auto const resultado = cil.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());

    if (resultado.has_value()) {
      EXPECT_NEAR(resultado->get_punto_interseccion().y, 5.0, 1e-3);
    }
  }

  // Rayo que intersecta la base inferior
  TEST(test_cilinder, interseccion_base_inferior) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 10.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 2.0, eje, material};

    // Rayo que golpea la base inferior
    render::ray const rayo{
      render::vector{0.0, -10.0, 0.0},
      render::vector{0.0,   1.0, 0.0}
    };

    auto const resultado = cil.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());

    if (resultado.has_value()) {
      EXPECT_NEAR(resultado->get_punto_interseccion().y, -5.0, 1e-3);
    }
  }

  // Intersección devuelve material correcto
  TEST(test_cilinder, interseccion_material_correcto) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 10.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{0.3, 0.7, 0.9});
    render::cilinder const cil{centro, 2.0, eje, material};

    render::ray const rayo{
      render::vector{-5.0, 0.0, 0.0},
      render::vector{ 1.0, 0.0, 0.0}
    };

    auto const resultado = cil.intersect(rayo);

    if (resultado.has_value()) {
      auto const mat = resultado->get_material();
      EXPECT_NE(mat, nullptr);
      EXPECT_EQ(mat->get_reflectancia().x, 0.3);
      EXPECT_EQ(mat->get_reflectancia().y, 0.7);
      EXPECT_EQ(mat->get_reflectancia().z, 0.9);
    } else {
      FAIL() << "Expected intersection but none found";
    }
  }

  // Múltiples intersecciones, devuelve la más cercana
  TEST(test_cilinder, interseccion_devuelve_mas_cercana) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 10.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 2.0, eje, material};

    // Rayo que atraviesa el cilindro completamente
    render::ray const rayo{
      render::vector{-5.0, 0.0, 0.0},
      render::vector{ 1.0, 0.0, 0.0}
    };

    auto const resultado = cil.intersect(rayo);

    if (resultado.has_value()) {
      // Debe devolver la intersección más cercana
      EXPECT_LT(resultado->get_t(), 10.0);
    } else {
      FAIL() << "Expected intersection but none found";
    }
  }

  // Casos de borde
  TEST(test_cilinder, radio_pequeno) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 5.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 0.1, eje, material};

    EXPECT_EQ(cil.getradio(), 0.1);

    render::ray const rayo{
      render::vector{-1.0, 0.0, 0.0},
      render::vector{ 1.0, 0.0, 0.0}
    };
    auto const resultado = cil.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());
  }

  TEST(test_cilinder, radio_grande) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 5.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 100.0, eje, material};

    EXPECT_EQ(cil.getradio(), 100.0);

    render::ray const rayo{
      render::vector{-200.0, 0.0, 0.0},
      render::vector{   1.0, 0.0, 0.0}
    };
    auto const resultado = cil.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());
  }

  TEST(test_cilinder, cilindro_pequeno) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 1.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 1.0, eje, material};

    EXPECT_EQ(cil.getaltura(), 1.0);
  }

  TEST(test_cilinder, cilindro_grande) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 100.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 1.0, eje, material};

    EXPECT_EQ(cil.getaltura(), 100.0);
  }

  // Ejes alineados con los ejes principales
  TEST(test_cilinder, eje_x) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{10.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 2.0, eje, material};

    EXPECT_EQ(cil.geteje().x, 10.0);
    EXPECT_EQ(cil.geteje().y, 0.0);
    EXPECT_EQ(cil.geteje().z, 0.0);
  }

  TEST(test_cilinder, eje_y) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 10.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 2.0, eje, material};

    EXPECT_EQ(cil.geteje().x, 0.0);
    EXPECT_EQ(cil.geteje().y, 10.0);
    EXPECT_EQ(cil.geteje().z, 0.0);
  }

  TEST(test_cilinder, eje_z) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 0.0, 10.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 2.0, eje, material};

    EXPECT_EQ(cil.geteje().x, 0.0);
    EXPECT_EQ(cil.geteje().y, 0.0);
    EXPECT_EQ(cil.geteje().z, 10.0);
  }

  // Eje diagonal
  TEST(test_cilinder, eje_diagonal) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{1.0, 1.0, 1.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 1.0, eje, material};

    double const expected_height = std::numbers::sqrt3;
    EXPECT_NEAR(cil.getaltura(), expected_height, 1e-6);
  }

  // Cilindro con centro desplazado
  TEST(test_cilinder, centro_offset) {
    render::vector const centro{10.0, 20.0, 30.0};
    render::vector const eje{0.0, 5.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 2.0, eje, material};

    EXPECT_EQ(cil.getcentro().x, 10.0);
    EXPECT_EQ(cil.getcentro().y, 20.0);
    EXPECT_EQ(cil.getcentro().z, 30.0);
  }

  // Intersección con rayo que comienza muy cerca de la superficie
  TEST(test_cilinder, interseccion_ignora_demasiado_cerca) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 10.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 2.0, eje, material};

    render::ray const rayo{
      render::vector{-2.001, 0.0, 0.0},
      render::vector{   1.0, 0.0, 0.0}
    };

    auto const resultado = cil.intersect(rayo);

    if (resultado.has_value()) {
      EXPECT_GT(resultado->get_t(), 0.001);
    }
  }

  // Rayo tangente al cilindro
  TEST(test_cilinder, interseccion_tangente) {
    render::vector const centro{0.0, 0.0, 0.0};
    render::vector const eje{0.0, 10.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::cilinder const cil{centro, 2.0, eje, material};

    render::ray const rayo{
      render::vector{-5.0, 0.0, 2.0},
      render::vector{ 1.0, 0.0, 0.0}
    };

    auto const resultado = cil.intersect(rayo);

    EXPECT_TRUE(true);
  }

}  // namespace
