#ifndef RENDER_SCENE_PARSER_HPP  // HAY QUE CAMBIAR ESTO PARA CADA COSA IMPORANTEEEEEEEEEEEEEEEE
#define RENDER_SCENE_PARSER_HPP
#include "cilinder.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include <memory>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

namespace render {

  class scene_parser {
  public:
    scene_parser();
    void leer(std::span<char *> argv);
    void parser_materiales();
    void comprobar_parametros_materiales(std::size_t tamaño_esperado, std::size_t i);
    void parser_objetos();
    void comprobar_parametros_objetos(std::size_t tamaño_esperado, std::size_t i);
    void parsear_mattes(std::size_t i);
    void parsear_metales(std::size_t i);
    void parsear_refractivos(std::size_t i);
    void parsear_esferas(std::size_t i);
    void parsear_cilindros(std::size_t i);

    std::vector<std::vector<std::string>> get_lineas_almacenadas() const {
      return lineas_almacenadas;
    }

    std::vector<std::shared_ptr<sphere>> get_objetos_esfera_shared() const;

    std::vector<std::shared_ptr<cilinder>> get_objetos_cilindro_shared() const;

    std::unordered_map<std::string, std::shared_ptr<material>> get_materiales_shared() const;

  private:
    std::vector<std::vector<std::string>> lineas_almacenadas;
    std::vector<std::unique_ptr<sphere>> objetos_esfera;
    std::vector<std::unique_ptr<cilinder>> objetos_cilindro;

    std::unordered_map<std::string, std::unique_ptr<material>>
        materiales;  // Como los materiales son abstractos uso punteros inteligentes
  };

}  // namespace render
#endif
