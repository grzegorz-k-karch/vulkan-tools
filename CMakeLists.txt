cmake_minimum_required(VERSION 3.7.1)

option(AUTO_LOCATE_VULKAN "AUTO_LOCATE_VULKAN" ON)

if (AUTO_LOCATE_VULKAN)
  message(STATUS "Attempting auto locate Vulkan using CMake...")

  find_package(Vulkan)

  if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
    set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
    # replace "/Include" with "" in ${VULKAN_PATH} and store in VULKAN_PATH
    STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
  endif()

  if (NOT Vulkan_FOUND)
    message(STATUS "Failed to locate Vulkan SDK, retrying...")
    if (EXISTS "${VULKAN_PATH}")
      message(STATUS "Successfully located the Vulkan SDK: ${Vulkan_Path}")
    else()
      message("Error: Unable to locate Vulkan SDK. Please turn off auto locate option by setting 'AUTO_LOCATE_VULKAN' to 'OFF' and specify manually path using 'VULKAN_SDK' and 'VULKAN_VERSION' variables in the CMakeLists.txt.")
      return()
    endif()
  endif()
else()
  message(STATUS "Attempting to locate Vulkan SDK using manual path...")
  set(VULKAN_SDK "")
  set(VULKAN_VERSION "1.0.33.0")
  set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
  message(STATUS "Using manually specified path: ${VULKAN_PATH}")
  if (NOT EXISTS "${VULKAN_PATH}")
    message("Error: Unable to locate this Vulkan SDK path VULKAN_PATH: ${VULKAN_PATH}, please specify correct path.")
    return()
  endif()
endif()


project(vulkan-skeleton)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.*)

add_executable(vulkan-skeleton ${CPP_FILES} ${HPP_FILES})

target_link_libraries(vulkan-skeleton
  vulkan
  glog
  xcb
  ${VULKAN_LIB_LIST})

set_property(TARGET vulkan-skeleton PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_property(TARGET vulkan-skeleton PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_property(TARGET vulkan-skeleton PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_property(TARGET vulkan-skeleton PROPERTY RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/bin) 
set_property(TARGET vulkan-skeleton PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin) 

set_property(TARGET vulkan-skeleton PROPERTY CXX_STANDARD 11)
set_property(TARGET vulkan-skeleton PROPERTY CXX_STANDARD_REQUIRED ON)

set_property(TARGET vulkan-skeleton PROPERTY C_STANDARD 99)
set_property(TARGET vulkan-skeleton PROPERTY C_STANDARD_RE`QUIRED ON)
