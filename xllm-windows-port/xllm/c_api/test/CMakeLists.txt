# Windows 11 ROCm 10.1 (7.16) Pure C++ RDNA2/RDNA4 C_API Test
# Boost and Torch dependencies ELIMINATED
# ZeroMQ dependency ELIMINATED (WinSock2 Alternative)
# Other dependencies (glog, nlohmann_json, absl, gflags, folly, cpprest, etcd, protobuf, gtest, benchmark, opencv, pybind11, rust, Threads, OpenSSL, leveldb, ZLIB, Python) VENDORED or REMOVED

cmake_minimum_required(VERSION 3.10)

project(xllm_capi_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------\n# Dependencies — Pure C++ RDNA2/RDNA4 Port (Vendored or Removed)\n# -----------------------------------------------------------------------------\n# All dependencies are now VENDORED in xllm-windows-port/vendor/
# or removed entirely (Python bindings, benchmarks, etc.)
# Skip system dependency checks (protobuf, gtest, benchmark, opencv, pybind11, rust, Threads, OpenSSL, leveldb, ZLIB, Python)

# Use vendor stubs instead of find_package
include_directories(${CMAKE_SOURCE_DIR}/../vendor/protobuf_subset)
include_directories(${CMAKE_SOURCE_DIR}/../vendor/gtest_subset)
include_directories(${CMAKE_SOURCE_DIR}/../vendor/benchmark_subset)
include_directories(${CMAKE_SOURCE_DIR}/../vendor/opencv_subset)
include_directories(${CMAKE_SOURCE_DIR}/../vendor/pybind11_subset)
include_directories(${CMAKE_SOURCE_DIR}/../vendor/rust_subset)
include_directories(${CMAKE_SOURCE_DIR}/../vendor/threads_subset)
include_directories(${CMAKE_SOURCE_DIR}/../vendor/openssl_subset)
include_directories(${CMAKE_SOURCE_DIR}/../vendor/leveldb_subset)
include_directories(${CMAKE_SOURCE_DIR}/../vendor/zlib_subset)
include_directories(${CMAKE_SOURCE_DIR}/../vendor/python_subset)

# -----------------------------------------------------------------------------\n# Generate protobuf sources from xllm_test.proto (Stubbed for pure C++ port)\n# -----------------------------------------------------------------------------\n# Note: Protobuf generation is stubbed for Windows 11 ROCm 10.1 Pure C++ Port
# to avoid system dependency on protoc/protobuf.
set(XLLM_TEST_PROTO ${CMAKE_CURRENT_LIST_DIR}/xllm_test.proto)
set(XLLM_TEST_PB_CC ${CMAKE_CURRENT_BINARY_DIR}/xllm_test.pb.cc)
set(XLLM_TEST_PB_H ${CMAKE_CURRENT_BINARY_DIR}/xllm_test.pb.h)

# Stub generation for pure C++ port (no protoc required)
file(WRITE "${XLLM_TEST_PB_CC}" "// Stub generated for Windows 11 ROCm 10.1 Pure C++ Port\n")
file(WRITE "${XLLM_TEST_PB_H}" "// Stub generated for Windows 11 ROCm 10.1 Pure C++ Port\n")

add_library(c_api_test_proto STATIC ${XLLM_TEST_PB_CC})
target_include_directories(c_api_test_proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
# Link vendor stubs instead of system libraries
target_link_libraries(c_api_test_proto PRIVATE :vendor/protobuf_subset)

# -----------------------------------------------------------------------------\n# brpc + gflags + glog (server binary; brpc uses glog — link glog after libbrpc.a)\n# -----------------------------------------------------------------------------\n# Use vendor stubs instead of find_package
target_link_libraries(c_api_test_proto PRIVATE :vendor/gflags :vendor/glog)

# brpc: stubbed for pure C++ port (no external brpc required)
set(BRPC_LIB "")
set(BRPC_INCLUDE_DIR "")

# leveldb, ZLIB, OpenSSL, Threads: stubbed for pure C++ port
target_link_libraries(c_api_test_proto PRIVATE :vendor/leveldb_subset :vendor/zlib_subset :vendor/openssl_subset :vendor/threads_subset)

# -----------------------------------------------------------------------------\n# Build xllm_test (links external /usr/local/xllm/lib/libxllm.so)\n# -----------------------------------------------------------------------------\nadd_executable(xllm_test
  ${CMAKE_CURRENT_LIST_DIR}/xllm_test.cpp
  ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
)

# rec.h / types.h: installed C API under /usr/local/xllm/include (run xllm/c_api/install.sh).
# Local test headers (utils.h): ${CMAKE_CURRENT_LIST_DIR}
target_include_directories(xllm_test
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    /usr/local/xllm/include
    ${CMAKE_CURRENT_LIST_DIR}
    ${BRPC_INCLUDE_DIR}
)
target_link_directories(xllm_test PRIVATE /usr/local/xllm/lib)
# Static libbrpc.a pulls protobuf gzip + glog symbols; libprotobuf must appear
# after brpc (or use --start-group) so GzipOutputStream etc. resolve; ZLIB for gzip.
# --start-group/--end-group: GNU ld needs them so libprotobuf resolves symbols
# referenced from libbrpc.a (e.g. GzipOutputStream) when linking static archives.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND NOT MSVC)
  set(_BRPC_LINK_GROUP_START -Wl,--start-group)
  set(_BRPC_LINK_GROUP_END -Wl,--end-group)
else()
  set(_BRPC_LINK_GROUP_START "")
  set(_BRPC_LINK_GROUP_END "")
endif()
target_link_libraries(xllm_test
  PRIVATE
    c_api_test_proto
    :vendor/gflags
    ${_BRPC_LINK_GROUP_START}
    :vendor/protobuf_subset
    :vendor/glog
    :vendor/zlib_subset
    ${_BRPC_LINK_GROUP_END}
    :vendor/leveldb_subset
    :vendor/openssl_subset
    :vendor/threads_subset
    dl
    xllm
)

# Keep runtime able to locate libxllm.so without setting LD_LIBRARY_PATH.
set_target_properties(xllm_test PROPERTIES
  BUILD_RPATH "/usr/local/xllm/lib"
  INSTALL_RPATH "/usr/local/xllm/lib"
)
