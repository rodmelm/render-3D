#include "random_system.hpp"

#include <tbb/enumerable_thread_specific.h>
#include <tbb/task_arena.h>

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

namespace render {

  tbb::enumerable_thread_specific<std::mt19937_64> random_system::material_gens_;
  tbb::enumerable_thread_specific<std::mt19937_64> random_system::ray_gens_;
  std::vector<std::uint64_t> random_system::material_seeds_vector_;
  std::vector<std::uint64_t> random_system::ray_seeds_vector_;

  void random_system::init_material_gen(std::uint64_t material_seed) {
    auto num_threads = tbb::this_task_arena::max_concurrency();
    if (num_threads == 0) {
      num_threads = 4;  // Fallback
    }

    material_seeds_vector_.resize(static_cast<std::size_t>(num_threads));
    std::mt19937_64 const seed_gen(material_seed);
    std::ranges::generate(material_seeds_vector_, seed_gen);

    material_gens_ = tbb::enumerable_thread_specific<std::mt19937_64>([num_threads]() {
      static std::atomic<std::size_t> counter{0};
      std::size_t const next_id = counter++;

      std::uint64_t const my_seed =
          material_seeds_vector_[next_id % static_cast<std::size_t>(num_threads)];
      return std::mt19937_64(my_seed);
    });
  }

  void random_system::init_ray_gen(std::uint64_t ray_seed) {
    auto num_threads = tbb::this_task_arena::max_concurrency();
    if (num_threads == 0) {
      num_threads = 4;  // Fallback
    }

    ray_seeds_vector_.resize(static_cast<std::size_t>(num_threads));
    std::mt19937_64 const seed_gen(ray_seed);
    std::ranges::generate(ray_seeds_vector_, seed_gen);

    ray_gens_ = tbb::enumerable_thread_specific<std::mt19937_64>([num_threads]() {
      static std::atomic<std::size_t> counter{0};
      std::size_t const next_id = counter++;

      std::uint64_t const my_seed =
          ray_seeds_vector_[next_id % static_cast<std::size_t>(num_threads)];
      return std::mt19937_64(my_seed);
    });
  }

  double random_system::random_double_material(double min, double max) {
    auto & material_gen = get_material_gen();
    std::uniform_real_distribution<double> dist(min, max);
    return dist(material_gen);
  }

  double random_system::random_double_ray(double min, double max) {
    auto & ray_gen = get_ray_gen();
    std::uniform_real_distribution<double> dist(min, max);
    return dist(ray_gen);
  }

  std::mt19937_64 & random_system::get_material_gen() {
    return material_gens_.local();
  }

  std::mt19937_64 & random_system::get_ray_gen() {
    return ray_gens_.local();
  }

}  // namespace render
