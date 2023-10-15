#include <filesystem>
#include <iostream>
int main(int argc, char **argv) {
  std::filesystem::path path =
      argc > 1 ? argv[1] : std::filesystem::current_path();

}