#ifndef ENGINE_GRAPHICS_VULKAN_SURFACE_H_
#define ENGINE_GRAPHICS_VULKAN_SURFACE_H_

#include "vulkan/vulkan.hpp"

namespace engine::graphics::vulkan {

class VulkanNativeSurface;

class VulkanSurface {
 public:
  VulkanSurface(vk::Instance instance, vk::SurfaceKHR handle,
                VulkanNativeSurface *native_surface)
      : instance_(instance), handle_(handle), native_surface_(native_surface) {}
  VulkanSurface(const VulkanSurface &) = delete;
  ~VulkanSurface() { instance_.destroySurfaceKHR(handle_); }

  vk::SurfaceKHR GetHandle() noexcept { return handle_; }

  VulkanNativeSurface *GetNativeSurface() noexcept { return native_surface_; }

  operator vk::SurfaceKHR() noexcept { return handle_; }

 private:
  vk::Instance instance_;
  vk::SurfaceKHR handle_;
  VulkanNativeSurface *native_surface_;
};

class VulkanNativeSurface {
 public:
  virtual ~VulkanNativeSurface() = default;

  virtual VulkanSurface CreateVulkanSurface(vk::Instance instance) = 0;

  virtual void GetVulkanSurfaceSize(uint32_t &width, uint32_t &height) = 0;
};

}  // namespace engine::graphics::vulkan

#endif
