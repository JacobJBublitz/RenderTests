#include "pipeline.h"

#include "glog/logging.h"

namespace engine::graphics::vulkan {

VulkanShader VulkanShader::FromSpirV(vk::Device device,
                                     const std::vector<uint8_t> &spirv_binary) {
  vk::ShaderModuleCreateInfo create_info;
  create_info.codeSize = spirv_binary.size();
  create_info.pCode = reinterpret_cast<const uint32_t *>(spirv_binary.data());

  return {device, device.createShaderModule(create_info)};
}

VulkanPipelineLayout::VulkanPipelineLayout(vk::Device device,
                                           vk::Extent2D swapchain_extent) {
  vk::PipelineVertexInputStateCreateInfo vertex_input_info;
  vertex_input_info.vertexBindingDescriptionCount = 0;
  vertex_input_info.vertexAttributeDescriptionCount = 0;

  vk::PipelineInputAssemblyStateCreateInfo input_assembly_info;
  input_assembly_info.topology = vk::PrimitiveTopology::eTriangleList;
  input_assembly_info.primitiveRestartEnable = false;

  vk::Viewport viewport;
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = swapchain_extent.width;
  viewport.height = swapchain_extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  vk::Rect2D scissor;
  scissor.offset = vk::Offset2D{0, 0};
  scissor.extent = swapchain_extent;

  vk::PipelineViewportStateCreateInfo viewport_info;
  viewport_info.viewportCount = 1;
  viewport_info.pViewports = &viewport;
  viewport_info.scissorCount = 1;
  viewport_info.pScissors = &scissor;

  vk::PipelineRasterizationStateCreateInfo rasterizer_info;
  rasterizer_info.depthClampEnable = false;
  rasterizer_info.rasterizerDiscardEnable = false;
  rasterizer_info.polygonMode = vk::PolygonMode::eFill;
  rasterizer_info.lineWidth = 1.0f;
  rasterizer_info.cullMode = vk::CullModeFlagBits::eBack;
  rasterizer_info.frontFace = vk::FrontFace::eClockwise;
  rasterizer_info.depthBiasEnable = false;

  vk::PipelineMultisampleStateCreateInfo multisample_info;
  multisample_info.sampleShadingEnable = false;
  multisample_info.rasterizationSamples = vk::SampleCountFlagBits::e1;
  multisample_info.minSampleShading = 1.0f;
  multisample_info.alphaToCoverageEnable = false;
  multisample_info.alphaToOneEnable = false;

  vk::PipelineColorBlendAttachmentState color_blend_attachment;
  color_blend_attachment.colorWriteMask =
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
      vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
  color_blend_attachment.blendEnable = true;
  color_blend_attachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
  color_blend_attachment.dstColorBlendFactor =
      vk::BlendFactor::eOneMinusSrcAlpha;
  color_blend_attachment.colorBlendOp = vk::BlendOp::eAdd;
  color_blend_attachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
  color_blend_attachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
  color_blend_attachment.alphaBlendOp = vk::BlendOp::eAdd;

  vk::PipelineColorBlendStateCreateInfo color_blend_info;
  color_blend_info.logicOpEnable = false;
  color_blend_info.attachmentCount = 1;
  color_blend_info.pAttachments = &color_blend_attachment;

  std::vector<vk::DynamicState> dynamic_states{};

  vk::PipelineDynamicStateCreateInfo dynamic_state_info;
  dynamic_state_info.dynamicStateCount = dynamic_states.size();
  dynamic_state_info.pDynamicStates = dynamic_states.data();

  vk::PipelineLayoutCreateInfo pipeline_layout_info;

  handle_ = device_.createPipelineLayoutUnique(pipeline_layout_info);
}

VulkanPipeline::~VulkanPipeline() { device_.destroyPipeline(handle_); }

}  // namespace engine::graphics::vulkan
