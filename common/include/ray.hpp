#ifndef RENDER_RAY_HPP  // HAY QUE CAMBIAR ESTO PARA CADA COSA IMPORANTEEEEEEEEEEEEEEEE
#define RENDER_RAY_HPP

#include "vector.hpp"
#include <cmath>

namespace render {

  class ray {
  public:
    vector origen, direccion;

    ray(vector const & origen, vector const & direccion) : origen{origen}, direccion{direccion} { }

    // Para encontrar cualquier punto en el rayo (formula P = Origen + Dirección*t)
    // (t es la distancia desde el origen)
    [[nodiscard]] constexpr vector punto_at(double t) const noexcept {
      return origen + (direccion * t);
    }
  };

}  // namespace render

#endif
