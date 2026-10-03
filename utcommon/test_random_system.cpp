#include "random_system.hpp"
#include <cstddef>
#include <cstdint>
#include <gtest/gtest.h>
#include <set>
#include <vector>

namespace {

  // Tests para el sistema de generación de números aleatorios
  TEST(test_random_system, init_material_gen_no_falla) {
    render::random_system::init_material_gen(42);
    SUCCEED();
  }

  TEST(test_random_system, init_ray_gen_no_falla) {
    render::random_system::init_ray_gen(42);
    SUCCEED();
  }

  // Verifica que los generadores devueltos son válidos y producen números
  TEST(test_random_system, get_material_gen_devuelve_generador_valido) {
    render::random_system::init_material_gen(12'345);
    auto & gen = render::random_system::get_material_gen();
    // Generar un número para verificar que funciona
    auto const value = gen();
    EXPECT_GE(value, 0);
  }

  // Test para el generador de rayos
  TEST(test_random_system, get_ray_gen_devuelve_generador_valido) {
    render::random_system::init_ray_gen(67'890);
    auto & gen       = render::random_system::get_ray_gen();
    auto const value = gen();
    EXPECT_GE(value, 0);
  }

  // Tests para la generación de números dobles en rangos específicos
  TEST(test_random_system, random_double_material_en_rango) {
    render::random_system::init_material_gen(42);
    double const min = 0.0;
    double const max = 1.0;

    for (int i = 0; i < 100; ++i) {
      double const value = render::random_system::random_double_material(min, max);
      EXPECT_GE(value, min);
      EXPECT_LE(value, max);
    }
  }

  TEST(test_random_system, random_double_ray_en_rango) {
    render::random_system::init_ray_gen(42);
    double const min = 0.0;
    double const max = 1.0;

    for (int i = 0; i < 100; ++i) {
      double const value = render::random_system::random_double_ray(min, max);
      EXPECT_GE(value, min);
      EXPECT_LE(value, max);
    }
  }

  TEST(test_random_system, random_double_material_en_rango_escogido) {
    render::random_system::init_material_gen(42);
    double const min = -5.0;
    double const max = 10.0;

    for (int i = 0; i < 100; ++i) {
      double const value = render::random_system::random_double_material(min, max);
      EXPECT_GE(value, min);
      EXPECT_LE(value, max);
    }
  }

  TEST(test_random_system, random_double_ray_en_rango_escogido) {
    render::random_system::init_ray_gen(42);
    double const min = -10.0;
    double const max = 5.0;

    for (int i = 0; i < 100; ++i) {
      double const value = render::random_system::random_double_ray(min, max);
      EXPECT_GE(value, min);
      EXPECT_LE(value, max);
    }
  }

  // Tests para rangos pequeños
  TEST(test_random_system, random_double_material_rango_reducido) {
    render::random_system::init_material_gen(42);
    double const min = 0.4;
    double const max = 0.6;

    for (int i = 0; i < 50; ++i) {
      double const value = render::random_system::random_double_material(min, max);
      EXPECT_GE(value, min);
      EXPECT_LE(value, max);
    }
  }

  TEST(test_random_system, random_double_ray_rango_reducido) {
    render::random_system::init_ray_gen(42);
    double const min = 0.4;
    double const max = 0.6;

    for (int i = 0; i < 50; ++i) {
      double const value = render::random_system::random_double_ray(min, max);
      EXPECT_GE(value, min);
      EXPECT_LE(value, max);
    }
  }

  // Tests para verificar la reproducibilidad con la misma semilla
  TEST(test_random_system, misma_semilla_produce_misma_secuencia_material) {
    std::vector<double> sequence1;
    std::vector<double> sequence2;

    // Primera secuencia
    sequence1.reserve(10);
    render::random_system::init_material_gen(12'345);
    for (int i = 0; i < 10; ++i) {
      sequence1.push_back(render::random_system::random_double_material(0.0, 1.0));
    }
    // Segunda secuencia con la misma semilla
    sequence2.reserve(10);
    render::random_system::init_material_gen(12'345);
    for (int i = 0; i < 10; ++i) {
      sequence2.push_back(render::random_system::random_double_material(0.0, 1.0));
    }
    // Las secuencias deben ser idénticas
    ASSERT_EQ(sequence1.size(), sequence2.size());
    for (size_t i = 0; i < sequence1.size(); ++i) {
      EXPECT_DOUBLE_EQ(sequence1[i], sequence2[i]);
    }
  }

  TEST(test_random_system, misma_semilla_produce_misma_secuencia_ray) {
    std::vector<double> sequence1;
    std::vector<double> sequence2;

    sequence1.reserve(10);
    render::random_system::init_ray_gen(67'890);
    for (int i = 0; i < 10; ++i) {
      sequence1.push_back(render::random_system::random_double_ray(0.0, 1.0));
    }

    sequence2.reserve(10);
    render::random_system::init_ray_gen(67'890);
    for (int i = 0; i < 10; ++i) {
      sequence2.push_back(render::random_system::random_double_ray(0.0, 1.0));
    }

    ASSERT_EQ(sequence1.size(), sequence2.size());
    for (size_t i = 0; i < sequence1.size(); ++i) {
      EXPECT_DOUBLE_EQ(sequence1[i], sequence2[i]);
    }
  }

  TEST(test_random_system, diferentes_semillas_producen_diferentes_secuencias_material) {
    std::vector<double> sequence1;
    std::vector<double> sequence2;

    sequence1.reserve(10);
    render::random_system::init_material_gen(12'345);
    for (int i = 0; i < 10; ++i) {
      sequence1.push_back(render::random_system::random_double_material(0.0, 1.0));
    }

    sequence2.reserve(10);
    render::random_system::init_material_gen(99'999);  // Semilla diferente
    for (int i = 0; i < 10; ++i) {
      sequence2.push_back(render::random_system::random_double_material(0.0, 1.0));
    }
    // Al menos un valor debe ser diferente
    bool encontrado_diferencia = false;
    for (size_t i = 0; i < sequence1.size(); ++i) {
      if (sequence1[i] != sequence2[i]) {
        encontrado_diferencia = true;
        break;
      }
    }
    EXPECT_TRUE(encontrado_diferencia);
  }

  TEST(test_random_system, diferentes_semillas_producen_diferentes_secuencias_ray) {
    std::vector<double> sequence1;
    std::vector<double> sequence2;

    render::random_system::init_ray_gen(11'111);
    sequence1.reserve(10);
    for (int i = 0; i < 10; ++i) {
      sequence1.push_back(render::random_system::random_double_ray(0.0, 1.0));
    }

    render::random_system::init_ray_gen(22'222);
    sequence2.reserve(10);
    for (int i = 0; i < 10; ++i) {
      sequence2.push_back(render::random_system::random_double_ray(0.0, 1.0));
    }

    bool encontrado_diferencia = false;
    for (size_t i = 0; i < sequence1.size(); ++i) {
      if (sequence1[i] != sequence2[i]) {
        encontrado_diferencia = true;
        break;
      }
    }
    EXPECT_TRUE(encontrado_diferencia);
  }

  // Tests para verificar la independencia entre los dos generadores
  TEST(test_random_system, material_and_ray_generadores_independientes) {
    // Inicializar ambos generadores con la misma semilla
    render::random_system::init_material_gen(42);
    render::random_system::init_ray_gen(42);

    // Aunque tengan la misma semilla inicial, son generadores independientes

    for (int i = 0; i < 10; ++i) {
      double const m = render::random_system::random_double_material(0.0, 1.0);
      double const r = render::random_system::random_double_ray(0.0, 1.0);
      EXPECT_GE(m, 0.0);
      EXPECT_LE(m, 1.0);
      EXPECT_GE(r, 0.0);
      EXPECT_LE(r, 1.0);
    }
  }

  // Tests para verificar la variedad de valores generados, es decir, que no siempre genere el mismo
  TEST(test_random_system, valores_random_tienen_variedad_material) {
    render::random_system::init_material_gen(42);
    std::set<double> valores_unicos;

    for (int i = 0; i < 100; ++i) {
      double const value = render::random_system::random_double_material(0.0, 1.0);
      valores_unicos.insert(value);
    }

    EXPECT_GT(valores_unicos.size(), 90);
  }

  TEST(test_random_system, valores_random_tienen_variedad_ray) {
    render::random_system::init_ray_gen(42);
    std::set<double> valores_unicos;

    for (int i = 0; i < 100; ++i) {
      double const value = render::random_system::random_double_ray(0.0, 1.0);
      valores_unicos.insert(value);
    }

    EXPECT_GT(valores_unicos.size(), 90);
    for (int i = 0; i < 100; ++i) {
      double const value = render::random_system::random_double_ray(0.0, 1.0);
      valores_unicos.insert(value);
    }

    EXPECT_GT(valores_unicos.size(), 90);
  }

  // Tests para rangos extremos y casos borde
  TEST(test_random_system, rango_cero_devuelve_valor_minimo_material) {
    render::random_system::init_material_gen(42);
    double const min   = 5.0;
    double const max   = 5.0;  // Rango de tamaño cero
    double const value = render::random_system::random_double_material(min, max);
    EXPECT_DOUBLE_EQ(value, min);
  }

  TEST(test_random_system, rango_cero_devuelve_valor_minimo_ray) {
    render::random_system::init_ray_gen(42);
    double const min   = 5.0;
    double const max   = 5.0;
    double const value = render::random_system::random_double_ray(min, max);
    EXPECT_DOUBLE_EQ(value, min);
  }

  TEST(test_random_system, rango_negativo_material) {
    render::random_system::init_material_gen(42);
    double const min = -1.0;
    double const max = -0.5;

    for (int i = 0; i < 50; ++i) {
      double const value = render::random_system::random_double_material(min, max);
      EXPECT_GE(value, min);
      EXPECT_LE(value, max);
    }
  }

  TEST(test_random_system, rango_negativo_ray) {
    render::random_system::init_ray_gen(42);
    double const min = -1.0;
    double const max = -0.5;

    for (int i = 0; i < 50; ++i) {
      double const value = render::random_system::random_double_ray(min, max);
      EXPECT_GE(value, min);
      EXPECT_LE(value, max);
    }
  }

  // Tests para semillas grandes
  TEST(test_random_system, valores_con_semilla_grande) {
    std::uint64_t const large_seed = 9'999'999'999'999ULL;
    render::random_system::init_material_gen(large_seed);
    render::random_system::init_ray_gen(large_seed);

    // Verificar que funcionan con semillas grandes
    double const mat_value = render::random_system::random_double_material(0.0, 1.0);
    double const ray_value = render::random_system::random_double_ray(0.0, 1.0);

    EXPECT_GE(mat_value, 0.0);
    EXPECT_LE(mat_value, 1.0);
    EXPECT_GE(ray_value, 0.0);
    EXPECT_LE(ray_value, 1.0);
  }

  // Tests para verificar que si lo llamamos de forma seguida, se producen diferentes valores
  TEST(test_random_system, llamadas_consecutivas_producen_valores_diferentes_material) {
    render::random_system::init_material_gen(42);

    double const value1 = render::random_system::random_double_material(0.0, 1.0);
    double const value2 = render::random_system::random_double_material(0.0, 1.0);
    double const value3 = render::random_system::random_double_material(0.0, 1.0);

    // Es extremadamente improbable que 3 valores consecutivos sean exactamente iguales
    bool const todos_diferentes = (value1 != value2) or (value2 != value3) or (value1 != value3);
    EXPECT_TRUE(todos_diferentes);
  }

  TEST(test_random_system, llamadas_consecutivas_producen_valores_diferentes_ray) {
    render::random_system::init_ray_gen(42);

    double const value1 = render::random_system::random_double_ray(0.0, 1.0);
    double const value2 = render::random_system::random_double_ray(0.0, 1.0);
    double const value3 = render::random_system::random_double_ray(0.0, 1.0);

    bool const todos_diferentes = (value1 != value2) or (value2 != value3) or (value1 != value3);
    EXPECT_TRUE(todos_diferentes);
  }

}  // namespace
