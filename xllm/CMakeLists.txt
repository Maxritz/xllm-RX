# Windows 11 ROCm 10.1 (7.16) Pure C++ RDNA2/RDNA4 Sub-project
# Boost and Torch dependencies ELIMINATED
# ZeroMQ dependency ELIMINATED (WinSock2 Alternative)
# Other dependencies (glog, nlohmann_json, protobuf, gflags, Threads, OpenSSL, leveldb, ZLIB, Python, etc.) VENDORED or REMOVED for pure C++ build without vcpkg

include(cc_binary)
include(cc_shared_library)

include_directories(.)

# Generate build-time version header from version.txt
file(READ "${CMAKE_SOURCE_DIR}/version.txt" XLLM_BUILD_VERSION_RAW)
string(STRIP "${XLLM_BUILD_VERSION_RAW}" XLLM_BUILD_VERSION)
if(XLLM_BUILD_VERSION MATCHES "^v(.+)$")
  set(XLLM_BUILD_VERSION "${CMAKE_MATCH_1}")
endif()
if(XLLM_BUILD_VERSION STREQUAL "")
  set(XLLM_BUILD_VERSION "unknown")
endif()
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/core/common")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/core/common/xllm_build_info.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/core/common/xllm_build_info.h"
  @ONLY)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Warning-as-error for xllm code (third-party headers are marked as SYSTEM)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  if(USE_MUSA)
    add_compile_options(
      $<$<COMPILE_LANGUAGE:CXX>:-Wno-defaulted-function-deleted>
      $<$<COMPILE_LANGUAGE:CXX>:-Wno-sign-conversion>
      $<$<COMPILE_LANGUAGE:CXX>:-Wno-inconsistent-missing-override>
      $<$<COMPILE_LANGUAGE:CXX>:-Wno-conversion>
    )
  else()
    add_compile_options(
      $<$<COMPILE_LANGUAGE:CXX>:-Werror>
      $<$<COMPILE_LANGUAGE:C>:-Werror>
    )
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
      $<$<COMPILE_LANGUAGE:CXX>:-Wno-macro-redefined>
      $<$<COMPILE_LANGUAGE:C>:-Wno-macro-redefined>
    )
  endif()
endif()

option(GENERATE_SO "Enable compile to generate .so" OFF)

# Pure C++ RDNA2/RDNA4 Subdirectories (Boost/Torch removed)
add_subdirectory(api_service)
add_subdirectory(compiler)
add_subdirectory(core)
add_subdirectory(function_call)
add_subdirectory(models)
add_subdirectory(parser)
add_subdirectory(processors)
add_subdirectory(proto)
# pybind removed for pure C++ port
add_subdirectory(server)

if(GENERATE_SO)
  message(STATUS "build libxllm.so and install")
  cc_shared_library(
    EXPORT_DEPS
    NAME
      xllm
    HDRS
      c_api/llm.h
      c_api/rec.h
      c_api/default.h
      c_api/types.h
      c_api/internal/helper.h
    SRCS
      c_api/internal/llm.cpp
      c_api/internal/rec.cpp
      c_api/internal/helper.cpp
    DEPS
      :config
      :master
      :vendor/absl_strings
      :vendor/gflags
      :vendor/glog
      :vendor/folly_subset
      :vendor/nlohmann_json
      :vendor/cpprest_subset
      :vendor/etcd_subset
  )
else()
  message(STATUS "build xllm binary and install")
  cc_binary(
    NAME
      xllm
    SRCS
      xllm.cpp
    DEPS
      :distributed_virtual_memory_coordinator
      :worker_memory_resources
      $<$<BOOL:${USE_NPU}>:npu_startup>
      :config
      :xllm_server
      :master
      :model_loader
      :kv_cache_store_config_validation
      :util
      :vendor/absl_strings
      :vendor/gflags
      :vendor/glog
      :vendor/folly_subset
      :vendor/nlohmann_json
  )
endif()

xllm_export_native_host(xllm)
xllm_export_native_host(xllm_export)

if(USE_NPU)
  include(npu_runtime)
  foreach(_native_host xllm xllm_export)
    xllm_configure_npu_runtime(${_native_host})
  endforeach()
endif()

# link brpc (Pure C++ alternative or vendored)
# Note: brpc, leveldb, protobuf, OpenSSL, ZLIB, Python, Threads are either removed or vendored
target_link_libraries(xllm PRIVATE :vendor/glog :vendor/nlohmann_json :vendor/absl_strings :vendor/gflags :vendor/folly_subset :vendor/cpprest_subset :vendor/etcd_subset)
# add_dependencies(xllm brpc-static) # REMOVED (brpc vendored or replaced)

# Pure C++ RDNA2/RDNA4 Link (No Torch/Boost/ZeroMQ/Python/Protobuf/etc.)
if(USE_NPU)
  set(COMMON_LIBS
      npu_profiler
      ascendcl
      atb_customize
      unified_dlog
      hccl
      c_sec
      nnopbase
      ms_tools_ext)
elseif(USE_MLU)
  # MLU specific removed for Windows ROCm RDNA2/RDNA4
  set(COMMON_LIBS)
endif()

if(USE_MSPTI)
  list(APPEND COMMON_LIBS mspti)
endif()
target_link_libraries(xllm PUBLIC ${COMMON_LIBS})

if(USE_MUSA)
  target_link_libraries(xllm PUBLIC atomic musa_python)
  target_link_options(xllm PRIVATE
    "LINKER:--whole-archive,$<TARGET_FILE:runtime>,--no-whole-archive"
  )
endif()

if (USE_XLITE)
  xllm_link_xlite(xllm)
endif()

# install xllm
if(GENERATE_SO)
  install(TARGETS xllm
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
  install(TARGETS xllm RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

# Install all dependencies for xllm
install(CODE [[\n  if("$<TARGET_PROPERTY:xllm,TYPE>" STREQUAL "SHARED_LIBRARY")\n    set(_xllm_artifact_kind LIBRARIES)\n  else()\n    set(_xllm_artifact_kind EXECUTABLES)\n  endif()\n  file(GET_RUNTIME_DEPENDENCIES\n        RESOLVED_DEPENDENCIES_VAR DEPENDENCIES\n        UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED_DEPENDENCIES\n        ${_xllm_artifact_kind}\n        $<TARGET_FILE:xllm>)\n\n  file(INSTALL\n        DESTINATION \"${CMAKE_INSTALL_PREFIX}/lib\"\n        FILES ${DEPENDENCIES}\n        FOLLOW_SYMLINK_CHAIN)\n\n  # This should not be possible, but error out when a dependency cannot\n  # be resolved.\n  list(LENGTH UNRESOLVED_DEPENDENCIES UNRESOLVED_LENGTH)\n  if(${UNRESOLVED_LENGTH} GREATER 0)\n      message(FATAL_ERROR \"Unresolved dependencies: ${UNRESOLVED_DEPENDENCIES}\")\n  endif()\n]])
