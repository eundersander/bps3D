// Copyright (c) Meta Platforms, Inc. All Rights Reserved
#pragma once

#include <bps3D/utils.hpp>
#include <glm/glm.hpp>

#include <optional>
#include <string_view>

namespace bps3D {

struct PreprocessData;

class ScenePreprocessor {
public:
    ScenePreprocessor(std::string_view gltf_path,
                      const glm::mat4 &base_txfm,
                      std::optional<std::string_view> texture_dir,
                      bool dump_textures,
                      bool doWriteInstances);

    void dump(std::string_view out_path);

private:
    Handle<PreprocessData> scene_data_;
    bool doWriteInstances_ = true;
};

}
