#pragma once
#include "engine/renderer/renderer.h"

namespace engine {
namespace game_object {

class BaseShape {
    const renderer::DeviceInfo& device_info_;
    renderer::BufferInfo vertex_buffer_;
public:
    BaseShape(const renderer::DeviceInfo& device_info) : device_info_(device_info){}
};

} // game_object
} // engine