#define VK_USE_PLATFORM_XCB_KHR
#include "vulkan/vulkan.h"

#include "VulkanUtils.h"
#include "XmlUtils.h"

#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;


void get_properties() {

  // VulkanInstanceLayerProperties vulkanInstanceLayerProperties;
  // vulkanInstanceLayerProperties.Fetch();
  // vulkanInstanceLayerProperties.GetLayerNames(instanceLayerNames);
  std::vector<const char*> instanceLayerNames = {
    // "VK_LAYER_LUNARG_api_dump",
    "VK_LAYER_LUNARG_assistant_layer",
    "VK_LAYER_LUNARG_core_validation",
    "VK_LAYER_LUNARG_monitor",
    "VK_LAYER_LUNARG_object_tracker",
    "VK_LAYER_LUNARG_parameter_validation",
    "VK_LAYER_LUNARG_screenshot",
    "VK_LAYER_LUNARG_standard_validation",
    "VK_LAYER_GOOGLE_threading",
    "VK_LAYER_GOOGLE_unique_objects",
    "VK_LAYER_KHRONOS_validation"    
  };

  // VulkanInstanceExtensionProperties vulkanInstanceExtensionProperties;
  // vulkanInstanceExtensionProperties.Fetch(instanceLayerNames, true);
  // vulkanInstanceExtensionProperties.GetExtensionNames(instanceExtensionNames);
  std::vector<const char*> instanceExtensionNames = {
    "VK_KHR_surface",
    "VK_KHR_xcb_surface"
  };
  
  VulkanInstance vulkanInstance;  
  vulkanInstance.CreateInstance(instanceLayerNames, instanceExtensionNames);

  VulkanPhysicalDevices vulkanPhysicalDevices;
  vulkanPhysicalDevices.Fetch(vulkanInstance.Instance);
  // TODO: choose device
  uint32_t physicalDeviceId = 0;
  VkPhysicalDevice &physicalDevice =
    vulkanPhysicalDevices.PhysicalDevices[physicalDeviceId];
  
  // TODO use selected extensions
  // VulkanDeviceExtensionProperties vulkanDeviceExtensionProperties;
  // vulkanDeviceExtensionProperties.Fetch(vulkanPhysicalDevices.PhysicalDevices,
  // 					instanceLayerNames);
  // vulkanDeviceExtensionProperties.GetExtensionNames(physicalDeviceId, 
  // 						    deviceExtensionNames);  
  std::vector<const char*> deviceExtensionNames = {};

  VulkanPhysicalDeviceProperties vulkanPhysicalDeviceProperties;
  vulkanPhysicalDeviceProperties.Fetch(physicalDevice);

  VulkanPhysicalDeviceMemoryProperties vulkanPhysicalDeviceMemoryProperties;
  vulkanPhysicalDeviceMemoryProperties.Fetch(physicalDevice);

  VulkanPhysicalDeviceQueueFamilyProperties vulkanPhysicalDeviceQueueFamilyProperties;
  vulkanPhysicalDeviceQueueFamilyProperties.Fetch(physicalDevice);

  VulkanDevice vulkanDevice;
  vulkanDevice.CreateDevice(physicalDevice,
			    deviceExtensionNames);

  VulkanSurface vulkanSurface;
  vulkanSurface.CreateSurface(vulkanInstance.Instance);
}

int main(int argc, char** argv) {

  try {
    po::options_description desc{"Options"};
    desc.add_options()
      ("help,h", "Help screen")
      ("get-properties", "Write Vulkan properties to file");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
    }
    if (vm.count("get-properties")) {
      get_properties();
    }
  }
  catch(const std::runtime_error &ex) {
    std::cerr << ex.what() << std::endl;
  }


  return 0;
}


