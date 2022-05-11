#pragma once
#include "engine/renderer/renderer.h"

namespace engine {
namespace game_object {

class ObjectMesh {
    const renderer::DeviceInfo& device_info_;
    renderer::BufferInfo vertex_buffer_;
public:
    ObjectMesh(const renderer::DeviceInfo& device_info, const std::string& object_name);
};

} // game_object
} // engine