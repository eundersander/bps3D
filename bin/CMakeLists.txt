add_executable(preprocess
    preprocess.cpp
)

if (BPS3D_INCLUDE_HABITAT_IO)
target_link_libraries(preprocess bps3D_preprocess habitat_io)
else()
target_link_libraries(preprocess bps3D_preprocess)
endif()

add_executable(singlebench
    singlebench.cpp
)
target_link_libraries(singlebench bps3D)

add_executable(save_frame
    save_frame.cpp
)
target_link_libraries(save_frame bps3D stb bps3D_debug)

find_package(glfw3 3.3)  # temp not quiet
find_package(GLEW)  # temp not quiet

if (glfw3_FOUND AND GLEW_FOUND)
    add_executable(fly
        fly.cpp
    )

    target_link_libraries(fly bps3D glfw GLEW GL)
endif()
