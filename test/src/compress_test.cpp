#include "cyx/compression/bind_output_buffer.h"
#include <cyx/compression/compress_context.h>
#include <cyx/compression/decompress_context.h>
#include <gtest/gtest.h>
#include <span>
#include <string>

TEST(Compress, zstd_compress) {
  const std::string src = "a little string";
  auto compress = cyx::compression::create_zstd_compress_stream();
  auto decompress = cyx::compression::create_zstd_decompress_stream();
  std::string mid_result;
  std::string result;

  cyx::compression::bind_output_buffer(*compress, mid_result);
  cyx::compression::bind_output_buffer(*decompress, result);
  compress->compress(src);
  compress->flush();
  EXPECT_TRUE(!mid_result.empty());
  decompress->decompress(mid_result);

  EXPECT_EQ(src, result);
}

TEST(Compress, zstd_compress_direct) {
  const std::string src = "a little string";
  auto compress = cyx::compression::create_zstd_compress();
  auto decompress = cyx::compression::create_zstd_decompress_stream();
  std::string result;

  cyx::compression::bind_output_buffer(*decompress, result);
  auto mid_result = compress->compress(src);

  EXPECT_TRUE(!mid_result.empty());
  decompress->decompress(mid_result);

  EXPECT_EQ(src, result);
}

TEST(Compress, zstd_compress_direct_compress_levels) {
  const std::string src = "a little string";
  auto compress = cyx::compression::create_zstd_compress();
  for (int i = 1; i < 19; i++) {
    auto decompress = cyx::compression::create_zstd_decompress_stream();
    std::string result;

    cyx::compression::bind_output_buffer(*decompress, result);
    auto mid_result = compress->compress(src, i);

    EXPECT_TRUE(!mid_result.empty());
    decompress->decompress(mid_result);

    EXPECT_EQ(src, result);
  }
}
