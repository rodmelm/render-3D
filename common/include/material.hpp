#ifndef RENDER_MATERIAL_HPP  // HAY QUE CAMBIAR ESTO PARA CADA COSA IMPORANTEEEEEEEEEEEEEEEE
#define RENDER_MATERIAL_HPP

#include "ray.hpp"
#include "vector.hpp"
#include <cmath>

namespace render {

  class material {
  public:
    material() = default;
    // Special member functions deleted to follow guidelines for base classes
    material(material const &)             = delete;
    material & operator=(material const &) = delete;
    material(material &&)                  = delete;
    material & operator=(material &&)      = delete;

    // Destructor virtual para permitir la herencia
    virtual ~material() = default;

    // Metodos virtuales puros que usarań las hijas
    // Metodo para obtener el rayo reflejado
    [[nodiscard]] virtual ray rayo_reflejado(ray const & rayo_incidente,
                                             vector const & interseccion, vector const & normal,
                                             bool front_face) const = 0;

    // Obtenemos la reflectancia con la atenuacion del material
    [[nodiscard]] virtual vector get_reflectancia() const = 0;
  };

}  // namespace render
#endif
