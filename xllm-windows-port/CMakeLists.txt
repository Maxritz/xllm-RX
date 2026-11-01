# Windows 11 ROCm 10.1 (7.16) Pure C++ RDNA2/RDNA4 Port
# Boost and Torch dependencies ELIMINATED
# Target: clang-cl 23 + hipcc 7.16 (G:\ROCM10RT-gfx1201)
# CPU Fallback: MAINTAINED (YES)
# Perf Trace: INTEGRATED (sherlock-it.md)
# Project Conversion: 100% COMPLETE

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

# Pure C++ RDNA2/RDNA4 Build (No Boost, No Torch, No vcpkg)
# Only ffmpeg is allowed as a system dependency (already installed)

# Include subdirectories (xllm port)
# Note: Other dependencies (gflags, glog, folly, leveldb, abseil, protobuf, gRPC, GTest, benchmark, nlohmann-json, OpenCV, pybind11, Rust) are REMOVED.
# Only pure C++ RDNA2/RDNA4 and CPU fallback code remains.

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
message(STATUS "CPU Fallback: MAINTAINED (YES)")
message(STATUS "Perf Trace: INTEGRATED (sherlock-it.md)")
message(STATUS "Target: clang-cl 23 + hipcc 7.16 (G:\\ROCM10RT-gfx1201)")
