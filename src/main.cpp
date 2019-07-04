#define VK_USE_PLATFORM_XCB_KHR
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_xcb.h"

#include "VulkanUtils.h"
#include "XmlUtils.h"

#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

// xcb
#include <xcb/xcb.h>
#include <xcb/xproto.h>

// loosely based on https://en.wikipedia.org/wiki/XCB
// and https://www.codeproject.com/Articles/1089819/An-Introduction-to-XCB-Programming
void createWindow(xcb_window_t *window, xcb_connection_t **connection) {

  *connection = xcb_connect(NULL, NULL);
  if (xcb_connection_has_error(*connection)) {
    std::cerr << "XCB connection failed. Exiting";
  }

  /* Obtain setup info and access the screen */
  const xcb_setup_t *setup = xcb_get_setup(*connection);
  xcb_screen_t *screen = xcb_setup_roots_iterator(setup).data;;

  /* Create window */
  *window = xcb_generate_id(*connection);
  uint32_t prop_name = XCB_CW_BACK_PIXEL;
  uint32_t prop_value = screen->white_pixel;

  xcb_void_cookie_t window_cookie = xcb_create_window(*connection, screen->root_depth,
						      *window, screen->root, 0, 0, 100, 100, 1,
						      XCB_WINDOW_CLASS_INPUT_OUTPUT,
						      screen->root_visual, prop_name, &prop_value);

}

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
  uint32_t physicalDeviceId = 0;
  vulkanDeviceExtensionProperties.GetExtensionNames(physicalDeviceId, 
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
      VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, // sType
      nullptr,                                    // pNext
      0,                                          // flags
      0,                                          // qeueueFamilyIndex
      1,                                          // queueCount
      &queuePriority                              // pQueuePriorities
    }
  };

  VulkanDevice vulkanDevice;
  vulkanDevice.CreateDevice(vulkanPhysicalDevices.PhysicalDevices[0],
			    deviceQueueCreateInfos,
			    deviceExtensionNames);

  VkQueue vulkanQueue;
  vkGetDeviceQueue(vulkanDevice.Device,
		   0,
		   0,
		   &vulkanQueue);

  xcb_window_t xcb_window;
  xcb_connection_t *xcb_connection;
  createWindow(&xcb_window, &xcb_connection);

  VkXcbSurfaceCreateInfoKHR xcbSurfaceCreateInfoKHR = {
    VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR, // sType;
    nullptr,                                       // pNext;
    0,                                             // flags;
    xcb_connection,                                // connection;
    xcb_window                                     // window;
  };

  VkSurfaceKHR vulkanSurface;
  vkCreateXcbSurfaceKHR(vulkanInstance.Instance,
			&xcbSurfaceCreateInfoKHR,
			nullptr,
			&vulkanSurface);
  
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


