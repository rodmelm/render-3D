#include <gtest/gtest.h>

#include <memory>

#include "intersection_info.hpp"
#include "matte.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "vector.hpp"

namespace {

  TEST(test_sphere, constructor) {
    render::vector const centro{0.0, 0.0, 0.0};
    double const radio = 5.0;
    auto material      = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, radio, material};

    EXPECT_EQ(esfera.getcentro().x, 0.0);
    EXPECT_EQ(esfera.getcentro().y, 0.0);
    EXPECT_EQ(esfera.getcentro().z, 0.0);
    EXPECT_EQ(esfera.getradio(), 5.0);
    EXPECT_NE(esfera.get_material(), nullptr);
  }

  TEST(test_sphere, getcentro) {
    render::vector const centro{10.0, 20.0, 30.0};
    auto material = std::make_shared<render::matte>(render::vector{0.5, 0.5, 0.5});
    render::sphere const esfera{centro, 3.0, material};

    render::vector const result = esfera.getcentro();
    EXPECT_EQ(result.x, 10.0);
    EXPECT_EQ(result.y, 20.0);
    EXPECT_EQ(result.z, 30.0);
  }

  TEST(test_sphere, getradio) {
    render::vector const centro{0.0, 0.0, 0.0};
    double const radio = 7.5;
    auto material      = std::make_shared<render::matte>(render::vector{1.0, 0.0, 0.0});
    render::sphere const esfera{centro, radio, material};

    EXPECT_EQ(esfera.getradio(), 7.5);
  }

  TEST(test_sphere, get_material) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{0.8, 0.2, 0.1});
    render::sphere const esfera{centro, 1.0, material};

    auto const mat = esfera.get_material();
    EXPECT_NE(mat, nullptr);
    EXPECT_EQ(mat->get_reflectancia().x, 0.8);
    EXPECT_EQ(mat->get_reflectancia().y, 0.2);
    EXPECT_EQ(mat->get_reflectancia().z, 0.1);
  }

  TEST(test_sphere, intersect_rayo_desde_fuera_perpendicular) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 1.0, material};

    // Rayo desde (0, 0, -5) hacia (0, 0, 1)
    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };

    auto const resultado = esfera.intersect(rayo);
    if (resultado.has_value()) {
      auto const & interseccion = *resultado;
      EXPECT_NEAR(interseccion.get_t(), 4.0, 1e-6);  // Intersecta en z = -1
      EXPECT_NEAR(interseccion.get_punto_interseccion().z, -1.0, 1e-6);
    } else {
      FAIL() << "Expected intersection but none found";
    }
  }

  TEST(test_sphere, intersect_rayo_no_intersecta) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 1.0, material};

    // Rayo que pasa por encima de la esfera
    render::ray const rayo{
      render::vector{0.0, 10.0, -5.0},
      render::vector{0.0,  0.0,  1.0}
    };

    auto const resultado = esfera.intersect(rayo);
    EXPECT_FALSE(resultado.has_value());
  }

  TEST(test_sphere, intersect_rayo_tangente) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 1.0, material};

    // Rayo tangente en y = 1
    render::ray const rayo{
      render::vector{0.0, 1.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };

    auto const resultado = esfera.intersect(rayo);
    if (resultado.has_value()) {
      auto const & interseccion = *resultado;
      EXPECT_NEAR(interseccion.get_punto_interseccion().y, 1.0, 1e-6);
    } else {
      FAIL() << "Expected intersection but none found";
    }
  }

  TEST(test_sphere, intersect_rayo_desde_dentro) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 5.0, material};

    // Rayo desde el interior de la esfera
    render::ray const rayo{
      render::vector{0.0, 0.0, 0.0},
      render::vector{0.0, 0.0, 1.0}
    };

    auto const resultado = esfera.intersect(rayo);
    if (resultado.has_value()) {
      auto const & interseccion = *resultado;
      EXPECT_FALSE(interseccion.is_frente());  // Está golpeando desde dentro
    } else {
      FAIL() << "Expected intersection but none found";
    }
  }

  TEST(test_sphere, intersect_normal_apunta_hacia_fuera) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 1.0, material};

    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };

    auto const resultado = esfera.intersect(rayo);
    if (resultado.has_value()) {
      auto const & interseccion = *resultado;
      EXPECT_TRUE(interseccion.is_frente());
      // La normal debe apuntar hacia el rayo (hacia -z)
      EXPECT_NEAR(interseccion.get_normal().z, -1.0, 1e-6);
    } else {
      FAIL() << "Expected intersection but none found";
    }
  }

  TEST(test_sphere, intersect_normal_normalizada) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 5.0, material};

    render::ray const rayo{
      render::vector{0.0, 0.0, -10.0},
      render::vector{0.0, 0.0,   1.0}
    };

    auto const resultado = esfera.intersect(rayo);
    if (resultado.has_value()) {
      auto const & interseccion = *resultado;
      EXPECT_NEAR(interseccion.get_normal().magnitude(), 1.0, 1e-6);
    } else {
      FAIL() << "Expected intersection but none found";
    }
  }

  TEST(test_sphere, intersect_devuelve_interseccion_mas_cercana) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 1.0, material};

    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };

    auto const resultado = esfera.intersect(rayo);
    if (resultado.has_value()) {
      auto const & interseccion = *resultado;
      // Debe devolver la intersección más cercana
      EXPECT_LT(interseccion.get_t(), 5.0);  // t debe ser menor que la distancia total
      EXPECT_NEAR(interseccion.get_punto_interseccion().z, -1.0, 1e-6);
    } else {
      FAIL() << "Expected intersection but none found";
    }
  }

  TEST(test_sphere, intersect_ignora_intersecciones_muy_cercanas) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 1.0, material};

    // Rayo que empieza casi en la superficie (t < 0.001 debe ser ignorado)
    render::ray const rayo{
      render::vector{0.0, 0.0, -1.0005},
      render::vector{0.0, 0.0,     1.0}
    };

    auto const resultado = esfera.intersect(rayo);
    // Debe encontrar la intersección trasera (si es que la hay)
    if (resultado.has_value()) {
      auto const & interseccion = *resultado;
      EXPECT_GT(interseccion.get_t(), 0.001);
    }
  }

  TEST(test_sphere, intersect_rayo_diagonal) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 5.0, material};

    // Rayo diagonal
    render::ray const rayo{
      render::vector{-10.0, -10.0, -10.0},
      render::vector{  1.0,   1.0,   1.0}
      .normalize()
    };

    auto const resultado = esfera.intersect(rayo);
    if (resultado.has_value()) {
      auto const & interseccion = *resultado;
      EXPECT_NEAR(interseccion.get_normal().magnitude(), 1.0, 1e-6);
    } else {
      FAIL() << "Expected intersection but none found";
    }
  }

  TEST(test_sphere, intersect_esfera_desplazada) {
    render::vector const centro{10.0, 5.0, -3.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 2.0, material};

    // Rayo hacia el centro de la esfera
    render::ray const rayo{
      render::vector{10.0, 5.0, -10.0},
      render::vector{ 0.0, 0.0,   1.0}
    };

    auto const resultado = esfera.intersect(rayo);
    if (resultado.has_value()) {
      auto const & interseccion = *resultado;
      EXPECT_NEAR(interseccion.get_punto_interseccion().z, -5.0, 1e-6);  // z = -3 - 2
    } else {
      FAIL() << "Expected intersection but none found";
    }
  }

  TEST(test_sphere, intersect_material_correcto) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{0.3, 0.7, 0.9});
    render::sphere const esfera{centro, 1.0, material};

    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };

    auto const resultado = esfera.intersect(rayo);
    if (resultado.has_value()) {
      auto const & interseccion = *resultado;
      EXPECT_NE(interseccion.get_material(), nullptr);
      EXPECT_EQ(interseccion.get_material()->get_reflectancia().x, 0.3);
      EXPECT_EQ(interseccion.get_material()->get_reflectancia().y, 0.7);
      EXPECT_EQ(interseccion.get_material()->get_reflectancia().z, 0.9);
    } else {
      FAIL() << "Expected intersection but none found";
    }
  }

  TEST(test_sphere, intersect_punto_en_superficie) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 3.0, material};

    render::ray const rayo{
      render::vector{0.0, 0.0, -10.0},
      render::vector{0.0, 0.0,   1.0}
    };

    auto const resultado = esfera.intersect(rayo);
    if (resultado.has_value()) {
      auto const & interseccion = *resultado;
      // El punto debe estar a distancia 'radio' del centro
      render::vector const distancia = interseccion.get_punto_interseccion() - centro;
      EXPECT_NEAR(distancia.magnitude(), 3.0, 1e-6);
    } else {
      FAIL() << "Expected intersection but none found";
    }
  }

  TEST(test_sphere, radio_grande) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 100.0, material};

    EXPECT_EQ(esfera.getradio(), 100.0);

    render::ray const rayo{
      render::vector{0.0, 0.0, -200.0},
      render::vector{0.0, 0.0,    1.0}
    };
    auto const resultado = esfera.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());
  }

  TEST(test_sphere, radio_pequeno) {
    render::vector const centro{0.0, 0.0, 0.0};
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    render::sphere const esfera{centro, 0.1, material};

    EXPECT_EQ(esfera.getradio(), 0.1);

    render::ray const rayo{
      render::vector{0.0, 0.0, -1.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = esfera.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());
  }

}  // namespace
