#ifndef ENGINE_GRAPHICS_VULKAN_PIPELINE_H_
#define ENGINE_GRAPHICS_VULKAN_PIPELINE_H_

#include <string>
#include <vector>

#include "vulkan/vulkan.hpp"

namespace engine::graphics::vulkan {

class VulkanShader final {
 public:
  VulkanShader(vk::Device device, vk::ShaderModule handle)
      : device_(device), handle_(handle) {}
  VulkanShader(const VulkanShader &) = delete;
  ~VulkanShader() { device_.destroyShaderModule(handle_); }

  static VulkanShader FromSpirV(vk::Device device,
                                const std::vector<uint8_t> &spirv_binary);

  operator vk::ShaderModule() noexcept { return handle_; }

 private:
  vk::Device device_;
  vk::ShaderModule handle_;
};

enum class VulkanShaderStage { kVertex, kFragment };

struct VulkanShaderStageInfo {
  VulkanShader *Shader;
  VulkanShaderStage Stage;
  std::string EntryPoint;
};

class VulkanPipelineLayout {
 public:
  VulkanPipelineLayout(vk::Device device, vk::Extent2D swapchain_extent);
  VulkanPipelineLayout(const VulkanPipelineLayout &) = delete;

  operator vk::PipelineLayout() noexcept { return handle_.get(); }

 private:
  vk::Device device_;
  vk::UniquePipelineLayout handle_;
};

class VulkanPipeline {
 public:
  VulkanPipeline(vk::Device device, vk::Extent2D swapchain_extent,
                 const std::vector<VulkanShaderStageInfo> &stages);
  VulkanPipeline(const VulkanPipeline &) = delete;
  ~VulkanPipeline();

 private:
  vk::Device device_;
  vk::UniquePipelineLayout layout_;
  vk::Pipeline handle_;
};

}  // namespace engine::graphics::vulkan

#endif
