#include <cyx/compression/decompress_context.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <zstd.h>
namespace cyx::compression {
class Decompress_stream_context_zstd : public Decompress_stream_context {
public:
  Decompress_stream_context_zstd() {
    dstream = ZSTD_createDStream();
    if (dstream == nullptr) {
      throw std::runtime_error("Failed to create compression stream");
    }
    ZSTD_initDStream(dstream);
  }

  ~Decompress_stream_context_zstd() override { ZSTD_freeDStream(dstream); }

  void decompress(std::span<const char> data) override {
    ZSTD_inBuffer input = {data.data(), data.size(), 0};

    while (input.pos < input.size) {
      auto output = create_out_buffer();
      size_t const code = ZSTD_decompressStream(dstream, &output, &input);

      if (ZSTD_isError(code)) {
        throw std::runtime_error(ZSTD_getErrorName(code));
      }

      call_write(output);
    }
  }

  void write_function(std::span<const char> write_data) {
    write_back_(write_data);
  }
  void
  set_write_function(std::function<void(std::span<const char>)> call) override {
    write_back_ = call;
  }
  void reset() override { ZSTD_DCtx_reset(dstream, ZSTD_reset_session_only); }

private:
  ZSTD_outBuffer create_out_buffer() {
    ZSTD_outBuffer output = {buffer.data(), buffer.size(), 0};
    return output;
  }
  void call_write(ZSTD_outBuffer &output) {
    if (output.pos > 0) {
      write_function({(const char *)output.dst, output.pos});
    }
  }

private:
  ZSTD_DStream *dstream;
  std::vector<char> buffer = std::vector<char>(ZSTD_DStreamOutSize());

  std::function<void(std::span<const char>)> write_back_;
};

std::unique_ptr<Decompress_stream_context> create_zstd_decompress_stream() {
  return std::make_unique<Decompress_stream_context_zstd>();
}
} // namespace cyx::compression