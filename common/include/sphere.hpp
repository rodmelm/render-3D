#ifndef RENDER_SPHERE_HPP  // HAY QUE CAMBIAR ESTO PARA CADA COSA IMPORANTEEEEEEEEEEEEEEEE
#define RENDER_SPHERE_HPP

#include "intersection_info.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <cmath>
#include <memory>

namespace render {

  class sphere {
  public:
    sphere(vector const & centro, double radio, std::shared_ptr<material> mat)
        : centro{centro}, radio{radio}, material_{std::move(mat)} { }

    // Getters
    [[nodiscard]] vector getcentro() const { return centro; }

    [[nodiscard]] double getradio() const { return radio; }

    // Método para obtener el material del objeto
    [[nodiscard]] std::shared_ptr<material> get_material() const { return material_; }

    // Para comprobar si un rayo intersecta con la esfera
    [[nodiscard]] std::optional<intersection_info> intersect(ray const & rayo) const;

  private:
    vector centro;
    double radio;
    std::shared_ptr<material> material_;
  };

}  // namespace render

#endif
