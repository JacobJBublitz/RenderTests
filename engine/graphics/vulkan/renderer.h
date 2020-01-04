#ifndef ENGINE_GRAPHICS_VULKAN_RENDERER_H_
#define ENGINE_GRAPHICS_VULKAN_RENDERER_H_

#include <list>
#include <memory>
#include <vector>

#include "engine/graphics/vulkan/surface.h"
#include "vulkan/vulkan.hpp"

namespace engine::graphics::vulkan {

class VkNativeDisplay {
 public:
  virtual ~VkNativeDisplay() = default;

  virtual bool DoesVkQueueSupportPresentation(
      vk::PhysicalDevice physical_device, uint32_t queue_family_index) = 0;

  virtual const std::list<const char *> &GetRequiredVkExtensions() = 0;
};

class VkRenderer {
 public:
  VkRenderer(VkNativeDisplay &native_display,
             VulkanNativeSurface &native_surface);
  ~VkRenderer();

 private:
  vk::UniqueInstance instance_;
  VulkanSurface surface_;
  vk::Device device_;
  vk::SwapchainKHR swapchain_;

  std::vector<vk::Image> swapchain_images_;
  std::vector<vk::ImageView> swapchain_image_views_;

  vk::Queue graphics_queue_;
  vk::Queue present_queue_;
};

}  // namespace engine::graphics::vulkan

#endif
