#include "scene_parser.hpp"
#include "cilinder.hpp"
#include "material.hpp"
#include "matte.hpp"
#include "metal.hpp"
#include "refractive.hpp"
#include "sphere.hpp"
#include "vector.hpp"
#include <cstddef>
#include <fstream>
#include <memory>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

  // Para poder compartir punteros inteligentes sin que se libere la memoria al salir de la función
  template <typename T> std::shared_ptr<T> compartir_punteros(T * ptr) {
    return std::shared_ptr<T>(ptr, [](T *) { });
  }

}  // namespace

namespace render {

  scene_parser::scene_parser() = default;

  void scene_parser::leer(std::span<char *> argv) {
    std::vector<std::string> args(argv.begin(), argv.end());
    if (args.size() != 4) {
      throw std::runtime_error("Error: Invalid number of arguments: " +
                               std::to_string(static_cast<long>(args.size()) - 1));
    }
    std::ifstream archivo(args[2]);
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
      throw std::runtime_error("Error: Could not open file: " + args[2]);
    }
    parser_materiales();
    parser_objetos();
  }

  void scene_parser::parser_materiales() {
    std::size_t i = 0;
    while (i < lineas_almacenadas.size()) {
      if (!lineas_almacenadas[i].empty()) {
        if (lineas_almacenadas[i][0] == "matte:") {
          comprobar_parametros_materiales(5, i);
          parsear_mattes(i);
        } else if (lineas_almacenadas[i][0] == "metal:") {
          comprobar_parametros_materiales(6, i);
          parsear_metales(i);
        } else if (lineas_almacenadas[i][0] == "refractive:") {
          comprobar_parametros_materiales(3, i);
          parsear_refractivos(i);
        } else if (lineas_almacenadas[i][0] != "sphere:" and
                   lineas_almacenadas[i][0] != "cylinder:")
        {
          throw std::runtime_error("Error: Unknown scene entity " + lineas_almacenadas[i][0]);
        }
      }
      ++i;
    }
  }

  void scene_parser::parser_objetos() {
    std::size_t i = 0;
    while (i < lineas_almacenadas.size()) {
      if (!lineas_almacenadas[i].empty()) {
        if (lineas_almacenadas[i][0] == "sphere:") {
          comprobar_parametros_objetos(6, i);
          parsear_esferas(i);
        } else if (lineas_almacenadas[i][0] == "cylinder:") {
          comprobar_parametros_objetos(9, i);
          parsear_cilindros(i);
        }
      }
      ++i;
    }
  }

  void scene_parser::comprobar_parametros_materiales(std::size_t tamaño_esperado, std::size_t i) {
    if (lineas_almacenadas[i].size() < tamaño_esperado) {
      throw std::runtime_error(
          "Error: Invalid " + lineas_almacenadas[i][0] + " material parameters");
    }
    if (lineas_almacenadas[i].size() > tamaño_esperado) {
      throw std::runtime_error("Error: Extra data after configuration value for  " +
                               lineas_almacenadas[i][0]);
    }
    if (materiales.contains(lineas_almacenadas[i][1])) {
      throw std::runtime_error("Error: Duplicate material name: " + lineas_almacenadas[i][1]);
    }
  }

  void scene_parser::comprobar_parametros_objetos(std::size_t tamaño_esperado, std::size_t i) {
    if (lineas_almacenadas[i].size() < tamaño_esperado) {
      throw std::runtime_error("Error: Invalid " + lineas_almacenadas[i][0] + " object parameters");
    }
    if (lineas_almacenadas[i].size() > tamaño_esperado) {
      throw std::runtime_error("Error: Extra data after configuration value for " +
                               lineas_almacenadas[i][0]);
    }
    if (not materiales.contains(lineas_almacenadas[i][tamaño_esperado - 1])) {
      throw std::runtime_error("Error: Material not found: " +
                               lineas_almacenadas[i][tamaño_esperado - 1]);
    }
  }

  void scene_parser::parsear_mattes(std::size_t i) {
    try {
      vector const reflectancia{std::stod(lineas_almacenadas[i][2]),
                                std::stod(lineas_almacenadas[i][3]),
                                std::stod(lineas_almacenadas[i][4])};
      materiales[lineas_almacenadas[i][1]] = std::make_unique<matte>(reflectancia);
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error: Invalid matte parameter") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error: matte parameter out of range ") + e.what());
    }
  }

  void scene_parser::parsear_metales(std::size_t i) {
    try {
      vector const reflectancia{std::stod(lineas_almacenadas[i][2]),
                                std::stod(lineas_almacenadas[i][3]),
                                std::stod(lineas_almacenadas[i][4])};
      materiales[lineas_almacenadas[i][1]] =
          std::make_unique<metal>(reflectancia, std::stod(lineas_almacenadas[i][5]));
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error: de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error: de rango ") + e.what());
    }
  }

  void scene_parser::parsear_refractivos(std::size_t i) {
    try {
      materiales[lineas_almacenadas[i][1]] =
          std::make_unique<refractive>(std::stod(lineas_almacenadas[i][2]));
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error: de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error: de rango ") + e.what());
    }
  }

  void scene_parser::parsear_esferas(std::size_t i) {
    try {
      vector const centro{std::stod(lineas_almacenadas[i][1]), std::stod(lineas_almacenadas[i][2]),
                          std::stod(lineas_almacenadas[i][3])};
      double const radio = std::stod(lineas_almacenadas[i][4]);
      auto mat           = compartir_punteros(materiales[lineas_almacenadas[i][5]].get());
      objetos_esfera.push_back(std::make_unique<sphere>(centro, radio, mat));
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error: de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error: de rango ") + e.what());
    }
  }

  void scene_parser::parsear_cilindros(std::size_t i) {
    try {
      vector const centro{std::stod(lineas_almacenadas[i][1]), std::stod(lineas_almacenadas[i][2]),
                          std::stod(lineas_almacenadas[i][3])};
      double const radio = std::stod(lineas_almacenadas[i][4]);
      vector const eje{std::stod(lineas_almacenadas[i][5]), std::stod(lineas_almacenadas[i][6]),
                       std::stod(lineas_almacenadas[i][7])};

      auto mat = compartir_punteros(materiales[lineas_almacenadas[i][8]].get());
      objetos_cilindro.push_back(std::make_unique<cilinder>(centro, radio, eje, mat));
    } catch (std::invalid_argument const & e) {
      throw std::runtime_error(std::string("Error: de formato ") + e.what());
    } catch (std::out_of_range const & e) {
      throw std::runtime_error(std::string("Error: de rango ") + e.what());
    }
  }

  std::vector<std::shared_ptr<sphere>> scene_parser::get_objetos_esfera_shared() const {
    std::vector<std::shared_ptr<sphere>> result;
    result.reserve(objetos_esfera.size());
    for (auto const & esfera : objetos_esfera) {
      result.push_back(std::shared_ptr<sphere>(esfera.get(), [](sphere *) { }));
    }
    return result;
  }

  std::vector<std::shared_ptr<cilinder>> scene_parser::get_objetos_cilindro_shared() const {
    std::vector<std::shared_ptr<cilinder>> result;
    result.reserve(objetos_cilindro.size());
    for (auto const & cilindro : objetos_cilindro) {
      result.push_back(std::shared_ptr<cilinder>(cilindro.get(), [](cilinder *) { }));
    }
    return result;
  }

  std::unordered_map<std::string, std::shared_ptr<material>> scene_parser::get_materiales_shared()
      const {
    std::unordered_map<std::string, std::shared_ptr<material>> result;
    for (auto const & [nombre, mat_ptr] : materiales) {
      result[nombre] = std::shared_ptr<material>(mat_ptr.get(), [](material *) { });
    }
    return result;
  }

}  // namespace render
