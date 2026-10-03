#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <span>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

#include "config_parser.hpp"
#include "vector.hpp"

namespace {

  class test_config_parser : public ::testing::Test {
  protected:
    std::filesystem::path filepath;
    std::vector<std::string> strs;
    std::vector<char *> cstrs;

    void SetUp() override {
      // create a minimal valid config used by happy-path test
      auto tmpdir = std::filesystem::temp_directory_path();
      filepath    = tmpdir / "test_config_parser.cfg";
      std::ofstream ofs(filepath);
      ofs << "aspect_ratio: 16 9\n";
      ofs << "image_width: 400\n";
      ofs << "gamma: 2.2\n";
      ofs << "camera_position: 0 0 0\n";
      ofs << "camera_target: 0 0 -1\n";
      ofs << "camera_north: 0 1 0\n";
      ofs << "field_of_view: 90\n";
      ofs << "samples_per_pixel: 10\n";
      ofs << "max_depth: 5\n";
      ofs << "material_rng_seed: 42\n";
      ofs << "ray_rng_seed: 43\n";
      ofs << "background_dark_color: 0 0 0\n";
      ofs << "background_light_color: 1 1 1\n";
      ofs.close();

      std::string const prog = "test_prog";
      std::string const path = filepath.string();
      std::string const u1   = "unused1";
      std::string const u2   = "unused2";
      strs                   = {prog, path, u1, u2};
      cstrs.clear();
      cstrs.reserve(strs.size());
      for (auto & s : strs) {
        cstrs.push_back(s.data());
      }
    }

    void TearDown() override {
      std::error_code ec;
      std::filesystem::remove(filepath, ec);
    }
  };

  class test_config_parser_errors_parametro_extra : public ::testing::Test {
  protected:
    std::filesystem::path filepath;
    std::vector<std::string> strs;
    std::vector<char *> cstrs;

    void SetUp() override {
      // image_width with extra token should throw
      auto tmpdir = std::filesystem::temp_directory_path();
      filepath    = tmpdir / "extra.cfg";
      std::ofstream ofs(filepath);
      ofs << "image_width: 100 extra\n";
      ofs.close();

      std::string const prog = "p";
      std::string const path = filepath.string();
      std::string const u1   = "u1";
      std::string const u2   = "u2";
      strs                   = {prog, path, u1, u2};
      cstrs.clear();
      cstrs.reserve(strs.size());
      for (auto & s : strs) {
        cstrs.push_back(s.data());
      }
    }

    void TearDown() override {
      std::error_code ec;
      std::filesystem::remove(filepath, ec);
    }
  };

  class test_config_parser_errors_argumentos_invalidos : public ::testing::Test {
  protected:
    std::filesystem::path filepath;
    std::vector<std::string> strs;
    std::vector<char *> cstrs;

    void SetUp() override {
      auto tmpdir = std::filesystem::temp_directory_path();
      filepath    = tmpdir / "cfg_small.span";
      std::ofstream ofs(filepath);
      ofs << "gamma: 2.0\n";
      ofs.close();

      std::string const prog = "p";
      strs                   = {prog, filepath.string(), "only3"};
      cstrs.clear();
      cstrs.reserve(strs.size());
      for (auto & s : strs) {
        cstrs.push_back(s.data());
      }
    }

    void TearDown() override {
      std::error_code ec;
      std::filesystem::remove(filepath, ec);
    }
  };

  class test_config_parser_errors_parametro_invalido : public ::testing::Test {
  protected:
    std::filesystem::path filepath;
    std::vector<std::string> strs;
    std::vector<char *> cstrs;

    void SetUp() override {
      // create a config with invalid numeric token
      auto tmpdir = std::filesystem::temp_directory_path();
      filepath    = tmpdir / "bad_config.cfg";

      std::ofstream ofs(filepath);
      ofs << "gamma: not_a_number\n";
      ofs.close();

      std::string const prog = "p";
      std::string const path = filepath.string();
      std::string const u1   = "u1";
      std::string const u2   = "u2";
      strs                   = {prog, path, u1, u2};
      cstrs.clear();
      cstrs.reserve(strs.size());
      for (auto & s : strs) {
        cstrs.push_back(s.data());
      }
    }

    void TearDown() override {
      std::error_code ec;
      std::filesystem::remove(filepath, ec);
    }
  };

  TEST_F(test_config_parser, parse_config_file_aspect_ratio) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    // parsing stage
    parser.parser();

    auto cfg = parser.get_config();

    ASSERT_TRUE(cfg.aspect_ratio.has_value());
    if (cfg.aspect_ratio.has_value()) {
      EXPECT_DOUBLE_EQ(cfg.aspect_ratio.value(), 16.0 / 9.0);
    }
  }

  TEST_F(test_config_parser, parse_config_file_image_width) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    // parsing stage
    parser.parser();

    auto cfg = parser.get_config();
    ASSERT_TRUE(cfg.image_width.has_value());
    if (cfg.image_width.has_value()) {
      EXPECT_EQ(cfg.image_width.value(), 400);
    }
  }

  TEST_F(test_config_parser, parse_config_file_gamma) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    // parsing stage
    parser.parser();

    auto cfg = parser.get_config();

    ASSERT_TRUE(cfg.gamma.has_value());
    if (cfg.gamma.has_value()) {
      EXPECT_DOUBLE_EQ(cfg.gamma.value(), 2.2);
    }
  }

  TEST_F(test_config_parser, parse_config_file_camera_position) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    // parsing stage
    parser.parser();

    auto cfg = parser.get_config();
    ASSERT_TRUE(cfg.camera_position.has_value());
    if (cfg.camera_position.has_value()) {
      EXPECT_DOUBLE_EQ(cfg.camera_position->x, 0.0);
      EXPECT_DOUBLE_EQ(cfg.camera_position->y, 0.0);
      EXPECT_DOUBLE_EQ(cfg.camera_position->z, 0.0);
    }
  }

  TEST_F(test_config_parser, parse_config_file_camera_target) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    // parsing stage
    parser.parser();

    auto cfg = parser.get_config();
    ASSERT_TRUE(cfg.camera_target.has_value());
    if (cfg.camera_target.has_value()) {
      EXPECT_DOUBLE_EQ(cfg.camera_target->x, 0.0);
      EXPECT_DOUBLE_EQ(cfg.camera_target->y, 0.0);
      EXPECT_DOUBLE_EQ(cfg.camera_target->z, -1.0);
    }
  }

  TEST_F(test_config_parser, parse_config_camera_north) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    // parsing stage
    parser.parser();

    auto cfg = parser.get_config();
    ASSERT_TRUE(cfg.camera_north.has_value());
    if (cfg.camera_north.has_value()) {
      EXPECT_DOUBLE_EQ(cfg.camera_north->x, 0.0);
      EXPECT_DOUBLE_EQ(cfg.camera_north->y, 1.0);
      EXPECT_DOUBLE_EQ(cfg.camera_north->z, 0.0);
    }
  }

  TEST_F(test_config_parser, parse_config_file_camera_fov) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    // parsing stage
    parser.parser();

    auto cfg = parser.get_config();
    ASSERT_TRUE(cfg.camera_fov.has_value());
    if (cfg.camera_fov.has_value()) {
      EXPECT_DOUBLE_EQ(cfg.camera_fov.value(), 90.0);
    }
  }

  TEST_F(test_config_parser, parse_config_file_num_samples) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    // parsing stage
    parser.parser();

    auto cfg = parser.get_config();
    ASSERT_TRUE(cfg.num_samples.has_value());
    if (cfg.num_samples.has_value()) {
      EXPECT_EQ(cfg.num_samples.value(), 10);
    }
  }

  TEST_F(test_config_parser, parse_config_file_max_depth) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    // parsing stage
    parser.parser();

    auto cfg = parser.get_config();
    ASSERT_TRUE(cfg.max_depth.has_value());
    if (cfg.max_depth.has_value()) {
      EXPECT_EQ(cfg.max_depth.value(), 5);
    }
  }

  TEST_F(test_config_parser, parse_config_file_material_rng_seed) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    // parsing stage
    parser.parser();

    auto cfg = parser.get_config();
    ASSERT_TRUE(cfg.material_rng_seed.has_value());
    if (cfg.material_rng_seed.has_value()) {
      EXPECT_EQ(cfg.material_rng_seed.value(), 42);
    }
  }

  TEST_F(test_config_parser, parse_config_file_background_dark_color) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    // parsing stage
    parser.parser();

    auto cfg = parser.get_config();
    ASSERT_TRUE(cfg.background_dark_color.has_value());
    if (cfg.background_dark_color.has_value()) {
      EXPECT_DOUBLE_EQ(cfg.background_dark_color->x, 0.0);
      EXPECT_DOUBLE_EQ(cfg.background_dark_color->y, 0.0);
      EXPECT_DOUBLE_EQ(cfg.background_dark_color->z, 0.0);
    }
  }

  TEST_F(test_config_parser, parse_config_file_background_light_color) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    // parsing stage
    parser.parser();

    auto cfg = parser.get_config();
    ASSERT_TRUE(cfg.background_light_color.has_value());
    if (cfg.background_light_color.has_value()) {
      EXPECT_DOUBLE_EQ(cfg.background_light_color->x, 1.0);
      EXPECT_DOUBLE_EQ(cfg.background_light_color->y, 1.0);
      EXPECT_DOUBLE_EQ(cfg.background_light_color->z, 1.0);
    }
  }

  TEST_F(test_config_parser_errors_parametro_invalido, invalid_numeric_token) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    ASSERT_THROW(parser.parser(), std::runtime_error);
  }

  TEST_F(test_config_parser_errors_parametro_extra, extra_tokens_for_key) {
    render::config_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    ASSERT_THROW(parser.parser(), std::runtime_error);
  }

  TEST_F(test_config_parser_errors_argumentos_invalidos, invalid_span_size) {
    // span with size != 4 triggers leer exception

    render::config_parser parser;
    ASSERT_THROW(parser.leer(std::span<char *>(cstrs.data(), cstrs.size())), std::runtime_error);

    std::error_code ec;
    std::filesystem::remove(filepath, ec);
  }

}  // namespace
