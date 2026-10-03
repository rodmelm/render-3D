set(COMMON_SRC_FILES 
  "${CMAKE_SOURCE_DIR}/common/src/vector.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/ray.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/matte.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/metal.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/refractive.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/sphere.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/camera.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/cilinder.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/engine.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/random_system.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/scene.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/scene_parser.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/config_parser.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/ppm_writer.cpp"
)
 
set(CURRENT_DIR_SRC_FILES 
  "${CMAKE_CURRENT_SOURCE_DIR}/test_vector.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_ray.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_matte.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_metal.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_refractive.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_sphere.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_camera.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_cilinder.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_engine.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_random_system.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_scene.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_scene_parser.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_config_parser.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_ppm_writer.cpp"
)

add_unit_test_target(
  TARGET_NAME utcommon
  SOURCE_FILES ${COMMON_SRC_FILES} ${CURRENT_DIR_SRC_FILES}
  LIBRARY_FILTER common
  COVERAGE_DIR coverage-common
  LIBRARY_TO_LINK common
)
