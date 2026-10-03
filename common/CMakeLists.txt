add_library(common STATIC)

target_sources(common 
    PRIVATE 
        src/vector.cpp
        src/ray.cpp
        src/sphere.cpp
        src/cilinder.cpp
        src/material.cpp
        src/refractive.cpp
        src/matte.cpp
        src/metal.cpp
        src/scene.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/random_system.cpp
        src/engine.cpp
        src/camera.cpp
        src/config_parser.cpp
        src/ppm_writer.cpp
        src/scene_parser.cpp
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                            ${CMAKE_CURRENT_SOURCE_DIR}/src
                            ${CMAKE_CURRENT_SOURCE_DIR}/../aos/include
                            ${CMAKE_CURRENT_SOURCE_DIR}/../par/include
                            

)

target_link_libraries(common PUBLIC Microsoft.GSL::GSL TBB::tbb)
