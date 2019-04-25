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


int vulkanCall(VkResult result, const char* file, int line);

VkApplicationInfo CreateVulkanApplicationInfo(const char* appName=nullptr);

struct VulkanLayerProperties {
  std::vector<VkLayerProperties> LayerProperties;
  
  void Fetch();
  void Print();
  void Write(pt::ptree& pt);
  void GetLayerNames(std::vector<const char*>& layerNames);
  void GetLayerNames(std::vector<const char*>& layerNames,
		     const std::vector<int>& selectedLayers);
};

struct VulkanExtensionProperties {
  std::vector<std::vector<VkExtensionProperties>> ExtensionProperties;
  
  void Fetch(const std::vector<const char*>& layerNames);
  void Print();
  void Write(pt::ptree& pt);
  void GetExtensionNames(std::vector<const char*>& extensionNames);
};



/* struct VulkanPhysicalDevice { */
/*   VkPhysicalDevice PhysicalDevice; */
/*   std::vector<std::vector<VkExtensionProperties>> ExtensionProperties; */
/* }; */

/* struct VulkanInstanceInfo { */

/*   std::vector<VulkanLayer> Layers; */
/*   std::vector<const char*> layerNames; */
/*   std::vector<const char*> extensionNames; */

/*   void GetLayers(); */
/* }; */

/* void GetLayerNames(const VkInstance instance, */
/* 		   std::vector<const char*>& layerNames) { */

/* } */

/* struct VulkanInstance { */

/*   VkInstance Instance; */
  
/*   std::vector<VulkanPhysicalDevice> PhysicalDevices; */

/*   ~VulkanInstance() { */
/*     vkDestroyInstance(Instance, NULL); */
/*   } */
  
/*   void CreateInstance(); */
/*   void GetPhysicalDevices(); */
/* }; */

/* struct VulkanDeviceInfo { */

/*   std::vector<VkQueueFamilyProperties> QueueFamilyProperties; */
/*   VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties; */
/*   VkPhysicalDeviceProperties PhysicalDeviceProperties; */

/*   void GetPhysicalDeviceInfo(VkPhysicalDevice physicalDevice); */
/* }; */


#endif // VULKAN_INFO_H_
