#include <gtest/gtest.h>

#include "scene_parser.hpp"
#include <filesystem>
#include <fstream>
#include <span>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

namespace {

  // Necesitamos un scene file para las pruebas
  class test_scene_parser : public ::testing::Test {
  protected:
    std::filesystem::path filepath;
    std::vector<std::string> strs;
    std::vector<char *> cstrs;

    void SetUp() override {
      // Create a temporary scene file
      auto tmpdir = std::filesystem::temp_directory_path();
      filepath    = tmpdir / "test_scene_parser.scene";
      std::ofstream ofs(filepath);
      ofs << "matte: mat1 0.5 0.4 0.3\n";
      ofs << "metal: met1 0.8 0.7 0.6 0.2\n";
      ofs << "sphere: 0 0 -1 0.5 mat1\n";
      ofs << "cylinder: 1 0 -2 0.3 0 1 0 met1\n";
      ofs.close();
      // Prepare argv-like span of 4 elements (scene_parser expects args[2] to be the file path)
      std::string const prog     = "test_prog";
      std::string const u1       = "unused1";
      std::string const path_str = filepath.string();
      std::string const u3       = "unused3";
      strs                       = {prog, u1, path_str, u3};
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

  class test_scene_parser_errors_materiales_duplicados : public ::testing::Test {
  protected:
    std::filesystem::path filepath;
    std::vector<std::string> strs;
    std::vector<char *> cstrs;

    void SetUp() override {
      // create a scene file with duplicate material names
      auto tmpdir = std::filesystem::temp_directory_path();
      filepath    = tmpdir / "dup_mat.scene";
      std::ofstream ofs(filepath);
      ofs << "matte: mat1 0.1 0.2 0.3\n";
      ofs << "matte: mat1 0.4 0.5 0.6\n";
      ofs.close();
      // Prepare argv-like span of 4 elements (scene_parser expects args[2] to be the file path)
      std::string const prog = "p";
      std::string const u1   = "u1";
      std::string const path = filepath.string();
      std::string const u3   = "u3";
      strs                   = {prog, u1, path, u3};
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

  class test_scene_parser_errors_material_parametro_invalido : public ::testing::Test {
  protected:
    std::filesystem::path filepath;
    std::vector<std::string> strs;
    std::vector<char *> cstrs;

    void SetUp() override {
      // invalid numeric token in material
      auto tmpdir   = std::filesystem::temp_directory_path();
      auto filepath = tmpdir / "bad_mat.scene";
      std::ofstream ofs(filepath);
      ofs << "matte: m1 0.5 not_a_number 0.3\n";
      ofs.close();
      // Prepare argv-like span of 4 elements (scene_parser expects args[2] to
      std::string const prog        = "p";
      std::string const u1          = "u1";
      std::string const path        = filepath.string();
      std::string const u3          = "u3";
      std::vector<std::string> strs = {prog, u1, path, u3};
      std::vector<char *> cstrs;
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

  class test_scene_parser_errors_material_desconocido : public ::testing::Test {
  protected:
    std::filesystem::path filepath;
    std::vector<std::string> strs;
    std::vector<char *> cstrs;

    void SetUp() override {
      // object references unknown material
      auto tmpdir   = std::filesystem::temp_directory_path();
      auto filepath = tmpdir / "missing_mat.scene";
      std::ofstream ofs(filepath);
      ofs << "sphere: 0 0 -1 0.5 unknown_mat\n";
      ofs.close();
      // Prepare argv-like span of 4 elements (scene_parser expects args[2] to be the file path)
      std::string const prog        = "p";
      std::string const u1          = "u1";
      std::string const path        = filepath.string();
      std::string const u3          = "u3";
      std::vector<std::string> strs = {prog, u1, path, u3};
      std::vector<char *> cstrs;
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

  class test_scene_parser_errors_argumentos_invalidos : public ::testing::Test {
  protected:
    std::filesystem::path filepath;
    std::vector<std::string> strs;
    std::vector<char *> cstrs;

    void SetUp() override {
      // call leer with incorrect span size
      auto tmpdir   = std::filesystem::temp_directory_path();
      auto filepath = tmpdir / "small_span.scene";
      std::ofstream ofs(filepath);
      ofs << "matte: m 0.1 0.2 0.3\n";
      ofs.close();
      std::string const prog = "p";
      // only 3 elements
      std::vector<std::string> strs = {prog, filepath.string(), "only3"};
      std::vector<char *> cstrs;
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

  TEST_F(test_scene_parser, test_materiales_shared) {
    render::scene_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    auto mats = parser.get_materiales_shared();
    EXPECT_TRUE(mats.contains("mat1"));
    EXPECT_TRUE(mats.contains("met1"));
  }

  TEST_F(test_scene_parser, test_objetos_esfera_shared) {
    render::scene_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    auto spheres = parser.get_objetos_esfera_shared();
    EXPECT_EQ(spheres.size(), 1U);
  }

  TEST_F(test_scene_parser, test_objetos_cilindro_shared) {
    render::scene_parser parser;
    parser.leer(std::span<char *>(cstrs.data(), cstrs.size()));
    auto cilindros = parser.get_objetos_cilindro_shared();
    EXPECT_EQ(cilindros.size(), 1U);
  }

  TEST_F(test_scene_parser_errors_materiales_duplicados, duplicate_material_name) {
    render::scene_parser parser;
    ASSERT_THROW(parser.leer(std::span<char *>(cstrs.data(), cstrs.size())), std::runtime_error);
  }

  TEST_F(test_scene_parser_errors_material_parametro_invalido, material_parse_error) {
    render::scene_parser parser;
    ASSERT_THROW(parser.leer(std::span<char *>(cstrs.data(), cstrs.size())), std::runtime_error);

    std::error_code ec;
    std::filesystem::remove(filepath, ec);
  }

  TEST_F(test_scene_parser_errors_material_desconocido, missing_material_for_object) {
    render::scene_parser parser;
    ASSERT_THROW(parser.leer(std::span<char *>(cstrs.data(), cstrs.size())), std::runtime_error);
  }

  TEST_F(test_scene_parser_errors_argumentos_invalidos, invalid_span_size) {
    render::scene_parser parser;
    ASSERT_THROW(parser.leer(std::span<char *>(cstrs.data(), cstrs.size())), std::runtime_error);
  }

}  // namespace
