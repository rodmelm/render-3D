#ifndef RENDER_IMAGE_SOA_HPP
#define RENDER_IMAGE_SOA_HPP

#include <cstdint>
#include <vector>

namespace render {

  class image_soa {
  public:
    struct pixel {
      std::uint8_t r, g, b;

      pixel() : r(0), g(0), b(0) { }

      pixel(std::uint8_t cr, std::uint8_t cg, std::uint8_t cb) : r(cr), g(cg), b(cb) { }
    };

    image_soa(std::size_t width, std::size_t height) : width_(width), height_(height) {
      r_vector.resize(width * height, 0);
      g_vector.resize(width * height, 0);
      b_vector.resize(width * height, 0);
    }

    void set_pixel(std::size_t x, std::size_t y, pixel p) {
      std::size_t const index = y * width_ + x;
      r_vector[index]         = p.r;
      g_vector[index]         = p.g;
      b_vector[index]         = p.b;
    }

    [[nodiscard]] pixel get_pixel(std::size_t x, std::size_t y) const {
      std::size_t const index = y * width_ + x;
      return {r_vector[index], g_vector[index], b_vector[index]};
    }

    [[nodiscard]] std::size_t width() const { return width_; }

    [[nodiscard]] std::size_t height() const { return height_; }

    [[nodiscard]] std::vector<uint8_t> const & r_data() const { return r_vector; }

    [[nodiscard]] std::vector<uint8_t> const & g_data() const { return g_vector; }

    [[nodiscard]] std::vector<uint8_t> const & b_data() const { return b_vector; }

  private:
    std::size_t width_, height_;
    std::vector<uint8_t> r_vector;
    std::vector<uint8_t> g_vector;
    std::vector<uint8_t> b_vector;
  };

}  // namespace render

#endif
