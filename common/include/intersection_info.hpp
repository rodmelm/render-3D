#ifndef RENDER_INTERSECTION_INFO_HPP
#define RENDER_INTERSECTION_INFO_HPP

#include "material.hpp"
#include "vector.hpp"
#include <memory>

namespace render {

  struct intersection_info {
    intersection_info(double t_val, vector const & punto_val, vector const & normal_val,
                      std::shared_ptr<material> material_val, bool frente_val)
        : t{t_val}, punto_interseccion{punto_val}, normal{normal_val},
          material_{std::move(material_val)}, frente{frente_val}, valida{t_val >= 0.01} { }

    // Método para verificar si la intersección es válida
    [[nodiscard]] bool es_valida() const { return valida; }

    // Getters
    [[nodiscard]] double get_t() const { return t; }

    [[nodiscard]] vector get_punto_interseccion() const { return punto_interseccion; }

    [[nodiscard]] vector get_normal() const { return normal; }

    [[nodiscard]] std::shared_ptr<material> get_material() const { return material_; }

    [[nodiscard]] bool is_frente() const { return frente; }

  private:
    // Atributos privados
    double t;
    vector punto_interseccion;
    vector normal;
    std::shared_ptr<material> material_;
    bool frente;
    bool valida;
  };

}  // namespace render

#endif
