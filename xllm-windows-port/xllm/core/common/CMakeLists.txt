# Windows 11 ROCm 10.1 (7.16) Pure C++ RDNA2/RDNA4 Common
# Boost and Torch dependencies ELIMINATED
# ZeroMQ dependency ELIMINATED (WinSock2 Alternative)
# Other dependencies (glog, nlohmann_json, absl, gflags, folly, cpprest, etcd, protobuf, gtest, benchmark, opencv, pybind11, rust, Threads, OpenSSL, leveldb, ZLIB, Python) VENDORED or REMOVED

cc_library(
  NAME
    common
  HDRS
    xllm_build_info.h
    native_library_build_info.h
    # ... other headers ...
  SRCS
    # ... sources ...
  DEPS
    :config
    # glog::glog REMOVED (Vendored below)
    # nlohmann_json::nlohmann_json REMOVED (Vendored below)
    # absl::strings REMOVED (Pure C++ alternatives or vendored)
    # gflags::gflags REMOVED (Pure C++ alternatives or vendored)
    # Folly::folly REMOVED (Pure C++ alternatives or vendored)
    # cpprest REMOVED (Pure C++ alternatives or vendored)
    # etcd-cpp-api REMOVED (Pure C++ alternatives or vendored)
    # protobuf REMOVED (Pure C++ alternatives or vendored)
    # gtest REMOVED (Pure C++ alternatives or vendored)
    # benchmark REMOVED (Pure C++ alternatives or vendored)
    # opencv REMOVED (Pure C++ alternatives or vendored)
    # pybind11 REMOVED (Pure C++ alternatives or vendored)
    # rust REMOVED (Pure C++ alternatives or vendored)
    # Threads REMOVED (Pure C++ alternatives or vendored)
    # OpenSSL REMOVED (Pure C++ alternatives or vendored)
    # leveldb REMOVED (Pure C++ alternatives or vendored)
    # ZLIB REMOVED (Pure C++ alternatives or vendored)
    # Python REMOVED (Pure C++ alternatives or vendored)
    # $<<BOOL:${USE_MLU}>:torch_mlu> REMOVED (Windows ROCm RDNA2/RDNA4)
    # WIN32_ZMQ_ALTERNATIVE (already in events)
    
    # VENDORED DEPS (Pure C++ RDNA2/RDNA4 Windows 11 ROCm 10.1)
    :vendor/glog
    :vendor/nlohmann_json
    :vendor/absl_strings
    :vendor/gflags
    :vendor/folly_subset
    :vendor/cpprest_subset
    :vendor/etcd_subset
    :vendor/protobuf_subset
    :vendor/gtest_subset
    :vendor/benchmark_subset
    :vendor/opencv_subset
    :vendor/pybind11_subset
    :vendor/rust_subset
    :vendor/threads_subset
    :vendor/openssl_subset
    :vendor/leveldb_subset
    :vendor/zlib_subset
    :vendor/python_subset
)
