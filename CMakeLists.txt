cmake_minimum_required(VERSION 3.7.1)

# set(CMAKE_MODULE PATH  "/usr/local/share/cmake-3.14/Modules/")
# find_package(Vulkan HINTS "/usr/local/share/cmake-3.14/Modules/") # NO_DEFAULT_PATH)
find_package(Vulkan)

if (NOT Vulkan_FOUND)
  message(SEND_ERROR "Failed to locate Vulkan SDK")
  return()
endif()

project(vulkan-tools)

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(vulkan-tools ${CPP_FILES} ${HPP_FILES})

target_link_libraries(vulkan-tools
  vulkan
  xcb)

set_property(TARGET vulkan-tools PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_property(TARGET vulkan-tools PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_property(TARGET vulkan-tools PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_property(TARGET vulkan-tools PROPERTY RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/bin) 
set_property(TARGET vulkan-tools PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin) 

set_property(TARGET vulkan-tools PROPERTY CXX_STANDARD 11)
set_property(TARGET vulkan-tools PROPERTY CXX_STANDARD_REQUIRED ON)

set_property(TARGET vulkan-tools PROPERTY C_STANDARD 99)
set_property(TARGET vulkan-tools PROPERTY C_STANDARD_REQUIRED ON)
