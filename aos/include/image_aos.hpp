#ifndef RENDER_IMAGE_AOS_HPP
#define RENDER_IMAGE_AOS_HPP

#include <cstdint>
#include <vector>

namespace render {

  class image_aos {
  public:
    struct pixel {
      std::uint8_t r, g, b;

      pixel() : r(0), g(0), b(0) { }

      pixel(std::uint8_t cr, std::uint8_t cg, std::uint8_t cb) : r(cr), g(cg), b(cb) { }
    };

    /*image_aos(std::size_t width, std::size_t height)
        : width_(width), height_(height),
          pixels_(static_cast<std::vector<pixel>::size_type>(width) *
                  static_cast<std::vector<pixel>::size_type>(height)) { }
    */

    image_aos(std::size_t width, std::size_t height) : width_(width), height_(height) {
      pixels_.resize(width * height);
    }

    /*
     void set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
         pixels_[y * width_ + x] = pixel(r, g, b);
     }
 */

    void set_pixel(std::size_t x, std::size_t y, pixel p) { pixels_[y * width_ + x] = p; }

    [[nodiscard]] pixel get_pixel(std::size_t x, std::size_t y) const {
      return pixels_[y * width_ + x];
    }

    [[nodiscard]] std::size_t width() const { return width_; }

    [[nodiscard]] std::size_t height() const { return height_; }

    [[nodiscard]] std::vector<pixel> const & data() const { return pixels_; }

  private:
    std::size_t width_;
    std::size_t height_;
    std::vector<pixel> pixels_;
  };

}  // namespace render

#endif
