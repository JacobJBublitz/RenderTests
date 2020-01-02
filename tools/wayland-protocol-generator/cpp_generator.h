#pragma once

#include <list>
#include <ostream>
#include <unordered_map>
#include <utility>

#include "generator.h"

class CppGenerator final : public Generator {
 public:
  ~CppGenerator() final;

  void AddProtocol(const Protocol &protocol, bool add_include = true);

  void Generate(const Protocol &protocol) final;

  std::optional<std::pair<Interface, Enum>> LookupEnum(
      const std::string &name) const;

  std::optional<Interface> LookupInterface(const std::string &name) const;

 private:
  std::string root_namespace_ = "graphics::wayland::internal";

  std::list<std::string> additional_includes_;

  std::unordered_map<std::string, std::pair<Interface, Enum>> enums_;
  std::unordered_map<std::string, Interface> interfaces_;

  std::string FormatArgumentType(const Interface &interface,
                                 const Argument &argument) const;
  std::string FormatEnumEntry(const Interface &interface, const Enum &e,
                              const Enum::Entry &entry) const;
  std::string FormatEnumType(const Interface &interface, const Enum &e) const;
  std::string FormatEventName(const Interface &interface,
                              const Event &event) const;
  std::string FormatInterfaceType(const Interface &interface) const;
  std::string FormatInterfaceValue(const Interface &interface) const;
  std::string FormatRequestName(const Interface &interface,
                                const Request &request) const;

  void GenerateHeader(std::ostream &out, const Protocol &protocol);
  void GenerateSource(std::ostream &out, const std::string &base_header_name,
                      const Protocol &protocol);

  void MakeDocumentationBlock(std::ostream &out, const Description &desc,
                              size_t indentation = 0) const;
  void MakeEnumDefinition(std::ostream &out, const Interface &interface,
                          const Enum &e) const;
  void MakeInterfaceClassDefinition(std::ostream &out,
                                    const Interface &interface) const;
  void MakeInterfaceValueDefinition(std::ostream &out,
                                    const Interface &interface) const;
};
