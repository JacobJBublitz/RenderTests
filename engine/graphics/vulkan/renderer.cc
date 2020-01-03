#include "renderer.h"

namespace engine::graphics::vulkan {

namespace {

const vk::ApplicationInfo kApplicationInfo = {"Engine", 1};

}  // namespace

VkRenderer::VkRenderer() {
  vk::InstanceCreateInfo instance_create_info{{}, &kApplicationInfo};

  instance_ = vk::createInstance(instance_create_info);
}

VkRenderer::~VkRenderer() { instance_.destroy(); }

}  // namespace engine::graphics::vulkan
