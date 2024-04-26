#include <cyx/compression/compress_context.h>
#include <cyx/compression/decompress_context.h>
#include <gtest/gtest.h>
#include <span>
#include <string>

TEST(Compress, zstd_compress) {
  std::string src = "a little string";
  auto compress = cyx::compression::create_zstd_compress_stream();
  auto decompress = cyx::compression::create_zstd_decompress_stream();
  std::string mid_result;
  std::string result;

  EXPECT_EQ(src, result);
}
