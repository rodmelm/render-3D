#ifndef RENDER_MATTE_HPP  // HAY QUE CAMBIAR ESTO PARA CADA COSA IMPORANTEEEEEEEEEEEEEEEE
#define RENDER_MATTE_HPP

#include "material.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <cmath>

namespace render {

  class matte : public material {
  public:
    matte(vector const & reflectancia) : reflectancia{reflectancia} { }

    // Implementamos los metodos virtuales puros
    // Metodo para obtener el rayo reflejado
    [[nodiscard]] ray rayo_reflejado(ray const & rayo_incidente, vector const & interseccion,
                                     vector const & normal, bool front_face) const override;

    // Obtenemos la atenuacion del material
    [[nodiscard]] vector get_reflectancia() const override { return reflectancia; }

  private:
    vector reflectancia;
  };

}  // namespace render

#endif
