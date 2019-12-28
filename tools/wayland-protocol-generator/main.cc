#include <tinyxml2.h>

#include <iostream>
#include <memory>

#include "cpp_generator.h"
#include "protocol.h"

namespace {

std::string ProtocolFileName;
std::string OutputFileName;

void PrintUsage(const char *invocation) {
  std::cout << "Usage: " << invocation << " <protocol file> [supporting protocols...]\n";
}

}  // namespace

int main(int argc, const char **argv) {
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
