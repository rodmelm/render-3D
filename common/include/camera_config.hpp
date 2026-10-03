#ifndef RENDER_CAMERA_CONFIG_HPP
#define RENDER_CAMERA_CONFIG_HPP

#include "vector.hpp"

namespace render {

  struct camera_config {
    vector posicion{0, 0, -10};
    vector destino{0, 0, 0};
    vector norte{0, 1, 0};
    double fov{90.0};
    int image_width{1'920};
    double aspect_ratio{16.0 / 9.0};
    // Constructor por defecto
    camera_config() = default;

    // Constructor con parámetros
    camera_config(vector const & pos, vector const & dest, vector const & nor, double f, int ancho,
                  double ratio)
        : posicion{pos}, destino{dest}, norte{nor}, fov{f}, image_width{ancho},
          aspect_ratio(ratio) { }
  };

}  // namespace render

#endif
