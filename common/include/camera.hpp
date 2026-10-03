#ifndef RENDER_CAMERA_HPP
#define RENDER_CAMERA_HPP

#include "camera_config.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <cmath>

namespace render {

  class camera {
  public:
    // Constructor sin parámetros
    explicit camera(camera_config const & config = camera_config{})
        : config_{config},
          image_height_{static_cast<int>(config_.image_width / config_.aspect_ratio)},
          delta_x_{0, 0, 0}, delta_y_{0, 0, 0}, origen_ventana_{0, 0, 0} {
      // image_height se calcula a partir del aspect ratio y el ancho de la imagen
      calcular_ventana_proyeccion();
    }

    [[nodiscard]] ray generar_rayo(int x, int y) const;

    // getters para los atributos
    [[nodiscard]] vector get_posicion() const { return config_.posicion; }

    [[nodiscard]] vector get_destino() const { return config_.destino; }

    [[nodiscard]] vector get_norte() const { return config_.norte; }

    [[nodiscard]] double get_fov() const { return config_.fov; }

    [[nodiscard]] int get_ancho_imagen() const { return config_.image_width; }

    [[nodiscard]] int get_alto_imagen() const { return image_height_; }

    // getter para los atributos de la cámara necesarios para generar los rayos
    [[nodiscard]] vector get_delta_x() const { return delta_x_; }

    [[nodiscard]] vector get_delta_y() const { return delta_y_; }

    [[nodiscard]] vector get_origen_ventana() const { return origen_ventana_; }

  private:
    // Parametros agrupados en struct camera_config
    camera_config config_;
    int image_height_;

    vector delta_x_;
    vector delta_y_;
    vector origen_ventana_;

    void calcular_ventana_proyeccion();
  };

}  // namespace render

#endif
