#ifndef ENGINE_GRAPHICS_VULKAN_DEVICE_H_
#define ENGINE_GRAPHICS_VULKAN_DEVICE_H_

#include <vector>

#include "vulkan/vulkan.hpp"

namespace engine::graphics::vulkan {

class VulkanDevice;

class VulkanPhysicalDevice {
 public:
  VulkanPhysicalDevice(vk::PhysicalDevice handle);

 private:
  vk::PhysicalDevice handle_;

  vk::SurfaceCapabilitiesKHR surface_capabilities_;
  std::vector<vk::SurfaceFormatKHR> surface_formats_;
  std::vector<vk::PresentModeKHR> present_modes_;
};

class VulkanDevice {
 public:
  VulkanDevice(vk::Device &handle,
               const VulkanPhysicalDevice &physical_device) noexcept
      : handle_(handle), physical_device_(physical_device) {}
  VulkanDevice(const VulkanDevice &) = delete;
  ~VulkanDevice() { handle_.destroy(); }

  vk::Device GetHandle() noexcept { return handle_; }

  VulkanPhysicalDevice &GetPhysicalDevice() noexcept {
    return physical_device_;
  }

  const VulkanPhysicalDevice &GetPhysicalDevice() const noexcept {
    return physical_device_;
  }

  vk::Device *operator->() noexcept { return &handle_; }

  operator vk::Device() noexcept { return handle_; }

 private:
  vk::Device handle_;
  VulkanPhysicalDevice physical_device_;
};

}  // namespace engine::graphics::vulkan

#endif
