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

struct VulkanInstance {

  static VulkanInstance& GetInstance() {
    static VulkanInstance s_Instance;
    return s_Instance;
  }

  VkInstance Instance;
  /* ~VulkanInstance() { */
  /*   std::cout << ">>>destructor" << std::endl; */
  /*   vkDestroyInstance(Instance, NULL); */
  /* } */
  
  void CreateInstance(const std::vector<const char*>& layerNames,
		      const std::vector<const char*>& extensionsNames);

  VulkanInstance(VulkanInstance const&) = delete;
  void operator=(VulkanInstance const&) = delete;
  
private:
  VulkanInstance() {std::cout << ">>>constructor" << std::endl;}
};

struct VulkanPhysicalDevices {
  std::vector<VkPhysicalDevice> PhysicalDevices;
  
  void Fetch(VkInstance instance);
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
};


/* struct VulkanPhysicalDevices { */
/*   VkPhysicalDevice PhysicalDevice; */
/*   std::vector<std::vector<VkExtensionProperties>> ExtensionProperties; */
/* }; */

int vulkanCall(VkResult result, const char* file, int line);


/* struct VulkanDeviceInfo { */

/*   std::vector<VkQueueFamilyProperties> QueueFamilyProperties; */
/*   VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties; */
/*   VkPhysicalDeviceProperties PhysicalDeviceProperties; */

/*   void GetPhysicalDeviceInfo(VkPhysicalDevice physicalDevice); */
/* }; */


#endif // VULKAN_INFO_H_
