#include <gtest/gtest.h>

#include "camera.hpp"
#include "camera_config.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <cmath>

namespace {

  // Tests para la clase camera
  // Constructor por defecto
  TEST(test_camera, constructor_por_defecto) {
    render::camera const cam;

    EXPECT_EQ(cam.get_posicion().x, 0.0);
    EXPECT_EQ(cam.get_posicion().y, 0.0);
    EXPECT_EQ(cam.get_posicion().z, -10.0);

    EXPECT_EQ(cam.get_destino().x, 0.0);
    EXPECT_EQ(cam.get_destino().y, 0.0);
    EXPECT_EQ(cam.get_destino().z, 0.0);

    EXPECT_EQ(cam.get_norte().x, 0.0);
    EXPECT_EQ(cam.get_norte().y, 1.0);
    EXPECT_EQ(cam.get_norte().z, 0.0);

    EXPECT_EQ(cam.get_fov(), 90.0);
    EXPECT_EQ(cam.get_ancho_imagen(), 1'920);
  }

  // Constructor con camera_config
  TEST(test_camera, constructor_con_config) {
    render::camera_config config;
    config.posicion     = render::vector{5.0, 5.0, 5.0};
    config.destino      = render::vector{0.0, 0.0, 0.0};
    config.norte        = render::vector{0.0, 1.0, 0.0};
    config.fov          = 60.0;
    config.image_width  = 800;
    config.aspect_ratio = 1.0;

    render::camera const cam{config};

    EXPECT_EQ(cam.get_posicion().x, 5.0);
    EXPECT_EQ(cam.get_posicion().y, 5.0);
    EXPECT_EQ(cam.get_posicion().z, 5.0);
    EXPECT_EQ(cam.get_fov(), 60.0);
    EXPECT_EQ(cam.get_ancho_imagen(), 800);
  }

  // Getters
  TEST(test_camera, get_posicion) {
    render::camera_config config;
    config.posicion = render::vector{10.0, 20.0, 30.0};

    render::camera const cam{config};
    render::vector const pos = cam.get_posicion();

    EXPECT_EQ(pos.x, 10.0);
    EXPECT_EQ(pos.y, 20.0);
    EXPECT_EQ(pos.z, 30.0);
  }

  TEST(test_camera, get_destino) {
    render::camera_config config;
    config.destino = render::vector{5.0, 5.0, 5.0};

    render::camera const cam{config};
    render::vector const dest = cam.get_destino();

    EXPECT_EQ(dest.x, 5.0);
    EXPECT_EQ(dest.y, 5.0);
    EXPECT_EQ(dest.z, 5.0);
  }

  TEST(test_camera, get_norte) {
    render::camera_config config;
    config.norte = render::vector{0.0, 0.0, 1.0};

    render::camera const cam{config};
    render::vector const norte = cam.get_norte();

    EXPECT_EQ(norte.x, 0.0);
    EXPECT_EQ(norte.y, 0.0);
    EXPECT_EQ(norte.z, 1.0);
  }

  TEST(test_camera, get_fov) {
    render::camera_config config;
    config.fov = 45.0;

    render::camera const cam{config};

    EXPECT_EQ(cam.get_fov(), 45.0);
  }

  TEST(test_camera, get_ancho_imagen) {
    render::camera_config config;
    config.image_width = 1'280;

    render::camera const cam{config};

    EXPECT_EQ(cam.get_ancho_imagen(), 1'280);
  }

  TEST(test_camera, get_alto_imagen_16_9) {
    render::camera_config config;
    config.image_width  = 1'920;
    config.aspect_ratio = 16.0 / 9.0;

    render::camera const cam{config};

    EXPECT_EQ(cam.get_alto_imagen(), 1'080);
  }

  TEST(test_camera, get_alto_imagen_4_3) {
    render::camera_config config;
    config.image_width  = 800;
    config.aspect_ratio = 4.0 / 3.0;

    render::camera const cam{config};

    EXPECT_EQ(cam.get_alto_imagen(), 600);
  }

  TEST(test_camera, get_alto_imagen_cuadrada) {
    render::camera_config config;
    config.image_width  = 500;
    config.aspect_ratio = 1.0;

    render::camera const cam{config};

    EXPECT_EQ(cam.get_alto_imagen(), 500);
  }

  // Generar rayo
  TEST(test_camera, generar_rayo_devuelve_ray) {
    render::camera const cam;

    render::ray const rayo = cam.generar_rayo(0, 0);

    // Verificar que el origen del rayo es la posición de la cámara
    EXPECT_NEAR(rayo.origen.x, cam.get_posicion().x, 1e-6);
    EXPECT_NEAR(rayo.origen.y, cam.get_posicion().y, 1e-6);
    EXPECT_NEAR(rayo.origen.z, cam.get_posicion().z, 1e-6);
  }

  // La dirección del rayo está normalizada
  TEST(test_camera, generar_rayo_direccion_normalizada) {
    render::camera const cam;

    render::ray const rayo = cam.generar_rayo(100, 100);

    double const magnitude = rayo.direccion.magnitude();
    EXPECT_NEAR(magnitude, 1.0, 1e-6);
  }

  // Diferentes píxeles generan diferentes rayos
  TEST(test_camera, generar_rayo_diferentes_pixels) {
    render::camera const cam;

    render::ray const rayo1 = cam.generar_rayo(0, 0);
    render::ray const rayo2 = cam.generar_rayo(100, 100);

    // Los rayos para diferentes píxeles deben tener direcciones diferentes
    bool const different = (rayo1.direccion.x != rayo2.direccion.x) or
                           (rayo1.direccion.y != rayo2.direccion.y) or
                           (rayo1.direccion.z != rayo2.direccion.z);

    EXPECT_TRUE(different);
  }

  TEST(test_camera, generar_rayo_pixel_central) {
    render::camera_config config;
    config.posicion     = render::vector{0.0, 0.0, 0.0};
    config.destino      = render::vector{0.0, 0.0, 10.0};
    config.image_width  = 100;
    config.aspect_ratio = 1.0;

    render::camera const cam{config};

    render::ray const rayo = cam.generar_rayo(50, 50);

    EXPECT_GT(rayo.direccion.z, 0.0);
  }

  // Verificar que los deltas no son cero
  TEST(test_camera, get_delta_x_no_es_cero) {
    render::camera const cam;
    render::vector const delta_x = cam.get_delta_x();

    bool const non_zero =
        (delta_x.x != 0.0) or (delta_x.y != 0.0) or (delta_x.z != 0.0);
    EXPECT_TRUE(non_zero);
  }

  TEST(test_camera, get_delta_y_no_es_cero) {
    render::camera const cam;
    render::vector const delta_y = cam.get_delta_y();

    // Delta Y no debería ser un vector cero
    bool const non_zero =
        (delta_y.x != 0.0) or (delta_y.y != 0.0) or (delta_y.z != 0.0);
    EXPECT_TRUE(non_zero);
  }

  // Verificar que el origen de la ventana no es cero
  TEST(test_camera, get_origen_ventana_no_es_cero) {
    render::camera const cam;
    render::vector const origen = cam.get_origen_ventana();

    // El origen de la ventana no debería ser un vector cero
    bool const non_zero =
        (origen.x != 0.0) or (origen.y != 0.0) or (origen.z != 0.0);
    EXPECT_TRUE(non_zero);
  }

  // Diferentes FOV deberían resultar en diferentes deltas
  TEST(test_camera, diferentes_fov_diferentes_deltas) {
    render::camera_config config1;
    config1.fov = 30.0;
    render::camera const cam1{config1};

    render::camera_config config2;
    config2.fov = 90.0;
    render::camera const cam2{config2};

    render::vector const delta1 = cam1.get_delta_x();
    render::vector const delta2 = cam2.get_delta_x();

    bool const different = (delta1.magnitude() != delta2.magnitude());

    EXPECT_TRUE(different);
  }

  // Verificar rayos en las esquinas de la imagen
  TEST(test_camera, generar_rayo_esquinas_pixels) {
    render::camera_config config;
    config.image_width  = 100;
    config.aspect_ratio = 1.0;

    render::camera const cam{config};

    // Generar rayos para las esquinas
    render::ray const top_left     = cam.generar_rayo(0, 0);
    render::ray const top_right    = cam.generar_rayo(99, 0);
    render::ray const bottom_left  = cam.generar_rayo(0, 99);
    render::ray const bottom_right = cam.generar_rayo(99, 99);

    // Todos los rayos deben estar normalizados
    EXPECT_NEAR(top_left.direccion.magnitude(), 1.0, 1e-6);
    EXPECT_NEAR(top_right.direccion.magnitude(), 1.0, 1e-6);
    EXPECT_NEAR(bottom_left.direccion.magnitude(), 1.0, 1e-6);
    EXPECT_NEAR(bottom_right.direccion.magnitude(), 1.0, 1e-6);
  }

  TEST(test_camera, diferentes_aspect_ratios) {
    render::camera_config config1;
    config1.image_width  = 1'920;
    config1.aspect_ratio = 16.0 / 9.0;
    render::camera const cam1{config1};

    render::camera_config config2;
    config2.image_width  = 1'920;
    config2.aspect_ratio = 4.0 / 3.0;
    render::camera const cam2{config2};

    EXPECT_NE(cam1.get_alto_imagen(), cam2.get_alto_imagen());
  }

  // Casos límite para la configuración de la cámara
  TEST(test_camera, camera_mirando_eje_x_negativo) {
    render::camera_config config;
    config.posicion = render::vector{-10.0, 0.0, 0.0};
    config.destino  = render::vector{0.0, 0.0, 0.0};
    config.norte    = render::vector{0.0, 1.0, 0.0};
    render::camera const cam{config};
    EXPECT_EQ(cam.get_posicion().x, -10.0);
    EXPECT_EQ(cam.get_destino().x, 0.0);
  }

  TEST(test_camera, camera_mirando_eje_y_negativo) {
    render::camera_config config;
    config.posicion = render::vector{0.0, -10.0, 0.0};
    config.destino  = render::vector{0.0, 0.0, 0.0};
    config.norte    = render::vector{0.0, 0.0, 1.0};

    render::camera const cam{config};

    EXPECT_EQ(cam.get_posicion().y, -10.0);
    EXPECT_EQ(cam.get_destino().y, 0.0);
  }

  TEST(test_camera, camera_mirando_eje_z_negativo) {
    render::camera_config config;
    config.posicion = render::vector{0.0, 0.0, -10.0};
    config.destino  = render::vector{0.0, 0.0, 0.0};
    config.norte    = render::vector{0.0, 1.0, 0.0};

    render::camera const cam{config};

    EXPECT_EQ(cam.get_posicion().z, -10.0);
    EXPECT_EQ(cam.get_destino().z, 0.0);
  }

  // Cámara en una posición diagonal
  TEST(test_camera, camera_en_angulo_diagonal) {
    render::camera_config config;
    config.posicion = render::vector{5.0, 5.0, 5.0};
    config.destino  = render::vector{0.0, 0.0, 0.0};
    config.norte    = render::vector{0.0, 1.0, 0.0};

    render::camera const cam{config};

    EXPECT_EQ(cam.get_posicion().x, 5.0);
    EXPECT_EQ(cam.get_posicion().y, 5.0);
    EXPECT_EQ(cam.get_posicion().z, 5.0);
  }

  // Imagen con dimensiones muy pequeñas y muy grandes
  TEST(test_camera, imagen_dimensiones_pequenas) {
    render::camera_config config;
    config.image_width  = 10;
    config.aspect_ratio = 1.0;

    render::camera const cam{config};

    EXPECT_EQ(cam.get_ancho_imagen(), 10);
    EXPECT_EQ(cam.get_alto_imagen(), 10);
  }

  TEST(test_camera, imagen_dimensiones_grandes) {
    render::camera_config config;
    config.image_width  = 3'840;
    config.aspect_ratio = 16.0 / 9.0;

    render::camera const cam{config};

    EXPECT_EQ(cam.get_ancho_imagen(), 3'840);
    EXPECT_EQ(cam.get_alto_imagen(), 2'160);
  }

  // FOV extremos
  TEST(test_camera, fov_pequeno) {
    render::camera_config config;
    config.fov = 30.0;

    render::camera const cam{config};

    EXPECT_EQ(cam.get_fov(), 30.0);
  }

  TEST(test_camera, fov_grande) {
    render::camera_config config;
    config.fov = 120.0;

    render::camera const cam{config};

    EXPECT_EQ(cam.get_fov(), 120.0);
  }

}  // namespace
