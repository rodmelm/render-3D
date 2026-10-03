#include "refractive.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <algorithm>
#include <cmath>

namespace render {

  ray refractive::rayo_reflejado(ray const & rayo_incidente, vector const & interseccion,
                                 vector const & normal, bool front_face) const {
    vector const dir_unitaria = rayo_incidente.direccion.normalize();

    double const cos_theta = std::min(-dir_unitaria.producto(normal), 1.0);
    double const sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    // El índice de refracción depende de la dirección del nuevo rayo
    double indice_corregido = 0.0;
    if (front_face) {
      indice_corregido = 1.0 / indice_refraccion;
    } else {
      indice_corregido = indice_refraccion;
    }

    // Caso 1
    if (indice_corregido * sin_theta >= 1.0) {
      vector const direccion = dir_unitaria - normal * (2.0 * dir_unitaria.producto(normal));
      return ray{interseccion, direccion};
    }

    // Caso 2
    vector const u         = (dir_unitaria + normal * cos_theta) * indice_corregido;
    vector const v         = normal * (-std::sqrt(std::abs(1.0 - u.producto(u))));
    vector const direccion = u + v;

    return ray{interseccion, direccion.normalize()};
  }

}  // namespace render
