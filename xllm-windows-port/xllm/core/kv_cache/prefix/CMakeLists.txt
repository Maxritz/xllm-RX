# Windows 11 ROCm 10.1 (7.16) Pure C++ RDNA2/RDNA4 KV Cache Prefix
# Boost and Torch dependencies ELIMINATED

include(cc_library)

cc_library(
  NAME
    block_hasher
  HDRS
    block_hasher.h
  SRCS
    block_hasher.cpp
  DEPS
    :config
    :multimodal
    glog::glog
    xxHash
    # torch REMOVED
)

cc_library(
  NAME
    prefix_cache
  HDRS
    prefix_cache.h
    linear_state_prefix_cache.h
    prefix_cache_factory.h
  SRCS
    prefix_cache.cpp
    linear_state_prefix_cache.cpp
    prefix_cache_factory.cpp
  DEPS
    :kv_cache_events
    :block_hasher
    :config
    :kv_cache_block_types
    :multimodal
    :common
    glog::glog
    absl::strings
    absl::time
    SMHasherSupport
    xxHash
    # Boost::serialization REMOVED
    # torch REMOVED
)
