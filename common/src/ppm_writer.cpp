#include "ppm_writer.hpp"
#include "image_aos.hpp"
#include "image_soa.hpp"
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>

namespace render {

  void ppm_writer::write_image_aos(std::string const & filename, image_aos const & img) {
    std::ofstream archivo(filename);
    if (!archivo.is_open()) [[unlikely]] {
      throw std::runtime_error("Error: Could not open file for writing: " + filename);
    }

    archivo << "P3\n" << img.width() << " " << img.height() << "\n255\n";

    for (std::size_t j = 0; j < img.height(); ++j) [[likely]] {
      for (std::size_t i = 0; i < img.width(); ++i) [[likely]] {
        auto pixel = img.get_pixel(i, j);
        archivo << static_cast<int>(pixel.r) << " " << static_cast<int>(pixel.g) << " "
                << static_cast<int>(pixel.b) << "\n";
      }
    }
    archivo.close();
  }

  void ppm_writer::write_image_soa(std::string const & filename, image_soa const & img) {
    std::ofstream archivo(filename);
    if (!archivo.is_open()) [[unlikely]] {
      throw std::runtime_error("Error: Could not open file for writing: " + filename);
    }

    archivo << "P3\n" << img.width() << " " << img.height() << "\n255\n";

    for (std::size_t j = 0; j < img.height(); ++j) [[likely]] {
      for (std::size_t i = 0; i < img.width(); ++i) [[likely]] {
        auto pixel = img.get_pixel(i, j);
        archivo << static_cast<int>(pixel.r) << " " << static_cast<int>(pixel.g) << " "
                << static_cast<int>(pixel.b) << "\n";
      }
    }
    archivo.close();
  }

}  // namespace render
