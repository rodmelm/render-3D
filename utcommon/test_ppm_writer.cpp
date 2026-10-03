#include <gtest/gtest.h>

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

#include "image_aos.hpp"
#include "image_soa.hpp"
#include "ppm_writer.hpp"

namespace {

  struct ppm_data {
    std::string magic;
    std::size_t w{};
    std::size_t h{};
    int maxval{};
    std::vector<int> pixels;
  };

  ppm_data read_ppm(std::filesystem::path const & path) {
    ppm_data out;
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
      throw std::runtime_error("could not open ppm file");
    }
    ifs >> out.magic >> out.w >> out.h >> out.maxval;
    out.pixels.assign(std::istream_iterator<int>(ifs), std::istream_iterator<int>());
    return out;
  }

  TEST(test_ppm_writer, write_image_aos_and_verify_contents) {
    using namespace render;
    auto tmp = std::filesystem::temp_directory_path();
    auto out = tmp / "test_aos.ppm";

    image_aos img(2, 2);
    img.set_pixel(0, 0, image_aos::pixel(10, 20, 30));
    img.set_pixel(1, 0, image_aos::pixel(40, 50, 60));
    img.set_pixel(0, 1, image_aos::pixel(70, 80, 90));
    img.set_pixel(1, 1, image_aos::pixel(100, 110, 120));

    ppm_writer::write_image_aos(out.string(), img);

    auto data = read_ppm(out);

    EXPECT_EQ(data.magic, "P3");
    EXPECT_EQ(data.w, static_cast<std::size_t>(2));
    EXPECT_EQ(data.h, static_cast<std::size_t>(2));
    EXPECT_EQ(data.maxval, 255);

    // expected order: (0,0),(1,0),(0,1),(1,1) each as r g b
    std::vector<int> const expected = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};
    EXPECT_EQ(data.pixels, expected);

    std::error_code ec;
    std::filesystem::remove(out, ec);
  }

  TEST(test_ppm_writer, write_image_soa_and_aos_produce_same_file) {
    using namespace render;
    auto tmp  = std::filesystem::temp_directory_path();
    auto out1 = tmp / "test_soa.ppm";
    auto out2 = tmp / "test_aos2.ppm";

    image_soa imgsoa(3, 1);
    image_aos imgaos(3, 1);

    // set three pixels
    imgsoa.set_pixel(0, 0, image_soa::pixel(1, 2, 3));
    imgsoa.set_pixel(1, 0, image_soa::pixel(4, 5, 6));
    imgsoa.set_pixel(2, 0, image_soa::pixel(7, 8, 9));

    imgaos.set_pixel(0, 0, image_aos::pixel(1, 2, 3));
    imgaos.set_pixel(1, 0, image_aos::pixel(4, 5, 6));
    imgaos.set_pixel(2, 0, image_aos::pixel(7, 8, 9));

    ppm_writer::write_image_soa(out1.string(), imgsoa);
    ppm_writer::write_image_aos(out2.string(), imgaos);

    auto d1 = read_ppm(out1);
    auto d2 = read_ppm(out2);

    EXPECT_EQ(d1.magic, "P3");
    EXPECT_EQ(d2.magic, "P3");
    EXPECT_EQ(d1.w, d2.w);
    EXPECT_EQ(d1.h, d2.h);
    EXPECT_EQ(d1.maxval, d2.maxval);
    EXPECT_EQ(d1.pixels, d2.pixels);

    std::error_code ec;
    std::filesystem::remove(out1, ec);
    std::filesystem::remove(out2, ec);
  }

  TEST(test_ppm_writer, write_image_aos_invalid_path_throws) {
    using namespace render;
    auto tmp = std::filesystem::temp_directory_path();
    auto bad_dir = tmp / "dir_does_not_exist_12345";
    auto out = bad_dir / "file.ppm";

    image_aos img(1, 1);
    img.set_pixel(0, 0, image_aos::pixel(0, 0, 0));

    EXPECT_THROW(ppm_writer::write_image_aos(out.string(), img), std::runtime_error);
  }

  TEST(test_ppm_writer, write_image_soa_invalid_path_throws) {
    using namespace render;
    auto tmp = std::filesystem::temp_directory_path();
    auto bad_dir = tmp / "dir_does_not_exist_12345";
    auto out = bad_dir / "file2.ppm";

    image_soa img(1, 1);
    img.set_pixel(0, 0, image_soa::pixel(0, 0, 0));

    EXPECT_THROW(ppm_writer::write_image_soa(out.string(), img), std::runtime_error);
  }

}  // namespace
