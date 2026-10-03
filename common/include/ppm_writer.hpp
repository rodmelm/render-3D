#ifndef RENDER_PPM_WRITER_HPP
#define RENDER_PPM_WRITER_HPP
#include "image_aos.hpp"
#include "image_soa.hpp"
#include <string>

namespace render {

  class ppm_writer {
  public:
    ppm_writer();
    static void write_image_aos(std::string const & filename, image_aos const & img);
    static void write_image_soa(std::string const & filename, image_soa const & img);
  };

}  // namespace render
#endif  // RENDER_PPM_WRITER_HPP
