#pragma once
#include <functional>
#include <memory>
#include <span>
#include <vector>
namespace cyx::compression {
class Decompress_stream_context {
public:
  virtual ~Decompress_stream_context() = default;

  virtual void decompress(std::span<const char> data) = 0;
  // Might we use generator rather than callback?
  virtual void
  set_write_function(std::function<void(std::span<const char>)> callback) = 0;
  virtual void reset() = 0;
};

class Decompress_context {
public:
  virtual ~Decompress_context() = default;

  virtual std::vector<char> decompress(std::span<const char> data) = 0;
};

std::unique_ptr<Decompress_stream_context> create_zstd_decompress_stream();
std::unique_ptr<Decompress_context> create_zstd_decompress();
} // namespace cyx::compression