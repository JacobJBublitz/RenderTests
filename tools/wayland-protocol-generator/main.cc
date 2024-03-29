#include <iostream>
#include <memory>

#include "gflags/gflags.h"
#include "tinyxml2.h"
#include "tools/wayland-protocol-generator/cpp_generator.h"
#include "tools/wayland-protocol-generator/protocol.h"

namespace {

void PrintUsage(const char *invocation) {
  std::cout << "Usage: " << invocation
            << " <protocol file> [supporting protocols...]\n";
}

}  // namespace

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (argc < 2) {
    PrintUsage(argv[0]);
    return 1;
  }

  auto generator = CppGenerator{};

  for (int i = 2; i < argc; i++) {
    tinyxml2::XMLDocument protocol_data;
    protocol_data.LoadFile(argv[i]);
    auto protocol = Protocol::Parse(protocol_data.RootElement());
    generator.AddProtocol(protocol);
  }

  tinyxml2::XMLDocument protocol_data;
  protocol_data.LoadFile(argv[1]);

  auto protocol = Protocol::Parse(protocol_data.RootElement());
  generator.AddProtocol(protocol, false);

  generator.Generate(protocol);
}
