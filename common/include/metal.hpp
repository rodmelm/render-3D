#ifndef RENDER_METAL_HPP  // HAY QUE CAMBIAR ESTO PARA CADA COSA IMPORANTEEEEEEEEEEEEEEEE
#define RENDER_METAL_HPP

#include "material.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <cmath>

namespace render {

  class metal : public material {
  public:
    metal(vector const & reflectancia, double difusion)
        : reflectancia{reflectancia}, difusion{difusion} { }

    // Implementamos los metodos virtuales puros
    // Metodo para obtener el rayo reflejado
    [[nodiscard]] ray rayo_reflejado(ray const & rayo_incidente, vector const & interseccion,
                                     vector const & normal, bool front_face) const override;

    // getters
    [[nodiscard]] vector get_reflectancia() const override { return reflectancia; }

    [[nodiscard]] double get_difusion() const { return difusion; }

  private:
    vector reflectancia;
    double difusion;
  };

}  // namespace render

#endif
