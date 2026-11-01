# Windows 11 ROCm 10.1 (7.16) Pure C++ RDNA2/RDNA4 Common
# Boost and Torch dependencies ELIMINATED
# ZeroMQ dependency ELIMINATED (WinSock2 Alternative)

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
)
