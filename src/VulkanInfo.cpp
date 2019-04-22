#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <cstring>

#include "VulkanInfo.h"

int vulkanCall(VkResult result, const char* file, int line) {
  if (result != VK_SUCCESS) {
    std::cerr << file << ":" << line
	      << " Vulkan returned error: "
	      << result
	      << "."
	      << std::endl;
  }
}

VkApplicationInfo CreateVulkanApplicationInfo(const char* appName) {
  VkApplicationInfo applicationInfo = {
      VK_STRUCTURE_TYPE_APPLICATION_INFO,
      nullptr,
      appName,
      VK_MAKE_VERSION(0, 0, 1),
      "MyEngine",
      VK_MAKE_VERSION(0, 0, 1),
      VK_MAKE_VERSION(1, 1, 73)
    };
  return applicationInfo;
}

void VulkanLayerProperties::Fetch() {

  uint32_t propertyCount;
  vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
  LayerProperties.resize(propertyCount);
  vkEnumerateInstanceLayerProperties(&propertyCount,
				     LayerProperties.data());
}

void VulkanLayerProperties::Print() {

  std::cout << "VkLayerProperties:" << std::endl;
  for (int i = 0; i < LayerProperties.size(); i++) {
    std::cout << "\t[" << i << "]: "
	      << LayerProperties[i].layerName << std::endl;
  }
}

void VulkanLayerProperties::GetLayerNames(std::vector<const char*>& layerNames) {
  std::vector<int> selectedLayers(LayerProperties.size());
  std::iota(selectedLayers.begin(), selectedLayers.end(), 0);
  GetLayerNames(layerNames, selectedLayers);
}

void VulkanLayerProperties::GetLayerNames(std::vector<const char*>& layerNames,
					  const std::vector<int>& selectedLayers) {
  for (int i : selectedLayers) {
    VkLayerProperties &prop = LayerProperties[i];
    char *name = new char[VK_MAX_EXTENSION_NAME_SIZE];
    strncpy(name, prop.layerName, sizeof(prop.layerName));
    layerNames.push_back(name);
  }
}

void VulkanExtensionProperties::Fetch(const std::vector<const char*>& layerNames) {
  // to get extension enabled by default, as layerName pass nullptr to
  // vkEnumerateInstanceExtensionProperties
  ExtensionProperties.resize(layerNames.size()+1);
  
  for (int i = 0; i <= layerNames.size(); i++) {
    const char* layerName = i < layerNames.size() ? layerNames[i] : nullptr;
    uint32_t propertyCount;
    vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, nullptr);
    ExtensionProperties[i].resize(propertyCount);
    vkEnumerateInstanceExtensionProperties(layerName,
					   &propertyCount,
					   ExtensionProperties[i].data());
  }
}

void VulkanExtensionProperties::Print() {
  
  std::cout << "VkExtensionProperties:" << std::endl;
  for (int i = 0; i < ExtensionProperties.size(); i++) {
    for (int j = 0; j < ExtensionProperties[i].size(); j++) {
    
      std::cout << "\t[" << i << "][" << j << "]: "
	      << ExtensionProperties[i][j].extensionName << std::endl;
    }
  }
}

void VulkanExtensionProperties::
GetExtensionNames(std::vector<const char*>& extensionNames) {
  for (auto &props : ExtensionProperties) {
    for (VkExtensionProperties &prop : props) {
      char *name = new char[VK_MAX_EXTENSION_NAME_SIZE];
      strncpy(name, prop.extensionName, sizeof(prop.extensionName));
      extensionNames.push_back(name);
    }
  }
}

// void VulkanLayer::FetchLayerExtensions() {
  
//   uint32_t extensionCount;
//   vkEnumerateInstanceExtensionProperties(LayerProperties.layerName,
// 					 &extensionCount,
// 					 nullptr);
//   ExtensionProperties.resize(extensionCount);
//   vkEnumerateInstanceExtensionProperties(LayerProperties.layerName,
// 					 &extensionCount,
// 					 ExtensionProperties.data());
// }

// void VulkanInstance::CreateInstance() {

//   VkInstanceCreateInfo instanceCreateInfo = {
//     VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
//     nullptr,
//     0,
//     nullptr, //&applicationInfo,    
//     0, //static_cast<unsigned int>(InstanceInfo.layerNames.size()),
//     nullptr, //InstanceInfo.layerNames.data(),
//     0, //static_cast<unsigned int>(InstanceInfo.extensionNames.size()),
//     nullptr, //InstanceInfo.extensionNames.data()
//   };

//   vulkanCall(vkCreateInstance(&instanceCreateInfo, nullptr, &Instance),
// 	     __FILE__, __LINE__);
// }

// void VulkanInstance::GetPhysicalDevices() {
  
//   // Get number of devices
//   uint32_t physicalDeviceCount;
//   vkEnumeratePhysicalDevices(Instance, &physicalDeviceCount, nullptr);

//   std::vector<VkPhysicalDevice> physicalDevices;
//   physicalDevices.resize(physicalDeviceCount);  
//   vkEnumeratePhysicalDevices(Instance, &physicalDeviceCount,
//   			     physicalDevices.data());

//   PhysicalDevices.resize(physicalDeviceCount);

//   for (int i = 0; i < physicalDeviceCount; i++) {

//     const VkPhysicalDevice &physicalDevice = physicalDevices[i];
//     PhysicalDevices[i].PhysicalDevice = physicalDevice;
    
//     PhysicalDevices[i].ExtensionProperties.resize(InstanceInfo.Layers.size());    
//     for (int j = 0; j < InstanceInfo.Layers.size(); j++) {
//       uint32_t deviceExtensionCount;
//       vkEnumerateDeviceExtensionProperties(physicalDevice,
//   					   InstanceInfo.Layers[j].LayerProperties.layerName,
//   					   &deviceExtensionCount,
//   					   nullptr);
      
//       PhysicalDevices[i].ExtensionProperties[j].resize(deviceExtensionCount);
//       vkEnumerateDeviceExtensionProperties(physicalDevice,
//   					   InstanceInfo.Layers[j].LayerProperties.layerName,
//   					   &deviceExtensionCount,
//   					   PhysicalDevices[i].ExtensionProperties[j].data());

//     }
//   }
// }

// void VulkanDeviceInfo::GetPhysicalDeviceInfo(VkPhysicalDevice physicalDevice) {
  
//   uint32_t queueFamilyCount;
//   vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
//   					   &queueFamilyCount,
//   					   nullptr);
//   QueueFamilyProperties.resize(queueFamilyCount);
//   vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
//   					   &queueFamilyCount,
//   					   QueueFamilyProperties.data());

//   vkGetPhysicalDeviceMemoryProperties(physicalDevice,
//   				      &PhysicalDeviceMemoryProperties);

//   vkGetPhysicalDeviceProperties(physicalDevice,
//   				&PhysicalDeviceProperties);

  
// }
