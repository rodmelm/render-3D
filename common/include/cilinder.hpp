#ifndef RENDER_CILINDER_HPP  // HAY QUE CAMBIAR ESTO PARA CADA COSA IMPORANTEEEEEEEEEEEEEEEE
#define RENDER_CILINDER_HPP

#include "intersection_info.hpp"
#include "material.hpp"
#include "vector.hpp"
#include <cmath>
#include <memory>
#include <optional>

namespace render {

  class cilinder {
  public:
    cilinder(vector const & centro, double radio, vector const & eje, std::shared_ptr<material> mat)
        : centro{centro}, radio{radio}, eje{eje}, altura{eje.magnitude()},
          material_{std::move(mat)} { }

    [[nodiscard]] std::optional<intersection_info> intersect(ray const & rayo) const;

    // Getters
    [[nodiscard]] vector getcentro() const { return centro; }

    [[nodiscard]] double getradio() const { return radio; }

    [[nodiscard]] vector geteje() const { return eje; }

    [[nodiscard]] double getaltura() const { return altura; }

    [[nodiscard]] std::shared_ptr<material> get_material() const { return material_; }

  private:
    vector centro;
    double radio;
    vector eje;
    double altura;
    std::shared_ptr<material> material_;
    // Aqui falta el material a añadir luego, tambien haria falta un getter para el matreial
  };

}  // namespace render

#endif
