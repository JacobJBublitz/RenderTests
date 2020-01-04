#include "renderer.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <list>
#include <optional>

#include "fmt/format.h"
#include "glog/logging.h"

namespace engine::graphics::vulkan {

namespace {

std::list<const char *> kRequiredInstanceExtensions = {
    VK_KHR_SURFACE_EXTENSION_NAME,
};

std::list<const char *> kRequiredDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

const vk::ApplicationInfo kApplicationInfo = {"Engine", 1};

class PhysicalDevice {
 public:
  PhysicalDevice(vk::PhysicalDevice handle, vk::SurfaceKHR surface)
      : handle_(handle) {
    auto queue_families = handle.getQueueFamilyProperties();
    for (uint32_t i = 0; i < queue_families.size(); i++) {
      auto &queue_family = queue_families[i];
      if (!graphics_family_ &&
          queue_family.queueFlags & vk::QueueFlagBits::eGraphics) {
        graphics_family_ = i;
      }
      if (!present_family_ && handle_.getSurfaceSupportKHR(i, surface)) {
        present_family_ = i;
      }
    }

    surface_capabilities_ = handle_.getSurfaceCapabilitiesKHR(surface);
    surface_formats_ = handle_.getSurfaceFormatsKHR(surface);
    present_modes_ = handle_.getSurfacePresentModesKHR(surface);
  }

  vk::PhysicalDevice GetHandle() noexcept { return handle_; }

  std::string GetName() const noexcept {
    return handle_.getProperties().deviceName;
  }

  std::optional<uint32_t> GetGraphicsFamily() const noexcept {
    return graphics_family_;
  }

  std::optional<uint32_t> GetPresentFamily() const noexcept {
    return present_family_;
  }

  const vk::SurfaceCapabilitiesKHR &GetSurfaceCapabilities() const noexcept {
    return surface_capabilities_;
  }

  const std::vector<vk::SurfaceFormatKHR> &GetSurfaceFormats() const noexcept {
    return surface_formats_;
  }

  const std::vector<vk::PresentModeKHR> &GetPresentModes() const noexcept {
    return present_modes_;
  }

 private:
  vk::PhysicalDevice handle_;

  std::optional<uint32_t> graphics_family_;
  std::optional<uint32_t> present_family_;

  vk::SurfaceCapabilitiesKHR surface_capabilities_;
  std::vector<vk::SurfaceFormatKHR> surface_formats_;
  std::vector<vk::PresentModeKHR> present_modes_;
};

vk::UniqueInstance CreateInstance(VkNativeDisplay &native_display) {
  auto available_instance_extensions =
      vk::enumerateInstanceExtensionProperties();
  std::vector<const char *> required_instance_extensions;
  required_instance_extensions.insert(required_instance_extensions.begin(),
                                      kRequiredInstanceExtensions.begin(),
                                      kRequiredInstanceExtensions.end());
  {
    auto display_required_extensions = native_display.GetRequiredVkExtensions();
    required_instance_extensions.insert(required_instance_extensions.end(),
                                        display_required_extensions.begin(),
                                        display_required_extensions.end());
  }

  for (auto &req_ext : required_instance_extensions) {
    bool found = false;
    for (auto &ext : available_instance_extensions) {
      if (std::strcmp(req_ext, ext.extensionName) == 0) {
        found = true;
        break;
      }
    }

    if (!found) {
      throw std::runtime_error(fmt::format(
          "Required Vulkan instance extension {0} was not found", req_ext));
    }
  }

  vk::InstanceCreateInfo create_info;
  create_info.setPApplicationInfo(&kApplicationInfo);
  create_info.setPpEnabledExtensionNames(required_instance_extensions.data());
  create_info.setEnabledExtensionCount(required_instance_extensions.size());

  return vk::createInstanceUnique(create_info);
}

vk::Device CreateDevice(PhysicalDevice &phys_device) {
  LOG(INFO) << "Using " << phys_device.GetName() << " as our device";

  // Device extensions
  std::vector<const char *> enabled_extensions;
  enabled_extensions.insert(enabled_extensions.end(),
                            kRequiredDeviceExtensions.begin(),
                            kRequiredDeviceExtensions.end());

  // Device queues
  LOG_IF(FATAL, !phys_device.GetGraphicsFamily())
      << "Chosen device doesn't have a graphics queue";
  LOG_IF(FATAL, !phys_device.GetPresentFamily())
      << "Chosen device doesn't have a present queue";

  std::array<vk::DeviceQueueCreateInfo, 2> queue_create_infos;

  vk::DeviceQueueCreateInfo &graphics_queue_create_info = queue_create_infos[0];
  std::array<float, 1> graphics_queue_priorities = {1.0f};
  graphics_queue_create_info.setQueueFamilyIndex(
      phys_device.GetGraphicsFamily().value());
  graphics_queue_create_info.setQueueCount(1);
  graphics_queue_create_info.setPQueuePriorities(
      graphics_queue_priorities.data());

  vk::DeviceQueueCreateInfo &present_queue_create_info = queue_create_infos[1];
  std::array<float, 1> present_queue_priorities = {1.0f};
  present_queue_create_info.setQueueFamilyIndex(
      phys_device.GetPresentFamily().value());
  present_queue_create_info.setQueueCount(1);
  present_queue_create_info.setPQueuePriorities(
      present_queue_priorities.data());

  vk::PhysicalDeviceFeatures device_features;

  vk::DeviceCreateInfo create_info;
  create_info.setPpEnabledExtensionNames(enabled_extensions.data());
  create_info.setEnabledExtensionCount(enabled_extensions.size());
  create_info.setPQueueCreateInfos(queue_create_infos.data());
  create_info.setQueueCreateInfoCount(queue_create_infos.size());
  create_info.setPEnabledFeatures(&device_features);

  return phys_device.GetHandle().createDevice(create_info);
}

vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR> &available_formats) {
  for (auto &format : available_formats) {
    if (format.format == vk::Format::eB8G8R8A8Unorm &&
        format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
      return format;
    }
  }

  return available_formats.front();
}

vk::PresentModeKHR ChooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR> &available_modes) {
  for (auto mode : available_modes) {
    if (mode == vk::PresentModeKHR::eMailbox) return mode;
  }

  return vk::PresentModeKHR::eFifo;
}

vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities,
                              VulkanSurface &surface) {
  if (capabilities.currentExtent.width == UINT32_MAX) {
    // Vulkan says we get to choose the resolution of the window so we will ask
    // the surface what size it wants to be
    vk::Extent2D extent;
    surface.GetNativeSurface()->GetVulkanSurfaceSize(extent.width,
                                                     extent.height);

    extent.width = std::clamp(extent.width, capabilities.minImageExtent.width,
                              capabilities.maxImageExtent.width);
    extent.height =
        std::clamp(extent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);
    return extent;
  }

  return capabilities.currentExtent;
}

vk::SwapchainKHR CreateSwapchain(vk::Device device,
                                 PhysicalDevice &physical_device,
                                 VulkanSurface &surface) {
  auto surface_format =
      ChooseSwapSurfaceFormat(physical_device.GetSurfaceFormats());
  auto present_mode = ChooseSwapPresentMode(physical_device.GetPresentModes());
  auto extent =
      ChooseSwapExtent(physical_device.GetSurfaceCapabilities(), surface);

  uint32_t image_count =
      physical_device.GetSurfaceCapabilities().minImageCount + 1;
  if (physical_device.GetSurfaceCapabilities().maxImageCount > 0 &&
      image_count > physical_device.GetSurfaceCapabilities().maxImageCount) {
    image_count = physical_device.GetSurfaceCapabilities().maxImageCount;
  }

  vk::SwapchainCreateInfoKHR create_info;
  create_info.setSurface(surface);
  create_info.setMinImageCount(image_count);
  create_info.setImageFormat(surface_format.format);
  create_info.setImageColorSpace(surface_format.colorSpace);
  create_info.setImageExtent(extent);
  create_info.setImageArrayLayers(1);
  create_info.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

  std::array<uint32_t, 2> queue_family_indices = {
      physical_device.GetGraphicsFamily().value(),
      physical_device.GetPresentFamily().value(),
  };

  if (physical_device.GetGraphicsFamily().value() ==
      physical_device.GetPresentFamily().value()) {
    create_info.setImageSharingMode(vk::SharingMode::eExclusive);
  } else {
    create_info.imageSharingMode = vk::SharingMode::eConcurrent;
    create_info.queueFamilyIndexCount = queue_family_indices.size();
    create_info.pQueueFamilyIndices = queue_family_indices.data();
  }

  create_info.preTransform =
      physical_device.GetSurfaceCapabilities().currentTransform;
  create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

  create_info.presentMode = present_mode;
  create_info.clipped = true;
  create_info.oldSwapchain = nullptr;

  return device.createSwapchainKHR(create_info);
}

std::vector<vk::ImageView> CreateImageViews(
    vk::Device device, const std::vector<vk::Image> &images,
    vk::Format format) {
  std::vector<vk::ImageView> image_views{images.size()};
  for (size_t i = 0; i < images.size(); i++) {
    vk::ImageViewCreateInfo create_info;
    create_info.image = images[i];
    create_info.viewType = vk::ImageViewType::e2D;
    create_info.format = format;

    create_info.components.r = vk::ComponentSwizzle::eIdentity;
    create_info.components.g = vk::ComponentSwizzle::eIdentity;
    create_info.components.b = vk::ComponentSwizzle::eIdentity;
    create_info.components.a = vk::ComponentSwizzle::eIdentity;

    create_info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    image_views[i] = device.createImageView(create_info);
  }

  return image_views;
}

}  // namespace

VkRenderer::VkRenderer(VkNativeDisplay &native_display,
                       VulkanNativeSurface &native_surface)
    : instance_(CreateInstance(native_display)),
      surface_(native_surface.CreateVulkanSurface(instance_.get())) {
  auto phys_devices = instance_->enumeratePhysicalDevices();
  LOG(INFO) << "Got " << phys_devices.size() << " devices";
  for (auto &dev : phys_devices) {
    auto props = dev.getProperties();
    LOG(INFO) << "  " << props.deviceName;
  }

  PhysicalDevice chosen_device{phys_devices.front(), surface_};

  device_ = CreateDevice(chosen_device);
  swapchain_ = CreateSwapchain(device_, chosen_device, surface_);

  swapchain_images_ = device_.getSwapchainImagesKHR(swapchain_);
  swapchain_image_views_ = CreateImageViews(
      device_, swapchain_images_,
      ChooseSwapSurfaceFormat(chosen_device.GetSurfaceFormats()).format);

  graphics_queue_ =
      device_.getQueue(chosen_device.GetGraphicsFamily().value(), 0);
  present_queue_ =
      device_.getQueue(chosen_device.GetPresentFamily().value(), 0);
}

VkRenderer::~VkRenderer() {
  for (auto image_view : swapchain_image_views_)
    device_.destroyImageView(image_view);

  device_.destroySwapchainKHR(swapchain_);
  device_.destroy();
}

}  // namespace engine::graphics::vulkan
