#ifndef VULKAN_INFO_H_
#define VULKAN_INFO_H_

// Vulkan
#include "vulkan/vulkan_core.h"
#include "vulkan/vulkan.h"

// std
#include <vector>

int vulkanCall(VkResult result, const char* file, int line);

struct VulkanLayer {
  VkLayerProperties LayerProperties;
  std::vector<VkExtensionProperties> ExtensionProperties;
};

struct VulkanPhysicalDevice {
  VkPhysicalDevice PhysicalDevice;
  std::vector<std::vector<VkExtensionProperties>> ExtensionProperties;
};

struct VulkanInstanceInfo {

  std::vector<VulkanLayer> Layers;
  std::vector<const char*> layerNames;
  std::vector<const char*> extensionNames;

  void GetLayers();
};

struct VulkanInstance {

  VulkanInstanceInfo InstanceInfo;
  VkInstance Instance;
  std::vector<VulkanPhysicalDevice> PhysicalDevices;

  ~VulkanInstance() {
    vkDestroyInstance(Instance, NULL);
  }
  
  void CreateInstance();
  void GetPhysicalDevices();
};

struct VulkanDeviceInfo {

  std::vector<VkQueueFamilyProperties> QueueFamilyProperties;
  VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
  VkPhysicalDeviceProperties PhysicalDeviceProperties;

  void GetPhysicalDeviceInfo(VkPhysicalDevice physicalDevice);
};


#endif // VULKAN_INFO_H_
