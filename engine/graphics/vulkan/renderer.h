#ifndef ENGINE_GRAPHICS_VULKAN_RENDERER_H_
#define ENGINE_GRAPHICS_VULKAN_RENDERER_H_

#include "vulkan/vulkan.hpp"

namespace engine::graphics::vulkan {

class VkRenderer {
 public:
  VkRenderer();
  ~VkRenderer();

 private:
  vk::Instance instance_;
};

}  // namespace engine::graphics::vulkan

#endif
