add_library(bps3D_preprocess SHARED
    gltf.hpp gltf.inl
    habitat_json.hpp habitat_json.inl
    import.hpp import.cpp
    ../../include/bps3D/preprocess.hpp preprocess.cpp
)

if (BPS3D_INCLUDE_HABITAT_IO)
# This rapidjson stuff is required by habitat_io. I'm not sure why it's not getting inherited automatically when linking with habitat_io below
find_package(RapidJSON CONFIG REQUIRED)
include_directories(SYSTEM ${RapidJSON_INCLUDE_DIR})
endif()

if (BPS3D_INCLUDE_HABITAT_IO)
target_link_libraries(bps3D_preprocess
    PRIVATE
        bps3D_core
        meshoptimizer
        simdjson
        habitat_io
        rapidjson
    PUBLIC
        glm
)
else()
target_link_libraries(bps3D_preprocess
    PRIVATE
        bps3D_core
        meshoptimizer
        simdjson
    PUBLIC
        glm
)
endif()

target_include_directories(bps3D_preprocess
    INTERFACE
        ${MAIN_INCLUDE_DIR}
)
