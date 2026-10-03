#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include "../../aos/include/image_aos.hpp"
#include "../../par/include/image_soa.hpp"
#include "camera.hpp"
#include "color_processing.hpp"
#include "scene.hpp"
#include "vector.hpp"
#include <array>
#include <atomic>
#include <iostream>
#include <oneapi/tbb/blocked_range.h>
#include <oneapi/tbb/parallel_for.h>
#include <oneapi/tbb/partitioner.h>
#include <ostream>
#include <type_traits>

namespace render {

  class engine {
  public:
    // estructura principal para almacenar configuracion de renderizado
    struct config {
      int max_depth         = 5;
      int samples_per_pixel = 20;
      double gamma          = 2.2;
      vector background_dark_color{0.25, 0.5, 1.0};
      vector background_light_color{1.0, 1.0, 1.0};
    };

    // Estructura para el resultado de trace_ray
    struct trace_result {
      vector color;
      bool hit;
    };

    engine() = default;

    // estructura auxiliar para almacenar coordenadas de pixel
    struct pixel_coords {
      int x, y;

      pixel_coords(int px, int py) : x(px), y(py) { }
    };

    // metodos principales
    static void render_scene(scene const & scene, camera const & camera, image_aos & image,
                             config const & cfg) {
      render_impl(scene, camera, image, cfg);
    }

    static void render_scene(scene const & scene, camera const & camera, image_soa & image,
                             config const & cfg) {
      render_impl(scene, camera, image, cfg);
    }

  private:
    // metodos para implementar logica de renderizado
    [[nodiscard]] static engine::trace_result trace_ray(ray const & rayo, scene const & escena,
                                                        int profundidad, config const & cfg);

    [[nodiscard]] static vector background_color(ray const & rayo, config const & cfg) {
      // seguimos las indicaciones de la pagina 18
      // Determinamos el vector unitario de la dirección del rayo
      vector const direccion_unitario = rayo.direccion.normalize();

      double const componente_y = direccion_unitario.y;
      // m = (dry + 1) / 2
      double const factor_mezcla = (componente_y + 1.0) * 0.5;

      // El color resultante para el fondo se compone mezclando los colores c_l y c_d usando el
      // factor m: c = (1 - m) * c_l + m * c_d
      return cfg.background_light_color * (1.0 - factor_mezcla) +
             cfg.background_dark_color * factor_mezcla;
    }

    [[nodiscard]] static vector sample_pixel(pixel_coords const & coords, camera const & camara,
                                             scene const & escena, config const & cfg);

    // Template para evitar duplicación con image_aos e image_soa
    template <typename ImageType>  // <- esto hace que podamos usar esta funcion en lo 2 render,
                                   // cambiando su comportamiento segun el tipo de imagen

    // funcion auxiliar para acortar render_impl
    static bool check_values(camera const & camara, ImageType const & image) {
      int const width  = camara.get_ancho_imagen();
      int const height = camara.get_alto_imagen();

      if (image.width() != static_cast<std::size_t>(width) or
          image.height() != static_cast<std::size_t>(height)) [[unlikely]]
      {
        std::cerr << "Error: Tamaño de imagen incorrecto. Se esperaba " << camara.get_ancho_imagen()
                  << "x" << camara.get_alto_imagen() << ".\n";
        return false;
      }
      return true;
    }

    // Constantes de configuración óptima
    static constexpr int OPTIMAL_GRAIN_SIZE = 8;

    // Función auxiliar para mostrar información de renderizado
    static void print_render_info(camera const & camara, config const & cfg) {
      std::cout << "Renderizando imagen " << camara.get_ancho_imagen() << "x"
                << camara.get_alto_imagen() << "\n";
      std::cout << "Muestras por píxel: " << cfg.samples_per_pixel << "\n";
      std::cout << "Profundidad máxima: " << cfg.max_depth << "\n";
      std::cout << "Corrección gamma: " << cfg.gamma << "\n";
      std::cout << "--> Estrategia de partición: simple\n";
      std::cout << "--> Tamaño de grano: " << OPTIMAL_GRAIN_SIZE << "\n";
    }

    // Función auxiliar para ejecutar parallel_for con simple_partitioner
    template <typename Func>
    static void execute_parallel_render(tbb::blocked_range<int> const & range, Func const & func) {
      tbb::parallel_for(range, func, tbb::simple_partitioner{});
    }

    template <typename ImageType>
    static void render_impl(scene const & escena, camera const & camara, ImageType & image,
                            config const & cfg) {
      if (!check_values(camara, image)) [[unlikely]] {
        return;
      }
      print_render_info(camara, cfg);

      std::atomic<int> filas_completadas{0};

      auto process_rows = [&](tbb::blocked_range<int> const & range) {
        for (int y = range.begin(); y < range.end(); ++y) {
          for (int x = 0; x < camara.get_ancho_imagen(); ++x) {
            vector const color_linear = sample_pixel(pixel_coords{x, y}, camara, escena, cfg);
            if constexpr (std::is_same_v<ImageType, image_aos>) {
              auto pixel = color_processing::vector_to_pixel_aos(color_linear, cfg.gamma);
              image.set_pixel(static_cast<std::size_t>(x), static_cast<std::size_t>(y), pixel);
            } else {
              auto pixel = color_processing::vector_to_pixel_soa(color_linear, cfg.gamma);
              image.set_pixel(static_cast<std::size_t>(x), static_cast<std::size_t>(y), pixel);
            }
          }
          int const f = ++filas_completadas;
          if (f % (camara.get_alto_imagen() / 10) == 0 or f == camara.get_alto_imagen())
              [[unlikely]] {
            std::cout << "Progreso: " << (f * 100) / (camara.get_alto_imagen()) << "%\n";
          }
        }
      };

      tbb::blocked_range<int> const range(0, camara.get_alto_imagen(),
                                          static_cast<std::size_t>(OPTIMAL_GRAIN_SIZE));
      execute_parallel_render(range, process_rows);
      std::cout << "Renderizado completado!\n";
    }
  };

}  // namespace render

#endif
