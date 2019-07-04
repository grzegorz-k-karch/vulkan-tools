#ifndef VULKAN_INFO_H_
#define VULKAN_INFO_H_

// vulkan
#include "vulkan/vulkan_core.h"
#include "vulkan/vulkan.h"

// std
#include <vector>
#include <iostream>

// project
#include "XmlUtils.h"


struct VulkanInstanceLayerProperties {

  std::vector<VkLayerProperties> LayerProperties;
  
  void Fetch();
  void Print();
  void Write(pt::ptree& pt);
  void GetLayerNames(std::vector<const char*>& layerNames);
};


struct VulkanInstanceExtensionProperties {

  std::vector<std::vector<VkExtensionProperties>> ExtensionProperties;
  
  void Fetch(const std::vector<const char*>& layerNames,
	     bool getDefaultExtensions=true);
  void Print();
  void Write(pt::ptree& pt);
  void GetExtensionNames(std::vector<const char*>& extensionNames);
};


struct VulkanInstance {

  VkInstance Instance;
  static int InstanceCounter;
  
  VulkanInstance() {
    InstanceCounter++;
    if (InstanceCounter > 1) {
      std::cerr << "[VulkanInstance][Warning] Created more than 1 VkInstance which\
 is not good in most cases." << std::endl;
    }
  }
  ~VulkanInstance() {
    InstanceCounter--;
    vkDestroyInstance(Instance, NULL);
  }
  
  void CreateInstance(const std::vector<const char*>& layerNames,
		      const std::vector<const char*>& extensionsNames);
  
  VulkanInstance(VulkanInstance const&) = delete;
  void operator=(VulkanInstance const&) = delete;
};


struct VulkanPhysicalDevices {

  std::vector<VkPhysicalDevice> PhysicalDevices;
  
  void Fetch(const VkInstance instance);
  void Print();
  void Write(pt::ptree& pt); // not implemented yet
};


struct VulkanDeviceExtensionProperties {

  // [num devices][num layers][num extensions per device, per layer]
  std::vector<std::vector<std::vector<VkExtensionProperties>>> ExtensionProperties;
  
  void Fetch(const std::vector<VkPhysicalDevice>& physicalDevices,
	     const std::vector<const char*>& layerNames);
  void Print();
  void Write(pt::ptree& pt); // not implemented yet
  void GetExtensionNames(uint32_t physicalDeviceId,
			 std::vector<const char*>& extensionNames);
};


struct VulkanPhysicalDeviceProperties {

  VkPhysicalDeviceProperties Properties;

  void Fetch(VkPhysicalDevice physicalDevice);
  void Print();
};

struct VulkanPhysicalDeviceMemoryProperties {

  VkPhysicalDeviceMemoryProperties Properties;
  
  void Fetch(VkPhysicalDevice physicalDevice);
  void Print();
};

struct VulkanPhysicalDeviceQueueFamilyProperties {

  std::vector<VkQueueFamilyProperties> Properties;
  
  void Fetch(VkPhysicalDevice physicalDevice);
  void Print();
};

struct VulkanDevice {

  VkDevice Device;
  bool DeviceCreated;
  
  VulkanDevice() : DeviceCreated(false) {}
  ~VulkanDevice() {
    if (DeviceCreated) {
      vkDestroyDevice(Device, nullptr);
    }
  }
  void CreateDevice(VkPhysicalDevice physicalDevice,
		    const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
		    const std::vector<const char*>& extensionNames);

};

VkResult vulkanCall(VkResult result, const char* file, int line);


#endif // VULKAN_INFO_H_
