#ifndef RENDER_SCENE_HPP  // HAY QUE CAMBIAR ESTO PARA CADA COSA IMPORANTEEEEEEEEEEEEEEEE
#define RENDER_SCENE_HPP

#include "cilinder.hpp"
#include "intersection_info.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include <memory>
#include <optional>
#include <vector>

namespace render {

  class scene {
  public:
    scene() = default;

    // Agregar esferas y cilindros a la escena
    void add_sphere(std::shared_ptr<sphere> const & esfera) { spheres_.push_back(esfera); }

    void add_cylinder(std::shared_ptr<cilinder> const & cilinder) {
      cilinders_.push_back(cilinder);
    }

    // Método para encontrar la intersección más cercana
    [[nodiscard]] std::optional<intersection_info> intersect(ray const & rayo) const;

  private:
    // Contenedores para los objetos de la escena
    std::vector<std::shared_ptr<sphere>> spheres_;
    std::vector<std::shared_ptr<cilinder>> cilinders_;
  };

}  // namespace render

#endif
