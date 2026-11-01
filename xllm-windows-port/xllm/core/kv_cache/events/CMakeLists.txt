# Copyright 2026 The xLLM Authors.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://github.com/xLLM-AI/xllm/blob/main/LICENSE
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include(cc_library)

# Alternative for ZeroMQ: Pure C++ WinSock2 (Strawberry/MinGW/MSYS2) implementation
# Since ZeroMQ is not installed and we are building a pure C++ RDNA2/RDNA4 Windows 11 port
# without vcpkg/Boost/Torch, we replace ZeroMQ with a lightweight WinSock2/MSYS2
# TCP socket implementation for KV event publishing.

# Define WIN32_ZMQ_ALTERNATIVE to enable the pure C++ socket alternative
add_compile_definitions(WIN32_ZMQ_ALTERNATIVE)

cc_library(
  NAME
    kv_cache_events
  HDRS
    kv_cache_event_journal.h
    kv_cache_event_publisher.h
  SRCS
    kv_cache_event_journal.cpp
    kv_cache_event_publisher.cpp
  DEPS
    glog::glog
    nlohmann_json::nlohmann_json
    Threads::Threads
    # WIN32_ZMQ_ALTERNATIVE replaces ZeroMQ dependency
    # WIN32/Strawberry/MinGW/MSYS2 sockets used instead
)
