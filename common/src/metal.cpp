#include "metal.hpp"
#include "random_system.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <cmath>

namespace render {

  ray metal::rayo_reflejado(ray const & rayo_incidente, vector const & interseccion,
                            vector const & normal, bool /*front_face*/) const {
    // Calculamos la direccion de reflexion inicial
    vector const direccion_incidente = rayo_incidente.direccion;
    vector direccion_reflexion_inicial =
        (direccion_incidente - normal * (2 * direccion_incidente.producto(normal))).normalize();

    // Generamos valor aleatorio a partir de la difusion
    direccion_reflexion_inicial =
        direccion_reflexion_inicial +
        vector{random_system::random_double_material(-difusion, difusion),
               random_system::random_double_material(-difusion, difusion),
               random_system::random_double_material(-difusion, difusion)};

    direccion_reflexion_inicial = direccion_reflexion_inicial.normalize();
    return ray{interseccion, direccion_reflexion_inicial};
  }

}  // namespace render
