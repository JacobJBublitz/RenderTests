#include <tinyxml2.h>

#include <iostream>
#include <memory>

#include "cpp_generator.h"
#include "protocol.h"

namespace {

std::string ProtocolFileName;
std::string OutputFileName;

void PrintUsage(const char *invocation) {
  std::cout << "Usage: " << invocation << " <protocol file> <output file>\n";
}

}  // namespace

int main(int argc, const char **argv) {
  if (argc != 2) {
    PrintUsage(argv[0]);
    return 1;
  }

  std::cout << argv[1] << std::endl;

  auto generator = CppGenerator{};

  tinyxml2::XMLDocument protocol_data;
  protocol_data.LoadFile(argv[1]);

  auto protocol = Protocol::Parse(protocol_data.RootElement());
  generator.AddProtocol(protocol);

  generator.Generate(protocol);
}
