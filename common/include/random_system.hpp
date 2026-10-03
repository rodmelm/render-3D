#ifndef RENDER_RANDOM_SYSTEM_HPP
#define RENDER_RANDOM_SYSTEM_HPP

#include <cstdint>
#include <random>
#include <tbb/enumerable_thread_specific.h>
#include <vector>

namespace render {

  class random_system {
    // Vamos a hacer la generacion de numeros aleatorios de manera que podamos paralelizar con hilos
    // sin que haya condiciones de carrera.
  public:
    static void init_material_gen(std::uint64_t material_seed);
    static void init_ray_gen(std::uint64_t ray_seed);

    static double random_double_material(double min, double max);
    static double random_double_ray(double min, double max);

    static std::mt19937_64 & get_material_gen();
    static std::mt19937_64 & get_ray_gen();

  private:
    static tbb::enumerable_thread_specific<std::mt19937_64> material_gens_;
    static tbb::enumerable_thread_specific<std::mt19937_64> ray_gens_;
    // Semillas base para los generadores
    static std::vector<std::uint64_t> material_seeds_vector_;
    static std::vector<std::uint64_t> ray_seeds_vector_;
  };

}  // namespace render
#endif
