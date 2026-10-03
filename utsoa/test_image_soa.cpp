#include <gtest/gtest.h>

#include "image_soa.hpp"

namespace {

  // Pruebas unitarias para la clase image_soa
  TEST(test_image_soa, constructor_dimensiones) {
    render::image_soa const img{800, 600};
    EXPECT_EQ(img.width(), 800);
    EXPECT_EQ(img.height(), 600);
  }

  // Verificar que los píxeles se inicializan correctamente
  TEST(test_image_soa, constructor_inicializado_en_negro) {
    render::image_soa const img{10, 10};

    // Verificar que todos los píxeles se inicializan a negro (0, 0, 0)
    for (std::size_t y = 0; y < img.height(); ++y) {
      for (std::size_t x = 0; x < img.width(); ++x) {
        render::image_soa::pixel const p = img.get_pixel(x, y);
        EXPECT_EQ(p.r, 0);
        EXPECT_EQ(p.g, 0);
        EXPECT_EQ(p.b, 0);
      }
    }
  }

  // Verificar que se pueden establecer y obtener píxeles
  TEST(test_image_soa, set_and_get_pixel) {
    render::image_soa img{100, 100};
    render::image_soa::pixel const test_pixel{255, 128, 64};

    img.set_pixel(50, 50, test_pixel);
    render::image_soa::pixel const retrieved = img.get_pixel(50, 50);

    EXPECT_EQ(retrieved.r, 255);
    EXPECT_EQ(retrieved.g, 128);
    EXPECT_EQ(retrieved.b, 64);
  }

  TEST(test_image_soa, set_pixel_esq_sup_izq) {
    render::image_soa img{100, 100};
    render::image_soa::pixel const test_pixel{255, 0, 0};

    img.set_pixel(0, 0, test_pixel);
    render::image_soa::pixel const retrieved = img.get_pixel(0, 0);

    EXPECT_EQ(retrieved.r, 255);
    EXPECT_EQ(retrieved.g, 0);
    EXPECT_EQ(retrieved.b, 0);
  }

  TEST(test_image_soa, set_pixel_esq_inf_der) {
    render::image_soa img{100, 100};
    render::image_soa::pixel const test_pixel{0, 255, 0};

    img.set_pixel(99, 99, test_pixel);
    render::image_soa::pixel const retrieved = img.get_pixel(99, 99);

    EXPECT_EQ(retrieved.r, 0);
    EXPECT_EQ(retrieved.g, 255);
    EXPECT_EQ(retrieved.b, 0);
  }

  TEST(test_image_soa, set_multiples_pixels) {
    render::image_soa img{50, 50};

    img.set_pixel(10, 20, render::image_soa::pixel{255, 0, 0});
    img.set_pixel(30, 40, render::image_soa::pixel{0, 255, 0});
    img.set_pixel(25, 25, render::image_soa::pixel{0, 0, 255});

    render::image_soa::pixel const p1 = img.get_pixel(10, 20);
    render::image_soa::pixel const p2 = img.get_pixel(30, 40);
    render::image_soa::pixel const p3 = img.get_pixel(25, 25);

    EXPECT_EQ(p1.r, 255);
    EXPECT_EQ(p1.g, 0);
    EXPECT_EQ(p1.b, 0);

    EXPECT_EQ(p2.r, 0);
    EXPECT_EQ(p2.g, 255);
    EXPECT_EQ(p2.b, 0);

    EXPECT_EQ(p3.r, 0);
    EXPECT_EQ(p3.g, 0);
    EXPECT_EQ(p3.b, 255);
  }

  // Sobrescribir un píxel existente
  TEST(test_image_soa, sobrescribir_pixel) {
    render::image_soa img{50, 50};

    img.set_pixel(25, 25, render::image_soa::pixel{255, 0, 0});
    img.set_pixel(25, 25, render::image_soa::pixel{0, 255, 255});

    render::image_soa::pixel const retrieved = img.get_pixel(25, 25);

    EXPECT_EQ(retrieved.r, 0);
    EXPECT_EQ(retrieved.g, 255);
    EXPECT_EQ(retrieved.b, 255);
  }

  // Verificar que los datos de los píxeles tienen el tamaño correcto
  TEST(test_image_soa, tamaño_datos_r) {
    render::image_soa const img{100, 50};
    EXPECT_EQ(img.r_data().size(), 100 * 50);
  }

  TEST(test_image_soa, tamaño_datos_g) {
    render::image_soa const img{100, 50};
    EXPECT_EQ(img.g_data().size(), 100 * 50);
  }

  TEST(test_image_soa, tamaño_datos_b) {
    render::image_soa const img{100, 50};
    EXPECT_EQ(img.b_data().size(), 100 * 50);
  }

  // Verificar que los datos reflejan los cambios realizados
  TEST(test_image_soa, consistencia_datos_vectores) {
    render::image_soa img{10, 10};

    img.set_pixel(5, 5, render::image_soa::pixel{100, 150, 200});

    std::size_t const expected_index = 5 * 10 + 5;

    EXPECT_EQ(img.r_data()[expected_index], 100);
    EXPECT_EQ(img.g_data()[expected_index], 150);
    EXPECT_EQ(img.b_data()[expected_index], 200);
  }

  // Verificar el comportamiento del constructor de pixel
  TEST(test_image_soa, constructor_pixel_defecto) {
    render::image_soa::pixel const p;
    EXPECT_EQ(p.r, 0);
    EXPECT_EQ(p.g, 0);
    EXPECT_EQ(p.b, 0);
  }

  // Verificar el comportamiento del constructor de pixel con valores específicos
  TEST(test_image_soa, constructor_pixel_valores) {
    render::image_soa::pixel const p{255, 128, 64};
    EXPECT_EQ(p.r, 255);
    EXPECT_EQ(p.g, 128);
    EXPECT_EQ(p.b, 64);
  }

  // Verificar que la imagen puede manejar dimensiones mínimas y máximas
  TEST(test_image_soa, imagen_pequeña) {
    render::image_soa img{1, 1};
    EXPECT_EQ(img.width(), 1);
    EXPECT_EQ(img.height(), 1);

    img.set_pixel(0, 0, render::image_soa::pixel{255, 255, 255});
    render::image_soa::pixel const retrieved = img.get_pixel(0, 0);

    EXPECT_EQ(retrieved.r, 255);
    EXPECT_EQ(retrieved.g, 255);
    EXPECT_EQ(retrieved.b, 255);
  }

  TEST(test_image_soa, dimensiones_imagen_grande) {
    render::image_soa const img{1'920, 1'080};
    EXPECT_EQ(img.width(), 1'920);
    EXPECT_EQ(img.height(), 1'080);
    EXPECT_EQ(img.r_data().size(), 1'920 * 1'080);
  }

  // Verificar imágenes rectangulares
  TEST(test_image_soa, imagenes_rectangulares) {
    render::image_soa const img1{200, 100};
    EXPECT_EQ(img1.width(), 200);
    EXPECT_EQ(img1.height(), 100);

    render::image_soa const img2{100, 200};
    EXPECT_EQ(img2.width(), 100);
    EXPECT_EQ(img2.height(), 200);
  }

  // Verificar que los píxeles no se afectan entre sí
  TEST(test_image_soa, independencia_pixeles) {
    render::image_soa img{50, 50};

    // Establecer un píxel
    img.set_pixel(10, 10, render::image_soa::pixel{255, 0, 0});

    // Verificar que los píxeles adyacentes no se ven afectados
    render::image_soa::pixel const p_above = img.get_pixel(10, 9);
    render::image_soa::pixel const p_below = img.get_pixel(10, 11);
    render::image_soa::pixel const p_left  = img.get_pixel(9, 10);
    render::image_soa::pixel const p_right = img.get_pixel(11, 10);

    EXPECT_EQ(p_above.r, 0);
    EXPECT_EQ(p_below.r, 0);
    EXPECT_EQ(p_left.r, 0);
    EXPECT_EQ(p_right.r, 0);
  }

  // Verificar con valores extremos de píxeles
  TEST(test_image_soa, valores_extremos_pixel) {
    render::image_soa img{10, 10};

    // Probar con valores extremos
    img.set_pixel(0, 0, render::image_soa::pixel{0, 0, 0});
    img.set_pixel(1, 0, render::image_soa::pixel{255, 255, 255});
    img.set_pixel(2, 0, render::image_soa::pixel{128, 64, 192});

    render::image_soa::pixel const p1 = img.get_pixel(0, 0);
    render::image_soa::pixel const p2 = img.get_pixel(1, 0);
    render::image_soa::pixel const p3 = img.get_pixel(2, 0);

    EXPECT_EQ(p1.r, 0);
    EXPECT_EQ(p2.r, 255);
    EXPECT_EQ(p3.r, 128);
    EXPECT_EQ(p3.g, 64);
    EXPECT_EQ(p3.b, 192);
  }

}  // namespace
