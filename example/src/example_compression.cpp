
#include <cassert>
#include <cyx/compression/compress_context.h>
#include <cyx/compression/decompress_context.h>
#include <iostream>
#include <string>
int main() {
  using namespace cyx::compression;
  std::string src = "abc";
  std::string out;
  {
    auto compress = create_zstd_compress_stream();
    compress->set_write_function(
        [&out](auto buf) { out.append(buf.data(), buf.size()); });
    compress->compress(src);
  }
  std::cout << "Compress result, len: " << out.size() << "  " << out << "\n";

  std::string de_out;
  auto decompress = create_zstd_decompress_stream();

  decompress->set_write_function(
      [&de_out](auto buf) { de_out.append(buf.data(), buf.size()); });
  decompress->decompress(out);
  std::cout << "Decompress size: " << de_out.size() << "\n";
  std::cout << "Decompress result: " << de_out << "\n";
  assert(src == de_out);
}