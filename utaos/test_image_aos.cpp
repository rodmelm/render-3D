#include <gtest/gtest.h>

#include "image_aos.hpp"

namespace {

  // Pruebas unitarias para la clase image_aos
  // Tests que aseguran el correcto funcionamiento del constructor
  TEST(test_image_aos, constructor_dimensiones) {
    render::image_aos const img{800, 600};
    EXPECT_EQ(img.width(), 800);
    EXPECT_EQ(img.height(), 600);
  }

  TEST(test_image_aos, constructor_cero) {
    render::image_aos const img{0, 0};
    EXPECT_EQ(img.width(), 0);
    EXPECT_EQ(img.height(), 0);
  }

  // Tests que aseguran el correcto funcionamiento del acceso a píxeles
  TEST(test_image_aos, constructor_inicializador_a_negro) {
    render::image_aos const img{10, 10};
    // Verificar que todos los píxeles están inicializados a (0,0,0)
    for (std::size_t y = 0; y < img.height(); ++y) {
      for (std::size_t x = 0; x < img.width(); ++x) {
        auto const pixel = img.get_pixel(x, y);
        EXPECT_EQ(pixel.r, 0);
        EXPECT_EQ(pixel.g, 0);
        EXPECT_EQ(pixel.b, 0);
      }
    }
  }

  TEST(test_image_aos, constructor_imagen_pequena) {
    render::image_aos const img{1, 1};
    EXPECT_EQ(img.width(), 1);
    EXPECT_EQ(img.height(), 1);
  }

  TEST(test_image_aos, constructor_imagen_grande) {
    render::image_aos const img{1'920, 1'080};
    EXPECT_EQ(img.width(), 1'920);
    EXPECT_EQ(img.height(), 1'080);
  }

  // Tests que crean un pixel y verifican que sus valores se inicializan correctamente
  TEST(test_image_aos, constructor_pixel_defecto) {
    render::image_aos::pixel const p;
    EXPECT_EQ(p.r, 0);
    EXPECT_EQ(p.g, 0);
    EXPECT_EQ(p.b, 0);
  }

  // Test que crea un pixel con valores específicos y verifica que se asignan correctamente
  TEST(test_image_aos, constructor_pixel_con_valores) {
    render::image_aos::pixel const p{255, 128, 64};
    EXPECT_EQ(p.r, 255);
    EXPECT_EQ(p.g, 128);
    EXPECT_EQ(p.b, 64);
  }

  // Tests que verifican el correcto funcionamiento de los métodos set_pixel y get_pixel,
  // verificando que los píxeles se establecen y recuperan correctamente en diferentes posiciones de
  // la imagen.
  TEST(test_image_aos, set_pixel_en_origen) {
    render::image_aos img{100, 100};
    render::image_aos::pixel const p{255, 0, 0};
    img.set_pixel(0, 0, p);

    auto const result = img.get_pixel(0, 0);
    EXPECT_EQ(result.r, 255);
    EXPECT_EQ(result.g, 0);
    EXPECT_EQ(result.b, 0);
  }

  TEST(test_image_aos, set_pixel_en_centro) {
    render::image_aos img{100, 100};
    render::image_aos::pixel const p{0, 255, 0};
    img.set_pixel(50, 50, p);

    auto const result = img.get_pixel(50, 50);
    EXPECT_EQ(result.r, 0);
    EXPECT_EQ(result.g, 255);
    EXPECT_EQ(result.b, 0);
  }

  TEST(test_image_aos, set_pixel_en_esquina_inferior) {
    render::image_aos img{100, 100};
    render::image_aos::pixel const p{0, 0, 255};
    img.set_pixel(99, 99, p);

    auto const result = img.get_pixel(99, 99);
    EXPECT_EQ(result.r, 0);
    EXPECT_EQ(result.g, 0);
    EXPECT_EQ(result.b, 255);
  }

  TEST(test_image_aos, set_multiple_pixels) {
    render::image_aos img{10, 10};

    render::image_aos::pixel const p1{255, 0, 0};
    render::image_aos::pixel const p2{0, 255, 0};
    render::image_aos::pixel const p3{0, 0, 255};

    img.set_pixel(0, 0, p1);
    img.set_pixel(5, 5, p2);
    img.set_pixel(9, 9, p3);

    auto const result1 = img.get_pixel(0, 0);
    auto const result2 = img.get_pixel(5, 5);
    auto const result3 = img.get_pixel(9, 9);

    EXPECT_EQ(result1.r, 255);
    EXPECT_EQ(result2.g, 255);
    EXPECT_EQ(result3.b, 255);
  }

  TEST(test_image_aos, get_pixel_devuelve_valores_correctos) {
    render::image_aos img{50, 50};
    render::image_aos::pixel const p{100, 150, 200};
    img.set_pixel(25, 25, p);

    auto const result = img.get_pixel(25, 25);
    EXPECT_EQ(result.r, 100);
    EXPECT_EQ(result.g, 150);
    EXPECT_EQ(result.b, 200);
  }

  // Test que sobrescribe un píxel existente y verifica que los nuevos valores se establecen
  // correctamente
  TEST(test_image_aos, sobrescribir_pixel) {
    render::image_aos img{50, 50};

    render::image_aos::pixel const p1{255, 0, 0};
    render::image_aos::pixel const p2{0, 255, 0};

    img.set_pixel(10, 10, p1);
    img.set_pixel(10, 10, p2);  // Sobrescribir

    auto const result = img.get_pixel(10, 10);
    EXPECT_EQ(result.r, 0);
    EXPECT_EQ(result.g, 255);
    EXPECT_EQ(result.b, 0);
  }

  // Tests que verifican el correcto funcionamiento del resize
  TEST(test_image_aos, tamaño_datos_despues_de_resize) {
    render::image_aos const img{100, 50};
    auto const & data = img.data();
    EXPECT_EQ(data.size(), 100 * 50);
  }

  TEST(test_image_aos, acceso_datos) {
    render::image_aos img{10, 10};
    render::image_aos::pixel const p{255, 128, 64};
    img.set_pixel(0, 0, p);

    auto const & data = img.data();
    EXPECT_EQ(data[0].r, 255);
    EXPECT_EQ(data[0].g, 128);
    EXPECT_EQ(data[0].b, 64);
  }

  TEST(test_image_aos, poner_pixels_mismo_valor) {
    render::image_aos img{5, 5};
    render::image_aos::pixel const p{200, 100, 50};

    for (std::size_t y = 0; y < img.height(); ++y) {
      for (std::size_t x = 0; x < img.width(); ++x) {
        img.set_pixel(x, y, p);
      }
    }

    for (std::size_t y = 0; y < img.height(); ++y) {
      for (std::size_t x = 0; x < img.width(); ++x) {
        auto const result = img.get_pixel(x, y);
        EXPECT_EQ(result.r, 200);
        EXPECT_EQ(result.g, 100);
        EXPECT_EQ(result.b, 50);
      }
    }
  }

  TEST(test_image_aos, pixel_max_valores) {
    render::image_aos img{10, 10};
    render::image_aos::pixel const p{255, 255, 255};
    img.set_pixel(5, 5, p);

    auto const result = img.get_pixel(5, 5);
    EXPECT_EQ(result.r, 255);
    EXPECT_EQ(result.g, 255);
    EXPECT_EQ(result.b, 255);
  }

  TEST(test_image_aos, pixel_min_valores) {
    render::image_aos img{10, 10};
    render::image_aos::pixel const p{0, 0, 0};
    img.set_pixel(5, 5, p);

    auto const result = img.get_pixel(5, 5);
    EXPECT_EQ(result.r, 0);
    EXPECT_EQ(result.g, 0);
    EXPECT_EQ(result.b, 0);
  }

  // Tests para imágenes no cuadradas
  TEST(test_image_aos, imagen_no_cuadrada_ancho_mayor) {
    render::image_aos const img{1'920, 1'080};
    EXPECT_EQ(img.width(), 1'920);
    EXPECT_EQ(img.height(), 1'080);
    EXPECT_EQ(img.data().size(), 1'920 * 1'080);
  }

  TEST(test_image_aos, imagen_no_cuadrada_alto_mayor) {
    render::image_aos const img{1'080, 1'920};
    EXPECT_EQ(img.width(), 1'080);
    EXPECT_EQ(img.height(), 1'920);
    EXPECT_EQ(img.data().size(), 1'080 * 1'920);
  }

  // Test que crean un gradiente de color en la imagen y verifican que los píxeles se establecen
  // correctamente
  TEST(test_image_aos, set_gradiente) {
    render::image_aos img{256, 100};

    // Crear un gradiente horizontal de rojo
    for (std::size_t y = 0; y < img.height(); ++y) {
      for (std::size_t x = 0; x < img.width(); ++x) {
        auto const value = static_cast<std::uint8_t>(x);
        render::image_aos::pixel const p{value, 0, 0};
        img.set_pixel(x, y, p);
      }
    }

    // Verificar algunos puntos del gradiente
    auto const p0   = img.get_pixel(0, 0);
    auto const p127 = img.get_pixel(127, 0);
    auto const p255 = img.get_pixel(255, 0);

    EXPECT_EQ(p0.r, 0);
    EXPECT_EQ(p127.r, 127);
    EXPECT_EQ(p255.r, 255);
  }

  // Test que establece píxeles en diferentes filas y verifica que son independientes entre sí
  //  asegurando que la modificación de un píxel no afecta a otro en una fila diferente
  TEST(test_image_aos, diferentes_filas_independientes) {
    render::image_aos img{100, 100};

    render::image_aos::pixel const p1{255, 0, 0};
    render::image_aos::pixel const p2{0, 255, 0};

    img.set_pixel(50, 0, p1);   // Primera fila
    img.set_pixel(50, 99, p2);  // Última fila

    auto const result1 = img.get_pixel(50, 0);
    auto const result2 = img.get_pixel(50, 99);

    EXPECT_EQ(result1.r, 255);
    EXPECT_EQ(result1.g, 0);
    EXPECT_EQ(result2.r, 0);
    EXPECT_EQ(result2.g, 255);
  }

  TEST(test_image_aos, width_getter) {
    render::image_aos const img{640, 480};
    EXPECT_EQ(img.width(), 640);
  }

  TEST(test_image_aos, height_getter) {
    render::image_aos const img{640, 480};
    EXPECT_EQ(img.height(), 480);
  }

}  // namespace
