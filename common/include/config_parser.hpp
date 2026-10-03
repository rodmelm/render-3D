#ifndef RENDER_CONFIG_PARSER_HPP  // HAY QUE CAMBIAR ESTO PARA CADA COSA IMPORANTEEEEEEEEEEEEEEEE
#define RENDER_CONFIG_PARSER_HPP
#include "vector.hpp"
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace render {

  // Estructura para almacenar la configuración del renderizado
  struct parser_config {
    std::optional<double> aspect_ratio;
    std::optional<int> image_width;
    std::optional<double> gamma;
    std::optional<vector> camera_position;
    std::optional<vector> camera_target;
    std::optional<vector> camera_north;
    std::optional<double> camera_fov;
    std::optional<int> num_samples;
    std::optional<int> max_depth;
    std::optional<int> material_rng_seed;
    std::optional<int> ray_rng_seed;
    std::optional<vector> background_dark_color;
    std::optional<vector> background_light_color;
  };

  class config_parser {
  public:
    config_parser();
    void leer(std::span<char *> argv);
    void parser();
    void comprobar_parametros(std::size_t tamaño_esperado, std::size_t i);
    std::vector<int> parsear_aspect_ratio(std::size_t i);
    int parsear_image_width(std::size_t i);
    double parsear_gamma(std::size_t i);
    vector parsear_camera_position(std::size_t i);
    vector parsear_camera_target(std::size_t i);
    vector parsear_camera_north(std::size_t i);
    double parsear_camera_fov(std::size_t i);
    int parsear_num_samples(std::size_t i);
    int parsear_max_depth(std::size_t i);
    int parsear_material_rng_seed(std::size_t i);
    int parsear_ray_rng_seed(std::size_t i);
    vector parsear_background_dark_color(std::size_t i);
    vector parsear_background_light_color(std::size_t i);

    [[nodiscard]] std::vector<std::vector<std::string>> get_lineas_almacenadas() const {
      return lineas_almacenadas;
    }

    [[nodiscard]] parser_config get_config() const { return config_; }

  private:
    std::vector<std::vector<std::string>> lineas_almacenadas;
    parser_config config_;  // Estructura para almacenar la configuración del renderizado
  };

}  // namespace render
#endif
