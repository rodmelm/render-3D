#include "sphere.hpp"
#include "intersection_info.hpp"
#include "ray.hpp"
#include "vector.hpp"

#include <cmath>
#include <optional>

namespace render {

  std::optional<intersection_info> sphere::intersect(ray const & rayo) const {
    vector const rc  = centro - rayo.origen;
    vector const dir = rayo.direccion;

    double const a = dir.producto(dir);
    double const b = -2.0 * dir.producto(rc);
    double const c = rc.producto(rc) - (radio * radio);

    double raiz = b * b - 4 * a * c;

    if (raiz < 0) [[unlikely]] {
      return std::nullopt;
    }

    raiz                = std::sqrt(raiz);
    double const inv_2a = 1.0 / (2.0 * a);
    double const t1     = (-b - raiz) * inv_2a;
    double const t2     = (-b + raiz) * inv_2a;

    // t1 siempre va a ser mas cercana que t2 debido a la formula, entonces solo hay que comprobar
    // que sean mayores a 0.001
    double t = -1.0;
    if (t1 > 0.001) [[likely]] {
      t = t1;
    } else if (t2 > 0.001) {
      t = t2;
    } else [[unlikely]] {
      return std::nullopt;
    }
    vector const punto_interseccion = rayo.punto_at(t);
    vector const normal_sin_ajustar = (punto_interseccion - centro) * (1.0 / radio);
    bool const frente_cara          = normal_sin_ajustar.producto(dir) < 0.0;
    vector normal                   = normal_sin_ajustar;
    if (!frente_cara) {
      normal = normal_sin_ajustar * -1.0;
    }

    return intersection_info{t, punto_interseccion, normal, material_, frente_cara};
  }

}  // namespace render
