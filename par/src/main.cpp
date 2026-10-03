#include "camera.hpp"
#include "camera_config.hpp"
#include "config_parser.hpp"
#include "engine.hpp"
#include "image_soa.hpp"
#include "ppm_writer.hpp"
#include "random_system.hpp"
#include "scene.hpp"
#include "scene_parser.hpp"
#include "vector.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <oneapi/tbb/global_control.h>  // Necesario para limitar hilos
#include <span>
#include <stdexcept>
#include <string>

namespace {

  // No sé si esto nos hace falta, porque en leer parse lo comprueba creo, pero por si acaso
  // de momento lo dejo por aquí
  void validar_argumentos(int argc) {
    if (argc != 4) {
      std::cerr << "Error: Invalid number of arguments: " << argc - 1 << "\n";
      throw std::runtime_error("Invalid number of arguments");
    }
  }

  render::camera configurar_camara(render::config_parser const & config_parser) {
    auto const parse_config = config_parser.get_config();
    render::camera_config cam_config;
    cam_config.posicion     = parse_config.camera_position.value_or(render::vector{0, 0, -10});
    cam_config.destino      = parse_config.camera_target.value_or(render::vector{0, 0, 0});
    cam_config.norte        = parse_config.camera_north.value_or(render::vector{0, 1, 0});
    cam_config.fov          = parse_config.camera_fov.value_or(90.0);
    cam_config.image_width  = parse_config.image_width.value_or(1'920);
    cam_config.aspect_ratio = parse_config.aspect_ratio.value_or(16.0 / 9.0);
    return render::camera{cam_config};
  }

  render::engine::config configurar_motor(render::config_parser const & config_parser) {
    auto const parse_config = config_parser.get_config();
    render::engine::config motor_config;
    motor_config.max_depth         = parse_config.max_depth.value_or(5);
    motor_config.samples_per_pixel = parse_config.num_samples.value_or(20);
    motor_config.gamma             = parse_config.gamma.value_or(2.2);
    motor_config.background_dark_color =
        parse_config.background_dark_color.value_or(render::vector{0.25, 0.5, 1.0});
    motor_config.background_light_color =
        parse_config.background_light_color.value_or(render::vector{1.0, 1.0, 1.0});
    return motor_config;
  }

  void configurar_random_system(render::config_parser const & config_parser) {
    auto const parse_config = config_parser.get_config();
    int const material_seed = parse_config.material_rng_seed.value_or(0);
    int const ray_seed      = parse_config.ray_rng_seed.value_or(0);

    render::random_system::init_material_gen(static_cast<std::uint64_t>(material_seed));
    render::random_system::init_ray_gen(static_cast<std::uint64_t>(ray_seed));
  }

  render::scene crear_escena(render::scene_parser const & scene_parser) {
    render::scene escena;

    auto const & esferas = scene_parser.get_objetos_esfera_shared();
    for (auto const & esfera : esferas) {
      escena.add_sphere(esfera);
    }

    auto const & cilindros = scene_parser.get_objetos_cilindro_shared();
    for (auto const & cilindro : cilindros) {
      escena.add_cylinder(cilindro);
    }

    return escena;
  }

  render::image_soa crear_imagen(render::config_parser const & config_parser) {
    auto const parse_config = config_parser.get_config();
    int const width         = parse_config.image_width.value_or(1'920);
    double const ratio      = parse_config.aspect_ratio.value_or(16.0 / 9.0);
    int const height        = static_cast<int>(width / ratio);
    return render::image_soa{static_cast<std::size_t>(width), static_cast<std::size_t>(height)};
  }

  std::unique_ptr<oneapi::tbb::global_control> configurar_tbb() {
    constexpr int num_hilos = 128;
    std::cout << "--> Ejecutando con " << num_hilos << " hilos.\n";
    return std::make_unique<oneapi::tbb::global_control>(
        oneapi::tbb::global_control::max_allowed_parallelism, num_hilos);
  }

}  // namespace

int main(int argc, char * argv[]) {
  try {
    // Configuración de paralelismo (si existe la variable de entorno)
    auto const gc = configurar_tbb();

    validar_argumentos(argc);
    std::span<char *> const args{argv, static_cast<std::size_t>(argc)};

    render::config_parser config_parser;
    config_parser.leer(args);
    config_parser.parser();

    render::scene_parser scene_parser;
    scene_parser.leer(args);

    configurar_random_system(config_parser);

    render::image_soa imagen = crear_imagen(config_parser);
    render::engine::render_scene(crear_escena(scene_parser), configurar_camara(config_parser),
                                 imagen, configurar_motor(config_parser));

    std::string const output_filename = args[3];
    render::ppm_writer::write_image_soa(output_filename, imagen);
    return 0;
  } catch (std::exception const & e) {
    std::cerr << "Exception: " << e.what() << "\n";
    return 1;
  }
}
