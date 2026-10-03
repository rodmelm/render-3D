#include "matte.hpp"
#include "random_system.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <cmath>

namespace render {

  ray matte::rayo_reflejado(ray const & /*rayo_incidente*/, vector const & interseccion,
                            vector const & normal, bool /*front_face*/) const {
    constexpr double threshold = 1e-8;
    // Generamos direccion aleatoria
    vector const direccion_aleatoria{random_system::random_double_material(-1.0, 1.0),
                                     random_system::random_double_material(-1.0, 1.0),
                                     random_system::random_double_material(-1.0, 1.0)};

    // Obtenemos vector direccion de sumar la normal y la direccion aleatoria
    vector const direccion_reflejada = normal + direccion_aleatoria;

    // Comprobamos que cada valor del vector (x y z) no sea menor a 1e-8
    if (direccion_reflejada.magnitude_squared() < threshold * threshold) {
      return ray{interseccion, normal};
    }

    return ray{interseccion, direccion_reflejada.normalize()};
  }

}  // namespace render
