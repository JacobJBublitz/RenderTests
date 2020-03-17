#include "device.h"

namespace engine::graphics::vulkan {

VulkanPhysicalDevice::VulkanPhysicalDevice(vk::PhysicalDevice handle) {}

VulkanRenderPass VulkanDevice::CreateRenderPass(
    const std::vector<vk::AttachmentDescription> &attachments,
    const std::vector<VulkanSubpass> &subpasses) {
  std::vector<vk::SubpassDescription> subpass_descriptions{subpasses.size()};
  for (size_t i = 0; i < subpasses.size(); i++) {
    auto &desc = subpass_descriptions[i];
    auto &subpass = subpasses[i];

    desc.pipelineBindPoint = subpass.BindPoint;
    desc.inputAttachmentCount = subpass.InputAttachments.size();
    desc.pInputAttachments = subpass.InputAttachments.data();
    desc.colorAttachmentCount = subpass.ColorAttachments.size();
    desc.pColorAttachments = subpass.ColorAttachments.data();
    if (subpass.DepthStencilAttachment)
      desc.pDepthStencilAttachment = &subpass.DepthStencilAttachment.value();
    desc.preserveAttachmentCount = subpass.PreserveAttachments.size();
    desc.pPreserveAttachments = subpass.PreserveAttachments.data();
  }

  vk::RenderPassCreateInfo create_info;
  create_info.attachmentCount = attachments.size();
  create_info.pAttachments = attachments.data();
  create_info.subpassCount = subpasses.size();
  create_info.pSubpasses = subpass_descriptions.data();

  return {handle_, handle_.createRenderPass(create_info)};
}

}  // namespace engine::graphics::vulkan
