#ifndef RENDER_REFRACTIVE_HPP  // HAY QUE CAMBIAR ESTO PARA CADA COSA IMPORANTEEEEEEEEEEEEEEEE
#define RENDER_REFRACTIVE_HPP

#include "material.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <cmath>

namespace render {

  class refractive : public material {
  public:
    refractive(double indice_refraccion) : indice_refraccion{indice_refraccion} { }

    // Implementamos los metodos virtuales puros
    // Metodo para obtener el rayo reflejado
    [[nodiscard]] ray rayo_reflejado(ray const & rayo_incidente, vector const & interseccion,
                                     vector const & normal, bool front_face) const override;

    // Obtenemos la atenuacion del material
    [[nodiscard]] vector get_reflectancia() const override { return vector{1.0, 1.0, 1.0}; }

  private:
    double indice_refraccion;
  };

}  // namespace render

#endif
