#include <gtest/gtest.h>

#include <memory>

#include "cilinder.hpp"
#include "intersection_info.hpp"
#include "matte.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "vector.hpp"

namespace {

  // Pruebas unitarias para la clase scene
  // Test para probar la construcción de una escena vacía por si no
  // se maneja bien el caso de no tener objetos
  TEST(test_scene, constructor_escena_vacia) {
    render::scene const escena;
    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);
    EXPECT_FALSE(resultado.has_value());
  }

  // Tests para añadir esferas y cilindros a la escena y verificar intersecciones
  TEST(test_scene, añadir_esfera_sola) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    auto esfera   = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 1.0, material);

    escena.add_sphere(esfera);

    // Verificar que ahora hay intersección
    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());
  }

  TEST(test_scene, añadir_varias_esferas) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});

    auto esfera1 = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 1.0, material);
    auto esfera2 = std::make_shared<render::sphere>(render::vector{5.0, 0.0, 0.0}, 1.0, material);
    auto esfera3 = std::make_shared<render::sphere>(render::vector{0.0, 5.0, 0.0}, 1.0, material);

    escena.add_sphere(esfera1);
    escena.add_sphere(esfera2);
    escena.add_sphere(esfera3);

    // Verificar intersección con la primera esfera
    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());
  }

  TEST(test_scene, añadir_cilindro_solo) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    auto cilindro = std::make_shared<render::cilinder>(render::vector{0.0, 0.0, 0.0}, 1.0,
                                                       render::vector{0.0, 2.0, 0.0}, material);

    escena.add_cylinder(cilindro);

    // Verificar que hay intersección
    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());
  }

  TEST(test_scene, añadir_varios_cilindros) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});

    auto cil1 = std::make_shared<render::cilinder>(render::vector{0.0, 0.0, 0.0}, 1.0,
                                                   render::vector{0.0, 2.0, 0.0}, material);
    auto cil2 = std::make_shared<render::cilinder>(render::vector{5.0, 0.0, 0.0}, 1.0,
                                                   render::vector{0.0, 2.0, 0.0}, material);

    escena.add_cylinder(cil1);
    escena.add_cylinder(cil2);

    // Verificar intersección
    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());
  }

  TEST(test_scene, objetos_mixtos_esferas_y_cilindros) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});

    auto esfera   = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 1.0, material);
    auto cilindro = std::make_shared<render::cilinder>(render::vector{3.0, 0.0, 0.0}, 1.0,
                                                       render::vector{0.0, 2.0, 0.0}, material);

    escena.add_sphere(esfera);
    escena.add_cylinder(cilindro);

    // Verificar intersección con la esfera
    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());
  }

  // Tests para verificar intersecciones en diferentes escenarios
  TEST(test_scene, interseccion_sin_objetos) {
    render::scene const escena;
    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);
    EXPECT_FALSE(resultado.has_value());
  }

  TEST(test_scene, interseccion_ray_falla_todos_los_objetos) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    auto esfera   = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 1.0, material);

    escena.add_sphere(esfera);

    render::ray const rayo{
      render::vector{10.0, 10.0, -5.0},
      render::vector{ 0.0,  0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);
    EXPECT_FALSE(resultado.has_value());
  }

  // Test para verificar que scene compara las distancias correctamente
  TEST(test_scene, interseccion_devuelve_esfera_mas_cercana) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});

    auto esfera_cercana =
        std::make_shared<render::sphere>(render::vector{0.0, 0.0, -2.0}, 0.5, material);
    auto esfera_lejana =
        std::make_shared<render::sphere>(render::vector{0.0, 0.0, 2.0}, 0.5, material);

    escena.add_sphere(esfera_lejana);
    escena.add_sphere(esfera_cercana);

    render::ray const rayo{
      render::vector{0.0, 0.0, -10.0},
      render::vector{0.0, 0.0,   1.0}
    };
    auto const resultado = escena.intersect(rayo);

    if (resultado.has_value()) {
      EXPECT_LT(resultado->get_t(), 8.0);
      EXPECT_GT(resultado->get_t(), 7.0);
    } else {
      FAIL() << "Expected intersection with closest sphere";
    }
  }

  // Test para verificar que scene compara las distancias correctamente entre diferentes tipos de
  // objetos
  TEST(test_scene, interseccion_devuelve_objeto_mas_cercano_entre_mixtos) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});

    auto esfera   = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 1.0, material);
    auto cilindro = std::make_shared<render::cilinder>(render::vector{0.0, 0.0, 5.0}, 0.5,
                                                       render::vector{0.0, 2.0, 0.0}, material);

    escena.add_cylinder(cilindro);
    escena.add_sphere(esfera);

    render::ray const rayo{
      render::vector{0.0, 0.0, -10.0},
      render::vector{0.0, 0.0,   1.0}
    };
    auto const resultado = escena.intersect(rayo);

    if (resultado.has_value()) {
      EXPECT_LT(resultado->get_t(), 10.0);
    } else {
      FAIL() << "Expected intersection with sphere";
    }
  }

  TEST(test_scene, interseccion_con_interseccion_invalida_ignorada) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    auto esfera   = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 1.0, material);

    escena.add_sphere(esfera);

    render::ray const rayo{
      render::vector{0.0, 0.0, -1.0005},
      render::vector{0.0, 0.0,     1.0}
    };
    auto const resultado = escena.intersect(rayo);
    if (resultado.has_value()) {
      EXPECT_GT(resultado->get_t(), 0.001);
    }
  }

  TEST(test_scene, interseccion_material_preservado) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{0.8, 0.2, 0.1});
    auto esfera   = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 1.0, material);

    escena.add_sphere(esfera);

    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);

    if (resultado.has_value()) {
      auto const mat = resultado->get_material();
      EXPECT_NE(mat, nullptr);
      EXPECT_EQ(mat->get_reflectancia().x, 0.8);
      EXPECT_EQ(mat->get_reflectancia().y, 0.2);
      EXPECT_EQ(mat->get_reflectancia().z, 0.1);
    } else {
      FAIL() << "Expected intersection";
    }
  }

  TEST(test_scene, interseccion_normal__preservada) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    auto esfera   = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 1.0, material);

    escena.add_sphere(esfera);

    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);

    if (resultado.has_value()) {
      EXPECT_NEAR(resultado->get_normal().magnitude(), 1.0, 1e-6);
      EXPECT_LT(resultado->get_normal().z, 0.0);
    } else {
      FAIL() << "Expected intersection";
    }
  }

  TEST(test_scene, interseccion_posicion_correcta) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    auto esfera   = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 1.0, material);

    escena.add_sphere(esfera);

    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);

    if (resultado.has_value()) {
      render::vector const punto      = resultado->get_punto_interseccion();
      render::vector const del_centro = punto - render::vector{0.0, 0.0, 0.0};
      EXPECT_NEAR(del_centro.magnitude(), 1.0, 1e-6);
    } else {
      FAIL() << "Expected intersection";
    }
  }

  TEST(test_scene, multiples_rays_diferentes_direcciones) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    auto esfera   = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 2.0, material);

    escena.add_sphere(esfera);

    render::ray const rayo1{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    render::ray const rayo2{
      render::vector{-5.0, 0.0, 0.0},
      render::vector{ 1.0, 0.0, 0.0}
    };
    render::ray const rayo3{
      render::vector{0.0, -5.0, 0.0},
      render::vector{0.0,  1.0, 0.0}
    };

    auto const resultado1 = escena.intersect(rayo1);
    auto const resultado2 = escena.intersect(rayo2);
    auto const resultado3 = escena.intersect(rayo3);

    EXPECT_TRUE(resultado1.has_value());
    EXPECT_TRUE(resultado2.has_value());
    EXPECT_TRUE(resultado3.has_value());
  }

  TEST(test_scene, gran_numero_de_esferas) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});

    // Añadir 100 esferas en diferentes posiciones
    for (int i = 0; i < 100; ++i) {
      auto esfera = std::make_shared<render::sphere>(
          render::vector{static_cast<double>(i), 0.0, 0.0}, 0.5, material);
      escena.add_sphere(esfera);
    }

    // Verificar intersección con la primera esfera
    render::ray const rayo{
      render::vector{0.0, 0.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);
    EXPECT_TRUE(resultado.has_value());
  }

  TEST(test_scene, ray_paralelo_a_superficie_esfera) {
    render::scene escena;
    auto material = std::make_shared<render::matte>(render::vector{1.0, 1.0, 1.0});
    auto esfera   = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 1.0, material);

    escena.add_sphere(esfera);

    render::ray const rayo{
      render::vector{0.0, 1.0, -5.0},
      render::vector{0.0, 0.0,  1.0}
    };
    auto const resultado = escena.intersect(rayo);

    if (resultado.has_value()) {
      EXPECT_NEAR(resultado->get_punto_interseccion().y, 1.0, 1e-5);
    }
  }

}  // namespace
