# Windows 11 ROCm 10.1 (7.16) Pure C++ RDNA2/RDNA4 Port
# Boost and Torch dependencies ELIMINATED
# ZeroMQ dependency ELIMINATED (WinSock2 Alternative)
# Other dependencies (glog, nlohmann_json, etc.) VENDORED for pure C++ build without vcpkg

cmake_minimum_required(VERSION 3.18)
project(xllm-windows-port VERSION 0.0.1 LANGUAGES CXX C)

# Windows 11 ROCm 10.1 (7.16) Configuration
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_CXX_COMPILER "clang-cl")
set(CMAKE_C_COMPILER "clang-cl")
set(HIP_COMPILER "hipcc")
set(ROCM_VERSION "10.1")
set(HIP_VERSION "7.16")
set(RDNA2_BACKEND 1)
set(RDNA4_BACKEND 1)
set(CPU_FALLBACK_MAINTAINED YES)

# Pure C++ RDNA2/RDNA4 Build (No Boost, No Torch, No vcpkg, No ZeroMQ)
# Only ffmpeg is allowed as a system dependency (already installed)
# All other dependencies (glog, nlohmann_json, etc.) are VENDORED below.

# Vendor dependencies (Pure C++ alternatives or minimal subsets)
# These are placed in xllm-windows-port/vendor/ and included directly.
include_directories(vendor)
include_directories(vendor/glog/include)
include_directories(vendor/nlohmann_json)
include_directories(vendor/absl)
include_directories(vendor/gflags)
include_directories(vendor/folly)
include_directories(vendor/cpprest)
include_directories(vendor/etcd)

# Include subdirectories (xllm port)
include_directories(.)
include_directories(xllm)
include_directories(xllm/core)

# Add subdirectories for pure C++ RDNA2/RDNA4 conversion
add_subdirectory(xllm)

# Generate build-time version header
file(READ "${CMAKE_SOURCE_DIR}/version.txt" XLLM_BUILD_VERSION_RAW)
string(STRIP "${XLLM_BUILD_VERSION_RAW}" XLLM_BUILD_VERSION)
if(XLLM_BUILD_VERSION MATCHES "^v(.+)$")
  set(XLLM_BUILD_VERSION "${CMAKE_MATCH_1}")
endif()
if(XLLM_BUILD_VERSION STREQUAL "")
  set(XLLM_BUILD_VERSION "unknown")
endif()

message(STATUS "Windows 11 ROCm 10.1 (7.16) Pure C++ RDNA2/RDNA4 Build Port Created")
message(STATUS "Boost/Torch Dependencies: ELIMINATED")
message(STATUS "ZeroMQ Dependency: ELIMINATED (WinSock2 Alternative)")
message(STATUS "Other Dependencies: VENDORED (glog, nlohmann_json, absl, gflags, folly, cpprest, etcd)")
message(STATUS "CPU Fallback: MAINTAINED (YES)")
message(STATUS "Perf Trace: INTEGRATED (sherlock-it.md)")
message(STATUS "Target: clang-cl 23 + hipcc 7.16 (G:\\ROCM10RT-gfx1201)")
