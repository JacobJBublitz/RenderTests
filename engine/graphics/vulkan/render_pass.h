#ifndef ENGINE_GRAPHICS_VULKAN_RENDER_PASS_H_
#define ENGINE_GRAPHICS_VULKAN_RENDER_PASS_H_

#include <optional>
#include <vector>

#include "vulkan/vulkan.hpp"

namespace engine::graphics::vulkan {

struct VulkanSubpass final {
  vk::PipelineBindPoint BindPoint;
  std::vector<vk::AttachmentReference> InputAttachments;
  std::vector<vk::AttachmentReference> ColorAttachments;
  std::optional<vk::AttachmentReference> DepthStencilAttachment;
  std::vector<uint32_t> PreserveAttachments;
};

class VulkanRenderPass final {
 public:
  VulkanRenderPass(vk::Device device, vk::RenderPass handle)
      : device_(device), handle_(handle) {}
  ~VulkanRenderPass() { device_.destroyRenderPass(handle_); }

  operator vk::RenderPass() noexcept { return handle_; }

 private:
  vk::Device device_;
  vk::RenderPass handle_;
};

}  // namespace engine::graphics::vulkan

#endif
