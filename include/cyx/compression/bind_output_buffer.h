#pragma once
#include <functional>
#include <span>
#include <string>

namespace cyx::compression {
template <typename T>
  requires requires(T t, std::function<void(std::span<const char>)> callback) {
    t.set_write_function(callback);
  }
void bind_output_buffer(T &ouput, std::string &output_buffer) {
  ouput.set_write_function([&output_buffer](std::span<const char> buffer) {
    output_buffer.append(buffer.data(), buffer.size());
  });
}
} // namespace cyx::compression