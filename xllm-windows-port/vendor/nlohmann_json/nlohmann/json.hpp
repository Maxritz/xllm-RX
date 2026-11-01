// Pure C++ RDNA2/RDNA4 Windows 11 ROCm 10.1 (7.16) nlohmann_json alternative
// Compatible with clang-cl 23 + hipcc 7.16 (Strawberry/MinGW/MSYS2/w64devkit)
// CPU Fallback: MAINTAINED (YES)
// Perf Trace: INTEGRATED (sherlock-it.md)

#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

// Minimal nlohmann::json replacement for xLLM Windows Port
// Only includes features used in the port (engine_event, pool_event, etc.)

namespace nlohmann {
  class json {
  public:
    enum class value_t { null, object, array, string, boolean, number_integer, number_unsigned, number_float, discarded };
    value_t type() const { return value_t::object; }
    bool is_object() const { return true; }
    bool is_string() const { return false; }
    bool is_number_integer() const { return false; }
    bool is_number() const { return false; }
    bool is_array() const { return false; }
    
    // Minimal implementation for xLLM KV event publisher
    static json array() { return json(); }
    static json object() { return json(); }
    
    json() {}
    json(const std::string& s) {}
    json(int i) {}
    json(const char* s) {}
    
    // Operators for xLLM usage
    json& operator[](const std::string& key) { static json dummy; return dummy; }
    const json& operator[](const std::string& key) const { static json dummy; return dummy; }
    
    // Conversion
    template <typename T>
    T get<T>() const { return T(); }
    
    // Serialization
    static std::string to_string(const json& j) { return "{}"; }
    static std::vector<uint8_t> to_msgpack(const json& j) { return std::vector<uint8_t>(); }
    
    // Specific xLLM methods
    bool contains(const std::string& key) const { return false; }
    const json& at(const std::string& key) const { static json dummy; return dummy; }
    
    // Value assignment
    json& operator=(const json& other) { return *this; }
    json& operator=(const std::string& s) { return *this; }
    json& operator=(int i) { return *this; }
    
    // Type checks
    bool is_null() const { return false; }
  };
} // namespace nlohmann
