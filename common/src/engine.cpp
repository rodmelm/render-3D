#include "engine.hpp"
#include "camera.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "vector.hpp"
#include <algorithm>
#include <cmath>

namespace render {

  vector engine::sample_pixel(pixel_coords const & coords, camera const & camara,
                              scene const & escena, config const & cfg) {
    vector color_acumulado{0, 0, 0};

    // Primeros rayos de prueba
    int const PRUEBA_INICIAL = std::min(4, cfg.samples_per_pixel);
    int muestras_realizadas  = 0;
    bool todos_al_fondo      = true;

    for (int muestra = 0; muestra < PRUEBA_INICIAL; ++muestra) {
      ray const rayo_muestra = camara.generar_rayo(coords.x, coords.y);
      auto resultado         = trace_ray(rayo_muestra, escena, cfg.max_depth, cfg);
      color_acumulado        = color_acumulado + resultado.color;
      muestras_realizadas++;
      // Verificar si chocó con algo usando el flag de trace_ray
      if (resultado.hit) {
        todos_al_fondo = false;
        break;
      }
    }
    // Si encontramos objetos, completamos todas las muestras
    if (!todos_al_fondo) {
      for (int muestra = muestras_realizadas; muestra < cfg.samples_per_pixel; ++muestra) {
        ray const rayo_muestra = camara.generar_rayo(coords.x, coords.y);
        auto resultado         = trace_ray(rayo_muestra, escena, cfg.max_depth, cfg);
        color_acumulado        = color_acumulado + resultado.color;
        muestras_realizadas++;
      }
    }
    // Si no hay objetos, usamos un muestreo reducido pero suficiente para antialiasing
    else if (cfg.samples_per_pixel > PRUEBA_INICIAL)
    {
      int const MUESTRAS_FONDO = std::min(6, cfg.samples_per_pixel);
      for (int muestra = muestras_realizadas; muestra < MUESTRAS_FONDO; ++muestra) {
        ray const rayo_muestra = camara.generar_rayo(coords.x, coords.y);
        auto resultado         = trace_ray(rayo_muestra, escena, cfg.max_depth, cfg);
        color_acumulado        = color_acumulado + resultado.color;
        muestras_realizadas++;
      }
    }
    return color_acumulado * (1.0 / muestras_realizadas);
  }

  engine::trace_result engine::trace_ray(ray const & rayo, scene const & escena, int profundidad,
                                         config const & cfg) {
    // Comprobacion de profundidad minima para contribución de color
    if (profundidad <= 0) [[unlikely]] {
      return {
        vector{0.0, 0.0, 0.0},
        false
      };
    }
    // Calculamos intersección del rayo con la escena
    auto interseccion = escena.intersect(rayo);
    if (!interseccion) {
      // Si no se produce intersección se genera como contribución el color de fondo
      return {background_color(rayo, cfg), false};
    }
    // Obtener información de la intersección
    auto punto            = interseccion->get_punto_interseccion();
    auto normal           = interseccion->get_normal();
    auto material         = interseccion->get_material();
    bool const front_face = interseccion->is_frente();

    if (!material) [[unlikely]] {
      return {
        vector{0, 0, 0},
        true
      };  // Objeto sin material, pero hubo intersección
    }
    // Generamos rayo reflejado según el material
    ray const rayo_reflejado = material->rayo_reflejado(rayo, punto, normal, front_face);

    // Obtenemos reflectancia del material
    vector const reflectancia = material->get_reflectancia();

    // Calculamos color reflejado recursivamente
    auto const resultado_reflejado = trace_ray(rayo_reflejado, escena, profundidad - 1, cfg);

    // Aplicar atenuación: c = c * r
    vector const color_final = {resultado_reflejado.color.x * reflectancia.x,
                                resultado_reflejado.color.y * reflectancia.y,
                                resultado_reflejado.color.z * reflectancia.z};
    return {color_final, true};  // Siempre que hay intersección
  }

}  // namespace render
