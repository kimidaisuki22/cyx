#pragma once
#include <functional>
#include <memory>
#include <span>
#include <vector>
namespace cyx::compression {
class Compress_stream_context {
public:
  virtual ~Compress_stream_context() = default;

  virtual void compress(std::span<const char> data) = 0;
  virtual void write_function(std::span<const char> write_data) = 0;
  virtual void
  set_write_function(std::function<void(std::span<const char>)> callback) = 0;
  virtual void reset() = 0;
  virtual void flush() = 0;
};

class Compress_context {
public:
  virtual ~Compress_context() = default;

  virtual std::vector<char> compress(std::span<const char> data) = 0;
};

std::unique_ptr<Compress_stream_context>
create_zstd_compress_stream(int level = 3);
} // namespace cyx::compression