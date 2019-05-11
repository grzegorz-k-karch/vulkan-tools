#define VK_USE_PLATFORM_XCB_KHR
#include "vulkan/vulkan.h"
#include "vulkan/vulkan_xcb.h"

#include "VulkanUtils.h"
#include "XmlUtils.h"

// std
#include <iostream>

// boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

void get_properties() {

  VulkanInstanceLayerProperties vulkanInstanceLayerProperties;
  vulkanInstanceLayerProperties.Fetch();
  vulkanInstanceLayerProperties.Print();
  std::vector<const char*> instanceLayerNames;
  vulkanInstanceLayerProperties.GetLayerNames(instanceLayerNames);

  VulkanInstanceExtensionProperties vulkanInstanceExtensionProperties;
  vulkanInstanceExtensionProperties.Fetch(instanceLayerNames, true);
  vulkanInstanceExtensionProperties.Print();
  std::vector<const char*> instanceExtensionNames;
  vulkanInstanceExtensionProperties.GetExtensionNames(instanceExtensionNames);

  VulkanInstance vulkanInstance;  
  vulkanInstance.CreateInstance(instanceLayerNames, instanceExtensionNames);

  VulkanPhysicalDevices vulkanPhysicalDevices;
  vulkanPhysicalDevices.Fetch(vulkanInstance);
  vulkanPhysicalDevices.Print();

  VulkanDeviceExtensionProperties vulkanDeviceExtensionProperties;
  vulkanDeviceExtensionProperties.Fetch(vulkanPhysicalDevices.PhysicalDevices,
  					instanceLayerNames);
  vulkanDeviceExtensionProperties.Print();

  // device
  VulkanDeviceProperties vulkanDeviceProperties;
  vulkanDeviceProperties.Fetch(vulkanPhysicalDevices.PhysicalDevices[0]);
  vulkanDeviceProperties.Print();

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


  // VulkanInstance vulkanInstance;
  // vulkanInstance.CreateInstance();

  // vulkanInstance.GetPhysicalDevices();

  // VulkanDeviceInfo vulkanDeviceInfo;
  // VkResult result = VK_SUCCESS;


  // //==========================================================================
  // // 4. Create Device
  // // TODO: here just taking the first device
  // const int physicalDeviceId = 0;
  // VkPhysicalDevice &physicalDevice =
  //   vulkanInstance.PhysicalDevices[physicalDeviceId].PhysicalDevice;

  // vulkanDeviceInfo.GetPhysicalDeviceInfo(physicalDevice);

  // std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
  // std::vector<float> queuePriorities = {1.0f};

  // deviceQueueCreateInfos.push_back({
  //     VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
  // 	nullptr,
  // 	0,
  // 	0, //selected_graphics_queue_family_index,
  // 	queuePriorities.size(),
  // 	queuePriorities.data()
  // 	});

  // // if( selected_graphics_queue_family_index != selected_present_queue_family_index ) {
  // //   deviceQueueCreateInfos.push_back({
  // // 	VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
  // // 	  nullptr,
  // // 	  0,
  // // 	  0, //selected_present_queue_family_index,
  // // 	  queuePriorities.size(),
  // // 	  queuePriorities.data()
  // // 	  });
  // // }

  // std::vector<const char*> extensions = {
  //   VK_KHR_SWAPCHAIN_EXTENSION_NAME
  // };

  // // Create the logical device object from physical device
  // VkDeviceCreateInfo deviceCreateInfo = {
  //   VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
  //   nullptr,
  //   0,
  //   deviceQueueCreateInfos.size(),
  //   deviceQueueCreateInfos.data(),
  //   0,
  //   nullptr,
  //   extensions.size(),
  //   extensions.data(),
  //   nullptr
  // };

  // VkDevice device;
  // if (vkCreateDevice(physicalDevice,
  // 		     &deviceCreateInfo,
  // 		     nullptr,
  // 		     &device) != VK_SUCCESS) {
  //   LOG(FATAL) << "Could not create logical device. Exiting.";
  // }

  // //==========================================================================
  // // 5. Presentation Initialization
  // xcb_window_t window;
  // xcb_connection_t *connection;

  // CreateWindow(&window, &connection);

  // VkXcbSurfaceCreateInfoKHR surfaceCreateInfo =
  //   {VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
  //    nullptr,
  //    0,
  //    connection,
  //    window};

  // checkKHR();

  // VkSurfaceKHR surface;
  // result = vkCreateXcbSurfaceKHR(vulkanInstance.Instance, &surfaceCreateInfo, nullptr, &surface);
  // if (result != VK_SUCCESS) {
  //   LOG(FATAL) << "Could not create surface (Error: " << result << "). Exiting.";
  // }

  // // int graphicsQueueFamilyIndex;
  // // for (int queueIndex = 0; queueIndex < 1; queueIndex++) {
  // //   VkBool32 isPresentationSupported = false;
  // //   vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice,
  // //                      queueIndex,
  // //                      surface,
  // //                      &isPresentationSupported);
  // //   if (isPresentationSupported) {
  // //     graphicsQueueFamilyIndex = queueIndex;
  // //     break;
  // //   }
  // // }
  // // VkQueue queue;
  // // vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &queue);

  // // uint32_t formatCount;
  // // vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
  // //                        surface,
  // //                        &formatCount,
  // //                        nullptr);
  // // vulkanInfo.SurfaceFormats.resize(formatCount);
  // // vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
  // //                        surface,
  // //                        &formatCount,
  // //                        vulkanInfo.SurfaceFormats.data());

  // // // vkBeginCommandBuffer(cmd, &cmdBufferInfo);
  // // // vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &surfaceCapabilities);
  // // // vkGetPhysicalDeviceSurfacePresentModesKHR(device,  surface, &presentModeCount, nullptr);
  // // // VkPresentModeKHR presentModes[presentModeCount];
  // // // vkGetPhysicalDeviceSurfacePresentModesKHR(device,  surface, &presentModeCount, presentModes);

  // // // VkSwapchainCreateInfoKHR swapChainInfo = {};
  // // // vkCreateSwapchainKHR(device, &swapChainInfo, nullptr, &swapChain);

  // // // vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, nullptr);
  // // // VkImage swapChainImages[swapChainImageCount];
  // // // vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, swapChainImages);

  // // // for (swapChainImages) {
  // // // SetImageLayout();
  // // // VkImageMemoryBarrier imageMemoryBarrier = {};
  // // // vkCmdPipelineBarrier(cmd, srcStages, destStages, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
  // // // vkCreateImageView(device, &colorImageView,  nullptr, &scBuffer.view);
  // // // buffers.push_back(scBuffer);
  // // // }


//   return 0;
// }

// based on
// https://software.intel.com/en-us/articles/api-without-secrets-introduction-to-vulkan-part-2
// bool CheckExtensionAvailability(const char* extension_name,
// 				const std::vector<VkExtensionProperties>& available_extensions) {
//   for( size_t i = 0; i < available_extensions.size(); ++i ) {
//     if( strcmp( available_extensions[i].extensionName, extension_name ) == 0 ) {
//       return true;
//     }
//   }
//   return false;
// }


// bool checkKHR()
// {
//   uint32_t extensions_count = 0;
//   if( (vkEnumerateInstanceExtensionProperties( nullptr, &extensions_count, nullptr ) != VK_SUCCESS) ||
//       (extensions_count == 0) ) {
//     std::cout << "Error occurred during instance extensions enumeration!" << std::endl;
//     return false;
//   }

//   std::vector<VkExtensionProperties> available_extensions( extensions_count );
//   if( vkEnumerateInstanceExtensionProperties( nullptr, &extensions_count, &available_extensions[0] ) != VK_SUCCESS ) {
//     std::cout << "Error occurred during instance extensions enumeration!" << std::endl;
//     return false;
//   }

//   std::vector<const char*> extensions = {
//     VK_KHR_SURFACE_EXTENSION_NAME,
// #if defined(VK_USE_PLATFORM_WIN32_KHR)
//     VK_KHR_WIN32_SURFACE_EXTENSION_NAME
// #elif defined(VK_USE_PLATFORM_XCB_KHR)
//     VK_KHR_XCB_SURFACE_EXTENSION_NAME
// #elif defined(VK_USE_PLATFORM_XLIB_KHR)
//     VK_KHR_XLIB_SURFACE_EXTENSION_NAME
// #endif
//   };

//   for( size_t i = 0; i < extensions.size(); ++i ) {
//     if( !CheckExtensionAvailability( extensions[i], available_extensions ) ) {
//       std::cout << "Could not find instance extension named \"" << extensions[i] << "\"!" << std::endl;
//       return false;
//     }
//     else {
//       std::cout << "Instance extension named \"" << extensions[i] << "\" is available." << std::endl;
//     }
//   }

// }

// xcb
#include <xcb/xcb.h>
#include <xcb/xproto.h>


// loosely based on https://en.wikipedia.org/wiki/XCB
// and https://www.codeproject.com/Articles/1089819/An-Introduction-to-XCB-Programming
xcb_window_t CreateWindow(xcb_window_t *window, xcb_connection_t **conn) {

  *conn = xcb_connect(NULL,NULL);;
  if (xcb_connection_has_error(*conn)) {
    std::cerr << "XCB connection failed. Exiting";
  }

  /* Obtain setup info and access the screen */
  const xcb_setup_t *setup = xcb_get_setup(*conn);
  xcb_screen_t *screen = xcb_setup_roots_iterator(setup).data;;

  /* Create window */
  *window = xcb_generate_id(*conn);
  uint32_t prop_name = XCB_CW_BACK_PIXEL;
  uint32_t prop_value = screen->white_pixel;

  xcb_create_window(*conn, screen->root_depth,
		    *window, screen->root, 0, 0, 100, 100, 1,
		    XCB_WINDOW_CLASS_INPUT_OUTPUT,
		    screen->root_visual, prop_name, &prop_value);

  // *conn = xcb_connect(NULL,NULL);;
  // if (xcb_connection_has_error(*conn)) {
  //   LOG(FATAL) << "XCB connection failed. Exiting";
  // }

  // *window = xcb_generate_id(*conn);

  // xcb_screen_t *s = xcb_setup_roots_iterator(xcb_get_setup(*conn)).data;

  // uint8_t depth = XCB_COPY_FROM_PARENT;
  // xcb_window_t parent;
  // int16_t x = 100;
  // int16_t y = 100;
  // uint16_t width = 640;
  // uint16_t height = 480;
  // uint16_t border_width = 0;
  // uint16_t _class = XCB_WINDOW_CLASS_INPUT_OUTPUT;
  // xcb_visualid_t visual = XCB_COPY_FROM_PARENT;
  // uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  // const uint32_t value_list[2] = {s->white_pixel,
  //                   XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS};

  // xcb_void_cookie_t window_cookie = xcb_create_window(*conn, depth, *window, parent, x, y,
  //                               width, height, border_width,
  //                               _class, visual, value_mask, value_list);

}
