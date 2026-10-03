#include "scene.hpp"
#include "intersection_info.hpp"
#include "ray.hpp"
#include <limits>
#include <optional>

namespace render {

  std::optional<intersection_info> scene::intersect(ray const & rayo) const {
    std::optional<intersection_info> mas_cercana;
    double distancia_minima = std::numeric_limits<double>::max();

    // Comprobar intersección con esferas
    for (auto const & esfera : spheres_) {
      if (auto interseccion = esfera->intersect(rayo)) {
        if (interseccion->es_valida() and interseccion->get_t() < distancia_minima) {
          distancia_minima = interseccion->get_t();
          mas_cercana      = interseccion;
        }
      }
    }

    // Comprobar intersección con cilindros
    for (auto const & cilinder : cilinders_) {
      if (auto interseccion = cilinder->intersect(rayo)) {
        if (interseccion->es_valida() and interseccion->get_t() < distancia_minima) {
          distancia_minima = interseccion->get_t();
          mas_cercana      = interseccion;
        }
      }
    }

    return mas_cercana;
  }

}  // namespace render
