#include "camera.hpp"
#include "camera_config.hpp"
#include "engine.hpp"
#include "image_aos.hpp"
#include "image_soa.hpp"
#include "matte.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "vector.hpp"
#include <cstddef>
#include <gtest/gtest.h>
#include <memory>

namespace {

  // Tests para la clase engine
  // Tests para la estructura config
  TEST(test_engine, config_valores_por_defecto) {
    render::engine::config const cfg;
    EXPECT_EQ(cfg.max_depth, 5);
    EXPECT_EQ(cfg.samples_per_pixel, 20);
    EXPECT_EQ(cfg.gamma, 2.2);
    EXPECT_EQ(cfg.background_dark_color.x, 0.25);
    EXPECT_EQ(cfg.background_dark_color.y, 0.5);
    EXPECT_EQ(cfg.background_dark_color.z, 1.0);
    EXPECT_EQ(cfg.background_light_color.x, 1.0);
    EXPECT_EQ(cfg.background_light_color.y, 1.0);
    EXPECT_EQ(cfg.background_light_color.z, 1.0);
  }

  TEST(test_engine, config_valores_personalizados) {
    render::engine::config cfg;
    cfg.max_depth              = 10;
    cfg.samples_per_pixel      = 50;
    cfg.gamma                  = 1.8;
    cfg.background_dark_color  = render::vector{1.0, 0.0, 0.0};
    cfg.background_light_color = render::vector{0.0, 1.0, 0.0};

    EXPECT_EQ(cfg.max_depth, 10);
    EXPECT_EQ(cfg.samples_per_pixel, 50);
    EXPECT_EQ(cfg.gamma, 1.8);
    EXPECT_EQ(cfg.background_dark_color.x, 1.0);
    EXPECT_EQ(cfg.background_light_color.y, 1.0);
  }

  // Tests para pixel_coords
  TEST(test_engine, pixel_coords_constructor) {
    render::engine::pixel_coords const coords{100, 200};
    EXPECT_EQ(coords.x, 100);
    EXPECT_EQ(coords.y, 200);
  }

  TEST(test_engine, pixel_coords_negativos) {
    render::engine::pixel_coords const coords{-10, -20};
    EXPECT_EQ(coords.x, -10);
    EXPECT_EQ(coords.y, -20);
  }

  TEST(test_engine, pixel_coords_cero) {
    render::engine::pixel_coords const coords{0, 0};
    EXPECT_EQ(coords.x, 0);
    EXPECT_EQ(coords.y, 0);
  }

  // Tests para render_scene con image_soa
  TEST(test_engine, render_scene_soa_basico) {
    render::camera_config cam_config;
    cam_config.posicion     = render::vector{0.0, 0.0, -10.0};
    cam_config.destino      = render::vector{0.0, 0.0, 0.0};
    cam_config.norte        = render::vector{0.0, 1.0, 0.0};
    cam_config.fov          = 90.0;
    cam_config.image_width  = 10;
    cam_config.aspect_ratio = 1.0;

    render::camera const camara{cam_config};
    render::scene const escena;
    render::image_soa imagen{10, 10};
    render::engine::config cfg;
    cfg.samples_per_pixel = 1;

    // No debería lanzar excepciones
    EXPECT_NO_THROW(render::engine::render_scene(escena, camara, imagen, cfg));
  }

  // Tests para render_scene con image_aos
  TEST(test_engine, render_scene_aos_basico) {
    render::camera_config cam_config;
    cam_config.posicion     = render::vector{0.0, 0.0, -10.0};
    cam_config.destino      = render::vector{0.0, 0.0, 0.0};
    cam_config.norte        = render::vector{0.0, 1.0, 0.0};
    cam_config.fov          = 90.0;
    cam_config.image_width  = 10;
    cam_config.aspect_ratio = 1.0;

    render::camera const camara{cam_config};
    render::scene const escena;
    render::image_aos imagen{10, 10};
    render::engine::config cfg;
    cfg.samples_per_pixel = 1;

    // No debería lanzar excepciones
    EXPECT_NO_THROW(render::engine::render_scene(escena, camara, imagen, cfg));
  }

  // Tests de integración para render_scene
  TEST(test_engine, render_scene_soa_con_esfera) {
    render::camera_config cam_config;
    cam_config.posicion     = render::vector{0.0, 0.0, -10.0};
    cam_config.destino      = render::vector{0.0, 0.0, 0.0};
    cam_config.norte        = render::vector{0.0, 1.0, 0.0};
    cam_config.fov          = 90.0;
    cam_config.image_width  = 20;
    cam_config.aspect_ratio = 1.0;

    render::camera const camara{cam_config};
    render::scene escena;

    auto material = std::make_shared<render::matte>(render::vector{1.0, 0.0, 0.0});
    auto esfera   = std::make_shared<render::sphere>(render::vector{0.0, 0.0, 0.0}, 2.0, material);
    escena.add_sphere(esfera);

    render::image_soa imagen{20, 20};
    render::engine::config cfg;
    cfg.samples_per_pixel = 1;
    cfg.max_depth         = 1;

    render::engine::render_scene(escena, camara, imagen, cfg);

    // Verificar que la imagen no está completamente negra
    bool has_non_black = false;
    for (std::size_t y = 0; y < imagen.height(); ++y) {
      for (std::size_t x = 0; x < imagen.width(); ++x) {
        auto const pixel = imagen.get_pixel(x, y);
        if (pixel.r > 0 or pixel.g > 0 or pixel.b > 0) {
          has_non_black = true;
          break;
        }
      }
      if (has_non_black) {
        break;
      }
    }
    EXPECT_TRUE(has_non_black);
  }

  TEST(test_engine, render_scene_diferentes_samples_por_pixel) {
    render::camera_config cam_config;
    cam_config.posicion     = render::vector{0.0, 0.0, -10.0};
    cam_config.destino      = render::vector{0.0, 0.0, 0.0};
    cam_config.norte        = render::vector{0.0, 1.0, 0.0};
    cam_config.fov          = 90.0;
    cam_config.image_width  = 5;
    cam_config.aspect_ratio = 1.0;

    render::camera const camara{cam_config};
    render::scene const escena;
    render::image_soa imagen{5, 5};

    // Probar con diferentes valores de samples_per_pixel
    for (int const samples : {1, 5, 10}) {
      render::engine::config cfg;
      cfg.samples_per_pixel = samples;
      cfg.max_depth         = 1;

      EXPECT_NO_THROW(render::engine::render_scene(escena, camara, imagen, cfg));
    }
  }

  TEST(test_engine, render_scene_diferentes_max_depth) {
    render::camera_config cam_config;
    cam_config.posicion     = render::vector{0.0, 0.0, -10.0};
    cam_config.destino      = render::vector{0.0, 0.0, 0.0};
    cam_config.norte        = render::vector{0.0, 1.0, 0.0};
    cam_config.fov          = 90.0;
    cam_config.image_width  = 5;
    cam_config.aspect_ratio = 1.0;

    render::camera const camara{cam_config};
    render::scene const escena;
    render::image_soa imagen{5, 5};

    // Probar con diferentes profundidades
    for (int const depth : {1, 3, 5, 10}) {
      render::engine::config cfg;
      cfg.samples_per_pixel = 1;
      cfg.max_depth         = depth;

      EXPECT_NO_THROW(render::engine::render_scene(escena, camara, imagen, cfg));
    }
  }

  // Tests para múltiples instancias de config
  TEST(test_engine, config_multiples_instancias) {
    render::engine::config cfg1;
    cfg1.max_depth = 10;

    render::engine::config cfg2;
    cfg2.max_depth = 5;

    EXPECT_EQ(cfg1.max_depth, 10);
    EXPECT_EQ(cfg2.max_depth, 5);
  }

}  // namespace
