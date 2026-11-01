# Windows 11 ROCm 10.1 (7.16) Pure C++ RDNA2/RDNA4 Common
# Boost and Torch dependencies ELIMINATED

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
    absl::strings
    gflags::gflags
    glog::glog
    Folly::folly
    nlohmann_json::nlohmann_json
    # Boost::serialization REMOVED
    # torch REMOVED
    cpprest
    etcd-cpp-api
    $<$<BOOL:${USE_MLU}>:torch_mlu> # MLU specific, not Windows ROCm
)
