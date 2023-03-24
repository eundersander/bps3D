# Copyright (c) Meta Platforms, Inc. All Rights Reserved
if (BPS3D_INCLUDE_HABITAT_IO)
message("BPS3D_INCLUDE_HABITAT_IO enabled for preprocess")
add_definitions(-DBPS3D_INCLUDE_HABITAT_IO)
endif()

add_library(bps3D_preprocess SHARED
    gltf.hpp gltf.inl
    habitat_json.hpp habitat_json.inl
    import.hpp import.cpp
    ../../include/bps3D/preprocess.hpp preprocess.cpp
)

if (BPS3D_INCLUDE_HABITAT_IO)
target_link_libraries(bps3D_preprocess
    PRIVATE
        bps3D_core
        meshoptimizer
        simdjson
        habitat_io
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
