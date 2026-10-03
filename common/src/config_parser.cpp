#include "config_parser.hpp"
#include "vector.hpp"
#include <cstddef>
#include <fstream>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace render {

  config_parser::config_parser() = default;

  void config_parser::leer(std::span<char *> argv) {
    std::vector<std::string> args(argv.begin(), argv.end());
    if (args.size() != 4) {
      throw std::runtime_error("Error: Invalid number of arguments: " +
                               std::to_string(static_cast<long>(args.size()) - 1));
    }
    std::ifstream archivo(args[1]);
    if (archivo.is_open()) {
      std::string linea;
      std::string palabra;
      std::vector<std::string> palabras_almacenadas = {};
      while (getline(archivo, linea)) {
        std::istringstream ss(linea);
        palabras_almacenadas.clear();
        palabra.clear();
        while (ss >> palabra) {  // operador >> salta automáticamente los espacios en blanco
          palabras_almacenadas.push_back(palabra);
        }
        lineas_almacenadas.push_back(palabras_almacenadas);
      }
      archivo.close();
    } else {
      throw std::runtime_error("Error: Could not open file: " + args[1]);
    }
  }

  void config_parser::parser() {
    std::size_t i = 0;
    while (i < lineas_almacenadas.size()) {
      if (!lineas_almacenadas[i].empty()) {
        if (lineas_almacenadas[i][0] == "aspect_ratio:") {
          auto aspect_ratio = parsear_aspect_ratio(i);
          config_.aspect_ratio =
              static_cast<double>(aspect_ratio[0]) / static_cast<double>(aspect_ratio[1]);
        } else if (lineas_almacenadas[i][0] == "image_width:") {
          config_.image_width = parsear_image_width(i);
        } else if (lineas_almacenadas[i][0] == "gamma:") {
          config_.gamma = parsear_gamma(i);
        } else if (lineas_almacenadas[i][0] == "camera_position:") {
          config_.camera_position = parsear_camera_position(i);
        } else if (lineas_almacenadas[i][0] == "camera_target:") {
          config_.camera_target = parsear_camera_target(i);
        } else if (lineas_almacenadas[i][0] == "camera_north:") {
          config_.camera_north = parsear_camera_north(i);
        } else if (lineas_almacenadas[i][0] == "field_of_view:") {
          config_.camera_fov = parsear_camera_fov(i);
        } else if (lineas_almacenadas[i][0] == "samples_per_pixel:") {
          config_.num_samples = parsear_num_samples(i);
        } else if (lineas_almacenadas[i][0] == "max_depth:") {
          config_.max_depth = parsear_max_depth(i);
        } else if (lineas_almacenadas[i][0] == "material_rng_seed:") {
          config_.material_rng_seed = parsear_material_rng_seed(i);
        } else if (lineas_almacenadas[i][0] == "ray_rng_seed:") {
          config_.ray_rng_seed = parsear_ray_rng_seed(i);
        } else if (lineas_almacenadas[i][0] == "background_dark_color:") {
          config_.background_dark_color = parsear_background_dark_color(i);
        } else if (lineas_almacenadas[i][0] == "background_light_color:") {
          config_.background_light_color = parsear_background_light_color(i);
        } else {
          throw std::runtime_error("Error: Unknown scene entity " + lineas_almacenadas[i][0]);
        }
      }
      ++i;
    }
  }

  void config_parser::comprobar_parametros(std::size_t tamaño_esperado, std::size_t i) {
    if (lineas_almacenadas[i].size() < tamaño_esperado) {
      throw std::runtime_error("Error: Invalid value for key: " + lineas_almacenadas[i][0]);
    }
    if (lineas_almacenadas[i].size() > tamaño_esperado) {
      throw std::runtime_error("Error: Extra data after cnfiguration value for key: " +
                               lineas_almacenadas[i][0]);
    }
  }

  std::vector<int> config_parser::parsear_aspect_ratio(std::size_t i) {
    comprobar_parametros(3, i);
    try {
      if (std::stoi(lineas_almacenadas[i][1]) <= 0 and std::stoi(lineas_almacenadas[i][2]) <= 0) {
        throw std::runtime_error("Error: Aspect ratio values must be positive integers");
      }
      std::vector<int> aspect_ratio = {std::stoi(lineas_almacenadas[i][1]),
                                       std::stoi(lineas_almacenadas[i][2])};
      return aspect_ratio;
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

  int config_parser::parsear_image_width(std::size_t i) {
    comprobar_parametros(2, i);
    try {
      if (std::stoi(lineas_almacenadas[i][1]) <= 0) {
        throw std::runtime_error("Error: Image width must be a positive integer");
      }
      return std::stoi(lineas_almacenadas[i][1]);
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

  double config_parser::parsear_gamma(std::size_t i) {
    comprobar_parametros(2, i);
    try {
      double const gamma = std::stod(lineas_almacenadas[i][1]);
      return gamma;
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

  vector config_parser::parsear_camera_position(std::size_t i) {
    comprobar_parametros(4, i);
    try {
      vector posicion = {std::stod(lineas_almacenadas[i][1]), std::stod(lineas_almacenadas[i][2]),
                         std::stod(lineas_almacenadas[i][3])};
      return posicion;
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

  vector config_parser::parsear_camera_target(std::size_t i) {
    comprobar_parametros(4, i);
    try {
      vector destino = {std::stod(lineas_almacenadas[i][1]), std::stod(lineas_almacenadas[i][2]),
                        std::stod(lineas_almacenadas[i][3])};
      return destino;
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

  vector config_parser::parsear_camera_north(std::size_t i) {
    comprobar_parametros(4, i);
    try {
      vector norte = {std::stod(lineas_almacenadas[i][1]), std::stod(lineas_almacenadas[i][2]),
                      std::stod(lineas_almacenadas[i][3])};
      return norte;
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

  double config_parser::parsear_camera_fov(std::size_t i) {
    comprobar_parametros(2, i);
    try {
      double const fov = std::stod(lineas_almacenadas[i][1]);
      if (fov <= 0.0 or fov >= 180.0) {
        throw std::runtime_error("Error: FOV must be in (0, 180) degrees");
      }
      return fov;
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

  int config_parser::parsear_num_samples(std::size_t i) {
    comprobar_parametros(2, i);
    try {
      int const num_samples = std::stoi(lineas_almacenadas[i][1]);
      if (num_samples <= 0) {
        throw std::runtime_error("Error: Number of samples must be a positive integer");
      }
      return num_samples;
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

  int config_parser::parsear_max_depth(std::size_t i) {
    comprobar_parametros(2, i);
    try {
      int const max_depth = std::stoi(lineas_almacenadas[i][1]);
      if (max_depth <= 0) {
        throw std::runtime_error("Error: Max depth must be a non-negative integer");
      }
      return max_depth;
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

  int config_parser::parsear_material_rng_seed(std::size_t i) {
    comprobar_parametros(2, i);
    try {
      int const rng_seed = std::stoi(lineas_almacenadas[i][1]);
      if (rng_seed <= 0) {
        throw std::runtime_error("Error: RNG seed must be a non-negative integer");
      }
      return rng_seed;
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

  int config_parser::parsear_ray_rng_seed(std::size_t i) {
    comprobar_parametros(2, i);
    try {
      int const ray_rng_seed = std::stoi(lineas_almacenadas[i][1]);
      if (ray_rng_seed <= 0) {
        throw std::runtime_error("Error: Ray RNG seed must be a non-negative integer");
      }
      return ray_rng_seed;
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

  vector config_parser::parsear_background_dark_color(std::size_t i) {
    comprobar_parametros(4, i);
    try {
      if (std::stod(lineas_almacenadas[i][1]) < 0.0 or
          std::stod(lineas_almacenadas[i][1]) > 1.0 or
          std::stod(lineas_almacenadas[i][2]) < 0.0 or
          std::stod(lineas_almacenadas[i][2]) > 1.0 or
          std::stod(lineas_almacenadas[i][3]) < 0.0 or
          std::stod(lineas_almacenadas[i][3]) > 1.0)
      {
        throw std::runtime_error("Error: Background dark color values must be between 0.0 and 1.0");
      }
      vector background_dark_color = {std::stod(lineas_almacenadas[i][1]),
                                      std::stod(lineas_almacenadas[i][2]),
                                      std::stod(lineas_almacenadas[i][3])};
      return background_dark_color;
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

  vector config_parser::parsear_background_light_color(std::size_t i) {
    comprobar_parametros(4, i);
    try {
      if (std::stod(lineas_almacenadas[i][1]) < 0.0 or
          std::stod(lineas_almacenadas[i][1]) > 1.0 or
          std::stod(lineas_almacenadas[i][2]) < 0.0 or
          std::stod(lineas_almacenadas[i][2]) > 1.0 or
          std::stod(lineas_almacenadas[i][3]) < 0.0 or
          std::stod(lineas_almacenadas[i][3]) > 1.0)
      {
        throw std::runtime_error(
            "Error: Background light color values must be between 0.0 and 1.0");
      }
      vector background_light_color = {std::stod(lineas_almacenadas[i][1]),
                                       std::stod(lineas_almacenadas[i][2]),
                                       std::stod(lineas_almacenadas[i][3])};
      return background_light_color;
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error de rango ") + e.what());
    }
  }

}  // namespace render
