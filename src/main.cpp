#define VK_USE_PLATFORM_XCB_KHR
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_xcb.h"

#include "VulkanUtils.h"
#include "XmlUtils.h"

#include <iostream>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

void get_properties() {

  VulkanInstanceLayerProperties vulkanInstanceLayerProperties;
  vulkanInstanceLayerProperties.Fetch();
  std::vector<const char*> instanceLayerNames;
  vulkanInstanceLayerProperties.GetLayerNames(instanceLayerNames);

  VulkanInstanceExtensionProperties vulkanInstanceExtensionProperties;
  vulkanInstanceExtensionProperties.Fetch(instanceLayerNames, true);
  std::vector<const char*> instanceExtensionNames;
  vulkanInstanceExtensionProperties.GetExtensionNames(instanceExtensionNames);

  // TODO: choose extensions here
  
  VulkanInstance vulkanInstance;  
  vulkanInstance.CreateInstance(instanceLayerNames, instanceExtensionNames);

  VulkanPhysicalDevices vulkanPhysicalDevices;
  vulkanPhysicalDevices.Fetch(vulkanInstance.Instance);

  VulkanDeviceExtensionProperties vulkanDeviceExtensionProperties;
  vulkanDeviceExtensionProperties.Fetch(vulkanPhysicalDevices.PhysicalDevices,
  					instanceLayerNames);
  // TODO use selected extensions
  std::vector<const char*> deviceExtensionNames;
  vulkanDeviceExtensionProperties.GetExtensionNames(0, // physicalDeviceId
						    deviceExtensionNames);  

  VulkanPhysicalDeviceProperties vulkanPhysicalDeviceProperties;
  // TODO: choose device
  vulkanPhysicalDeviceProperties.Fetch(vulkanPhysicalDevices.PhysicalDevices[0]);

  VulkanPhysicalDeviceMemoryProperties vulkanPhysicalDeviceMemoryProperties;
  // TODO: choose device
  vulkanPhysicalDeviceMemoryProperties.Fetch(vulkanPhysicalDevices.PhysicalDevices[0]);

  VulkanPhysicalDeviceQueueFamilyProperties vulkanPhysicalDeviceQueueFamilyProperties;
  // TODO: choose device
  vulkanPhysicalDeviceQueueFamilyProperties.Fetch(vulkanPhysicalDevices.PhysicalDevices[0]);

  // TODO: choose queue families here
  float queuePriority = 1.0f;
  std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos = {
    {
      VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
      nullptr,
      0,
      0,
      1,
      &queuePriority
    }
  };

  VulkanDevice vulkanDevice;
  vulkanDevice.CreateDevice(vulkanPhysicalDevices.PhysicalDevices[0],
			    deviceQueueCreateInfos,
			    deviceExtensionNames);

  // pt::ptree pt;
  // vulkanInstanceLayerProperties.Write(pt);
  // vulkanInstanceExtensionProperties.Write(pt);
  // XmlWrite(pt, std::string("config.xml"));
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


