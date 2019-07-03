// project
#include "XmlUtils.h"
#include "VulkanUtils.h"

// std
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <cstring>
#include <map>

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

//----------------------------------------------------------------------------
// VulkanInstanceLayerProperties
void VulkanInstanceLayerProperties::Fetch()
{
  uint32_t propertyCount;
  vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
  LayerProperties.resize(propertyCount);
  vkEnumerateInstanceLayerProperties(&propertyCount, LayerProperties.data());
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

//----------------------------------------------------------------------------
// VulkanInstanceExtensionProperties
void VulkanInstanceExtensionProperties::Fetch(const std::vector<const char*>& layerNames,
					      bool getDefaultExtensions)
{
  ExtensionProperties.resize(layerNames.size() + (getDefaultExtensions ? 1 : 0));
  
  for (int i = 0; i < layerNames.size(); i++) {
    uint32_t propertyCount;
    vkEnumerateInstanceExtensionProperties(layerNames[i],
					   &propertyCount,
					   nullptr);
    ExtensionProperties[i].resize(propertyCount);
    vkEnumerateInstanceExtensionProperties(layerNames[i],
					   &propertyCount,
					   ExtensionProperties[i].data());
  }
  if (getDefaultExtensions) {
    int i = ExtensionProperties.size() - 1;
    uint32_t propertyCount;
    vkEnumerateInstanceExtensionProperties(nullptr,
					   &propertyCount,
					   nullptr);
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

//----------------------------------------------------------------------------
// VulkanInstance
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

//----------------------------------------------------------------------------
// VulkanPhysicalDevices
void VulkanPhysicalDevices::Fetch(const VkInstance instance)
{
  uint32_t physicalDeviceCount;
  vkEnumeratePhysicalDevices(instance,
			     &physicalDeviceCount,
			     nullptr);
  PhysicalDevices.resize(physicalDeviceCount);
  vkEnumeratePhysicalDevices(instance,
			     &physicalDeviceCount,
			     PhysicalDevices.data());
}

void VulkanPhysicalDevices::Print()
{
  std::cout << "Number of physical devices: " << PhysicalDevices.size() << std::endl;
}

//----------------------------------------------------------------------------
// VulkanDeviceExtensionProperties
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

void VulkanDeviceExtensionProperties::GetExtensionNames(uint32_t physicalDeviceId,
							std::vector<const char*>& extensionNames)
{
  for (auto &layerProps : ExtensionProperties[physicalDeviceId]) {    
    for (auto &prop : layerProps) {
      
      char *name = new char[VK_MAX_EXTENSION_NAME_SIZE];
      strncpy(name, prop.extensionName, sizeof(prop.extensionName));
      extensionNames.push_back(name);
      std::cout << "||||||" << name << std::endl;
    }
  }
}

//----------------------------------------------------------------------------
// VulkanPhysicalDeviceProperties
void VulkanPhysicalDeviceProperties::Fetch(VkPhysicalDevice physicalDevice)
{
  vkGetPhysicalDeviceProperties(physicalDevice, &Properties);
}

void VulkanPhysicalDeviceProperties::Print()
{
  std::cout << "Device name:" << std::endl
	    << "\t" << Properties.deviceName << std::endl
	    << "API version:" << std::endl
	    << "\t" << Properties.apiVersion << std::endl
	    << "Driver version:" << std::endl
	    << "\t" << Properties.driverVersion << std::endl
	    << "Vendor ID:" << std::endl
	    << "\t" << Properties.vendorID << std::endl
	    << "Device ID:" << std::endl
	    << "\t" << Properties.deviceID << std::endl;
  
}

//----------------------------------------------------------------------------
// VulkanPhysicalDeviceMemoryProperties
void VulkanPhysicalDeviceMemoryProperties::Fetch(VkPhysicalDevice physicalDevice)
{
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &Properties);
}


void VulkanPhysicalDeviceMemoryProperties::Print()
{
  std::map<int, std::string> memoryPropertyFlagNames = {    
    {0x00000001, "VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT"},
    {0x00000002, "VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT"},
    {0x0000004, "VK_MEMORY_PROPERTY_HOST_COHERENT_BIT"},
    {0x00000008, "VK_MEMORY_PROPERTY_HOST_CACHED_BIT"},
    {0x00000010, "VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT"},
    {0x00000020, "VK_MEMORY_PROPERTY_PROTECTED_BIT"},
    {0x7FFFFFFF, "VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM"}
  };
  std::map<int, std::string> emoryHeapFlagNames = {
    {0x00000001, "VK_MEMORY_HEAP_DEVICE_LOCAL_BIT"},
    {0x00000002, "VK_MEMORY_HEAP_MULTI_INSTANCE_BIT"},
    {0x7FFFFFFF, "VK_MEMORY_HEAP_FLAG_BITS_MAX_ENUM"} 
  };

  std::cout << "======================================================================" << std::endl;
  std::cout << "VkPhysicalDeviceMemoryProperties" << std::endl;
  std::cout << "\tmemoryTypes (count: " << Properties.memoryTypeCount << ")" << std::endl;
  
  for (int i = 0; i < Properties.memoryTypeCount; i++) {
    
    std::cout << "\t\theapIndex: " << Properties.memoryTypes[i].heapIndex << " "
	      << "propertyFlags: " << Properties.memoryTypes[i].propertyFlags << std::endl;
    for (int j = 0; j < 32; j++) {
      if ((1 << j) & Properties.memoryTypes[i].propertyFlags) {
	std::cout << "\t\t\t" << memoryPropertyFlagNames[1<<j] << std::endl;
      }
    }
  }
  std::cout << "\tmemoryHeaps (count: " << Properties.memoryHeapCount << ")" << std::endl;
  for (int i = 0; i < Properties.memoryHeapCount; i++) {
    
    std::cout << "\t\theap size: " << Properties.memoryHeaps[i].size << " "
	      << "flags: " << Properties.memoryHeaps[i].flags << std::endl;
    for (int j = 0; j < 32; j++) {
      if ((1 << j) & Properties.memoryHeaps[i].flags) {
	std::cout << "\t\t\t" << memoryPropertyFlagNames[1<<j] << std::endl;
      }
    }
  }
  std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
}

//----------------------------------------------------------------------------
// VulkanPhysicalDeviceQueueFamilyProperties
void VulkanPhysicalDeviceQueueFamilyProperties::Fetch(VkPhysicalDevice physicalDevice)
{
  uint32_t propertyCount;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertyCount, NULL);
  Properties.resize(propertyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propertyCount, Properties.data());
}

void VulkanPhysicalDeviceQueueFamilyProperties::Print()
{
  std::map<int, std::string> queueFlagNames = {
    {0x00000001, "VK_QUEUE_GRAPHICS_BIT"},
    {0x00000002, "VK_QUEUE_COMPUTE_BIT"},
    {0x00000004, "VK_QUEUE_TRANSFER_BIT"},
    {0x00000008, "VK_QUEUE_SPARSE_BINDING_BIT"},
    {0x00000010, "VK_QUEUE_PROTECTED_BIT"},
    {0x7FFFFFFF, "VK_QUEUE_FLAG_BITS_MAX_ENUM"}
  };

  std::cout << "======================================================================" << std::endl;
  std::cout << "VulkanPhysicalDeviceQueueFamilyProperties [" << Properties.size() << "]" << std::endl;

  for (auto &props : Properties) {
    std::cout << "\tqueueCount: " << props.queueCount << " "
	      << "queueFlags: " << props.queueFlags << std::endl;
    for (int j = 0; j < 32; j++) {
      if ((1 << j) & props.queueFlags) {
	std::cout << "\t\t" << queueFlagNames[1<<j] << std::endl;
      }
    }
  }
  std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
}

//----------------------------------------------------------------------------
// VulkanDevice
void VulkanDevice::CreateDevice(VkPhysicalDevice physicalDevice,
				const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
				const std::vector<const char*>& extensionNames)
{
  VkDeviceCreateInfo deviceCreateInfo = {
    VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,           // sType
    nullptr,                                        // pNext
    0,                                              // flags
    static_cast<uint32_t>(queueCreateInfos.size()), // queueCreateInfoCount
    queueCreateInfos.data(),                        // pQueueCreateInfos
    0,                                              // enabledLayerCount (DEPRECATED)
    nullptr,                                        // ppEnabledLayerNames (DEPRECATED)
    static_cast<uint32_t>(extensionNames.size()),   // enabledExtensionCount
    extensionNames.data(),                          // ppEnabledExtensionNames
    nullptr                                         // pEnabledFeatures
  };

  vulkanCall(vkCreateDevice(physicalDevice,     // VkPhysicalDevice             physicalDevice
			    &deviceCreateInfo,  // const VkDeviceCreateInfo*    pCreateInfo
			    nullptr,            // const VkAllocationCallbacks* pAllocator
			    &Device),           // VkDevice*                    pDevice
	     __FILE__, __LINE__);
}
