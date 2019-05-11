// std
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <cstring>

// project
#include "XmlUtils.h"
#include "VulkanUtils.h"

int vulkanCall(VkResult result, const char* file, int line)
{
  if (result != VK_SUCCESS) {
    std::cerr << file << ":" << line
	      << " Vulkan returned error: "
	      << result
	      << "."
	      << std::endl;
  }
}

void VulkanInstanceLayerProperties::Fetch()
{

  uint32_t propertyCount;
  vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
  LayerProperties.resize(propertyCount);
  vkEnumerateInstanceLayerProperties(&propertyCount,
				     LayerProperties.data());
}

void VulkanInstanceLayerProperties::Print()
{

  std::cout << "Instance layer properties:" << std::endl;
  for (int i = 0; i < LayerProperties.size(); i++) {
    std::cout << "\t[" << i << "]: "
	      << LayerProperties[i].layerName << std::endl;
  }
}

void VulkanInstanceLayerProperties::Write(pt::ptree& tree)
{

  tree.put("info.name", "Layer names");
  for (int i = 0; i < LayerProperties.size(); i++) {
    tree.add("info.layers.name", LayerProperties[i].layerName);
    tree.add("info.layers.index", i);
  }
}

void VulkanInstanceLayerProperties::GetLayerNames(std::vector<const char*>& layerNames)
{
  for (VkLayerProperties &prop : LayerProperties) {
    char *name = new char[VK_MAX_EXTENSION_NAME_SIZE];
    strncpy(name, prop.layerName, sizeof(prop.layerName));
    layerNames.push_back(name);
  }
}

void VulkanInstanceExtensionProperties::Fetch(const std::vector<const char*>& layerNames,
					      bool getDefaultExtensions)
{
  ExtensionProperties.resize(layerNames.size() + (getDefaultExtensions ? 1 : 0));
  
  for (int i = 0; i < layerNames.size(); i++) {
    uint32_t propertyCount;
    vkEnumerateInstanceExtensionProperties(layerNames[i], &propertyCount, nullptr);
    ExtensionProperties[i].resize(propertyCount);
    vkEnumerateInstanceExtensionProperties(layerNames[i],
					   &propertyCount,
					   ExtensionProperties[i].data());
  }
  if (getDefaultExtensions) {
    int i = ExtensionProperties.size() - 1;
    uint32_t propertyCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);
    ExtensionProperties[i].resize(propertyCount);
    vkEnumerateInstanceExtensionProperties(nullptr,
					   &propertyCount,
					   ExtensionProperties[i].data());    
  }
}

void VulkanInstanceExtensionProperties::Print()
{
  
  std::cout << "Instance extension properties:" << std::endl;
  for (int i = 0; i < ExtensionProperties.size(); i++) {
    for (int j = 0; j < ExtensionProperties[i].size(); j++) {
    
      std::cout << "\t[" << i << "][" << j << "]: "
		<< ExtensionProperties[i][j].extensionName << std::endl;
    }
  }
}

void VulkanInstanceExtensionProperties::Write(pt::ptree& tree)
{

  tree.put("info.name", "Extension names");
  for (int i = 0; i < ExtensionProperties.size(); i++) {
    for (int j = 0; j < ExtensionProperties[i].size(); j++) {
      
      tree.add("info.extensions.name", ExtensionProperties[i][j].extensionName);
      tree.add("info.extensions.fromLayer", i);
    }
  }
}

void VulkanInstanceExtensionProperties::GetExtensionNames(std::vector<const char*>& extensionNames)
{
  for (auto &props : ExtensionProperties) {
    for (VkExtensionProperties &prop : props) {
      char *name = new char[VK_MAX_EXTENSION_NAME_SIZE];
      strncpy(name, prop.extensionName, sizeof(prop.extensionName));
      extensionNames.push_back(name);
    }
  }
}

int VulkanInstance::InstanceCounter = 0;

void VulkanInstance::CreateInstance(const std::vector<const char*>& layerNames,
				    const std::vector<const char*>& extensionNames)
{

  VkApplicationInfo applicationInfo = {
    VK_STRUCTURE_TYPE_APPLICATION_INFO, // sType
    nullptr,                            // pNext
    "Vulkan-Tools",                     // pApplicationName
    VK_MAKE_VERSION(0, 0, 1),           // applicationVersion
    "MyEngine",                         // pEngineName
    VK_MAKE_VERSION(0, 0, 1),           // engineVersion
    VK_MAKE_VERSION(1, 1, 73)           // apiVersion
  };

  VkInstanceCreateInfo instanceCreateInfo = {
    VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,       // sType
    nullptr,                                      // pNext
    0,                                            // flags
    &applicationInfo,                             // pApplicationInfo
    static_cast<uint32_t>(layerNames.size()),     // enabledLayerCount
    layerNames.data(),                            // ppEnabledLayerNames
    static_cast<uint32_t>(extensionNames.size()), // enabledExtensionCount
    extensionNames.data()                         // ppEnabledExtensionNames
  };

  vulkanCall(vkCreateInstance(&instanceCreateInfo, nullptr, &Instance),
	     __FILE__, __LINE__);
}

void VulkanPhysicalDevices::Fetch(const VulkanInstance& instance)
{

  uint32_t physicalDeviceCount;
  vkEnumeratePhysicalDevices(instance.Instance,
			     &physicalDeviceCount,
			     nullptr);
  PhysicalDevices.resize(physicalDeviceCount);
  vkEnumeratePhysicalDevices(instance.Instance,
			     &physicalDeviceCount,
			     PhysicalDevices.data());
}

void VulkanPhysicalDevices::Print()
{

  std::cout << "Number of physical devices: " << PhysicalDevices.size() << std::endl;
}

void VulkanDeviceExtensionProperties::Fetch(const std::vector<VkPhysicalDevice>& physicalDevices,
					    const std::vector<const char*>& layerNames)
{

  ExtensionProperties.resize(physicalDevices.size());
  
  for (int i = 0; i < physicalDevices.size(); i++) {

    ExtensionProperties[i].resize(layerNames.size());
    
    for (int j = 0; j < layerNames.size(); j++) {
      
      uint32_t propertyCount;
      vkEnumerateDeviceExtensionProperties(physicalDevices[i],
					   layerNames[j],
					   &propertyCount,
					   nullptr);
      ExtensionProperties[i][j].resize(propertyCount);
      vkEnumerateDeviceExtensionProperties(physicalDevices[i],
					   layerNames[j],
					   &propertyCount,
					   ExtensionProperties[i][j].data());
    }
  }
}

void VulkanDeviceExtensionProperties::Print()
{
  
  std::cout << "Device extension properties [device][layer]:" << std::endl;
  for (int i = 0; i < ExtensionProperties.size(); i++) {
    for (int j = 0; j < ExtensionProperties[i].size(); j++) {
      for (int k = 0; k < ExtensionProperties[i][j].size(); k++) {
	std::cout << "\t[" << i << "][" << j << "]: "
		  << ExtensionProperties[i][j][k].extensionName << std::endl;
      }
    }
  }
}

void VulkanDeviceProperties::Fetch(VkPhysicalDevice physicalDevice)
{
  
  // Physical device properties
  vkGetPhysicalDeviceProperties(physicalDevice, &Properties);

  // Memory properties
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &MemoryProperties);

  uint32_t propertyCount;

  // Device layer properties
  vkEnumerateDeviceLayerProperties(physicalDevice, &propertyCount, nullptr);
  LayerProperties.resize(propertyCount);
  vkEnumerateDeviceLayerProperties(physicalDevice, &propertyCount, LayerProperties.data());

  // Queue family properties
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertyCount, nullptr);
  QueueFamilyProperties.resize(propertyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertyCount, QueueFamilyProperties.data());
}

void VulkanDeviceProperties::Print()
{

  std::cout << "Device name:" << std::endl;
  std::cout << "\t" << Properties.deviceName << std::endl;
  std::cout << "API version:" << std::endl;
  std::cout << "\t" << Properties.apiVersion << std::endl;
  std::cout << "Driver version:" << std::endl;
  std::cout << "\t" << Properties.driverVersion << std::endl;
  std::cout << "Vendor ID:" << std::endl;
  std::cout << "\t" << Properties.vendorID << std::endl;
  std::cout << "Device ID:" << std::endl;
  std::cout << "\t" << Properties.deviceID << std::endl;
  
  std::cout << "Device layer properties:" << std::endl;
  for (int i = 0; i < LayerProperties.size(); i++) {
    std::cout << "\t[" << i << "]: " << LayerProperties[i].layerName << std::endl;
  }

  std::cout << "Queue family properties:" << std::endl;
  for (int i = 0; i < QueueFamilyProperties.size(); i++) {
    std::cout << "\t[" << i << "] queueFlags: " << QueueFamilyProperties[i].queueFlags << std::endl;
    std::cout << "\t[" << i << "] queueCount: " << QueueFamilyProperties[i].queueCount << std::endl;    
  }
}

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
