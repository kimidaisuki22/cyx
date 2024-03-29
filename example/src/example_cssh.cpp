#include <iostream>
#include <libssh2.h>
#include <spdlog/spdlog.h>
#include <string>

int main() {
  auto error = libssh2_init(0);
  if (error) {
    SPDLOG_ERROR("Failed to init libssh2.");
  }
}
