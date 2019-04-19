#include <iostream>

#include "VulkanInfo.h"

// Google logging library
#include "glog/logging.h"

int vulkanCall(VkResult result, const char* file, int line) {
  if (result != VK_SUCCESS) {
    std::cerr << file << ":" << line
	      << " Vulkan returned error: "
	      << result
	      << "."
	      << std::endl;
  }
}

void VulkanInstanceInfo::GetLayers() {

  
  // Get number of instance layers
  uint32_t instanceLayerCount;
  vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
  
  std::vector<VkLayerProperties> layerProperties;
  layerProperties.resize(instanceLayerCount);
  vkEnumerateInstanceLayerProperties(&instanceLayerCount,
				     layerProperties.data());

  Layers.resize(instanceLayerCount);

  for (int i = 0; i < instanceLayerCount; i++) {
    
    const VkLayerProperties &prop = layerProperties[i];
    Layers[i].LayerProperties = prop;
    layerNames.push_back(prop.layerName);

    uint32_t layerExtensionCount;
    vkEnumerateInstanceExtensionProperties(prop.layerName,
					   &layerExtensionCount,
					   nullptr);
    
    Layers[i].ExtensionProperties.resize(layerExtensionCount);
    vkEnumerateInstanceExtensionProperties(prop.layerName,
					   &layerExtensionCount,
					   Layers[i].ExtensionProperties.data());
    
    for (int j = 0; j < layerExtensionCount; j++) {
      const VkExtensionProperties &ext_prop = Layers[i].ExtensionProperties[j];
      extensionNames.push_back(ext_prop.extensionName);
    }
  }
}

void VulkanInstance::CreateInstance() {

  VkResult result = VK_SUCCESS;
  //==========================================================================
  // Get Instance Layer properties
  InstanceInfo.GetLayers();

  VkApplicationInfo applicationInfo = {
    VK_STRUCTURE_TYPE_APPLICATION_INFO,
    nullptr,
    "Vulkan Skeleton",
    VK_MAKE_VERSION(0, 0, 1),
    "My engine",
    VK_MAKE_VERSION(0, 0, 1),
    VK_MAKE_VERSION(1, 1, 73)
  };

  VkInstanceCreateInfo instanceCreateInfo = {
    VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    nullptr,
    0,
    &applicationInfo,    
    0, //InstanceInfo.layerNames.size(),
    nullptr, //InstanceInfo.layerNames.data(),
    0, //InstanceInfo.extensionNames.size(),
    nullptr, //InstanceInfo.extensionNames.data()
  };

  vulkanCall(vkCreateInstance(&instanceCreateInfo, nullptr, &Instance),
	     __FILE__, __LINE__);
}

void VulkanInstance::GetPhysicalDevices() {
  
  // Get number of devices
  uint32_t physicalDeviceCount;
  vkEnumeratePhysicalDevices(Instance, &physicalDeviceCount, nullptr);

  std::vector<VkPhysicalDevice> physicalDevices;
  physicalDevices.resize(physicalDeviceCount);  
  vkEnumeratePhysicalDevices(Instance, &physicalDeviceCount,
  			     physicalDevices.data());

  PhysicalDevices.resize(physicalDeviceCount);

  for (int i = 0; i < physicalDeviceCount; i++) {

    const VkPhysicalDevice &physicalDevice = physicalDevices[i];
    PhysicalDevices[i].PhysicalDevice = physicalDevice;
    
    PhysicalDevices[i].ExtensionProperties.resize(InstanceInfo.Layers.size());    
    for (int j = 0; j < InstanceInfo.Layers.size(); j++) {
      uint32_t deviceExtensionCount;
      vkEnumerateDeviceExtensionProperties(physicalDevice,
  					   InstanceInfo.Layers[j].LayerProperties.layerName,
  					   &deviceExtensionCount,
  					   nullptr);
      
      PhysicalDevices[i].ExtensionProperties[j].resize(deviceExtensionCount);
      vkEnumerateDeviceExtensionProperties(physicalDevice,
  					   InstanceInfo.Layers[j].LayerProperties.layerName,
  					   &deviceExtensionCount,
  					   PhysicalDevices[i].ExtensionProperties[j].data());

    }
  }
}

void VulkanDeviceInfo::GetPhysicalDeviceInfo(VkPhysicalDevice physicalDevice) {
  
  uint32_t queueFamilyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
  					   &queueFamilyCount,
  					   nullptr);
  QueueFamilyProperties.resize(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
  					   &queueFamilyCount,
  					   QueueFamilyProperties.data());

  vkGetPhysicalDeviceMemoryProperties(physicalDevice,
  				      &PhysicalDeviceMemoryProperties);

  vkGetPhysicalDeviceProperties(physicalDevice,
  				&PhysicalDeviceProperties);

  
}
