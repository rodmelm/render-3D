#include "camera.hpp"
#include "random_system.hpp"
#include "ray.hpp"
#include "vector.hpp"

#include <cmath>

#ifndef M_PI
 #define M_PI 3.14159265358979323846
#endif

namespace render {

  void camera::calcular_ventana_proyeccion() {
    // Seguimos instrucciones según el pdf para calcular la ventana de proyección

    // 1. Determinación del vector focal.
    vector const focal = config_.posicion - config_.destino;

    // 2. Distancia focal.
    double const distancia_focal = focal.magnitude();

    // 3. Altura de la ventana de proyección.
    double const altura_ventana =
        2.0 * std::tan((config_.fov * M_PI / 180.0) / 2.0) * distancia_focal;

    // 4. Anchura de la ventana de proyección.
    double const anchura_ventana =
        (static_cast<double>(config_.image_width) / static_cast<double>(image_height_)) *
        altura_ventana;

    // 5. Vectores directores de la ventana.
    vector const v = focal.normalize();
    vector const u = (config_.norte.vectorial(v)).normalize();
    vector const w = v.vectorial(u);

    // 6. Vectores horizontal y vertical.
    vector const horizontal = u * anchura_ventana;
    vector const vertical   = w * -altura_ventana;

    // 7. Origen de la ventana de proyección.
    delta_x_ = horizontal * (1.0 / static_cast<double>(config_.image_width));
    delta_y_ = vertical * (1.0 / static_cast<double>(image_height_));
    origen_ventana_ =
        config_.posicion - focal - (horizontal + vertical) * 0.5 + (delta_x_ + delta_y_) * 0.5;
  }

  ray camera::generar_rayo(int x, int y) const {
    // Obtenemos los offsets aleatorios para antialiasing
    double const offset_x = random_system::random_double_ray(0, 1) - 0.5;
    double const offset_y = random_system::random_double_ray(0, 1) - 0.5;

    vector const punto_ventana =
        origen_ventana_ + delta_x_ * (x + offset_x) + delta_y_ * (y + offset_y);

    // Rayo desde cámara hasta ese punto
    vector const direccion = (punto_ventana - config_.posicion);
    return ray{config_.posicion, direccion.normalize()};
  }

}  // namespace render
