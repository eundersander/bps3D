# Copyright (c) Meta Platforms, Inc. All Rights Reserved
add_library(
  habitat_io SHARED
  io.cpp
  io.h
  json.cpp
  json.h
  JsonAllTypes.h
  JsonBuiltinTypes.h
  JsonStlTypes.h
  JsonUtils.h
)

target_link_libraries(
  habitat_io
)

target_include_directories(habitat_io
    INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

