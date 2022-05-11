#pragma once
#include "engine/renderer/renderer.h"
#include "base_shape.h"

namespace engine {
namespace game_object {

class Patch : public BaseShape {
public:
Patch(const renderer::DeviceInfo& device_info) : BaseShape(device_info) {
}
};

} // game_object
} // engine