#include "cpp_generator.h"

#include <fmt/format.h>

#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>

namespace {

std::string ToUpperCamelCase(const std::string &str) {
  std::vector<char> data(str.size() + 1, '\0');

  char prev_char = '\0';
  size_t str_pos = 0;
  size_t data_pos = 0;
  while (str_pos < str.size()) {
    char c = str[str_pos];
    if (std::isalnum(c, std::locale::classic())) {
      if (!std::isalpha(prev_char, std::locale::classic())) {
        c = std::toupper(c, std::locale::classic());
      } else {
        c = std::tolower(c, std::locale::classic());
      }

      data[data_pos++] = c;
    }

    prev_char = c;
    str_pos++;
  }

  return data.data();
}

std::string TrimString(const std::string &str) {
  size_t start_pos = 0;
  size_t end_pos = str.size() - 1;

  while (std::isspace(str[start_pos])) start_pos++;
  while (std::isspace(str[end_pos])) end_pos--;

  return str.substr(start_pos, end_pos - start_pos);
}

std::string TryRemovePrefix(const std::string &str, const std::string &prefix) {
  if (str.size() < prefix.size()) return str;

  if (str.substr(0, prefix.size()) == prefix) return str.substr(prefix.size());

  return str;
}

}  // namespace

CppGenerator::~CppGenerator() = default;

void CppGenerator::AddProtocol(const Protocol &protocol) {
  for (const auto &interface : protocol.Interfaces) {
    interfaces_[interface.Name] = interface;

    for (const auto &e : interface.Enums) {
      enums_.insert({fmt::format("{0}.{1}", interface.Name, e.GetName()),
                     std::pair{interface, e}});
    }
  }
}

std::string CppGenerator::FormatArgumentType(const Interface &interface,
                                             const Argument &argument) const {
  if (argument.GetEnum()) {
    if (auto e = LookupEnum(interface.Name + "." + argument.GetEnum().value());
        e)
      return FormatEnumType(e.value().first, e.value().second) + ' ';
    if (auto e = LookupEnum(argument.GetEnum().value()); e)
      return FormatEnumType(e.value().first, e.value().second) + ' ';

    throw std::runtime_error(
        fmt::format("Unresolved enum '{0}' for argument '{1}'",
                    argument.GetEnum().value(), argument.GetName()));
  }

  if (argument.GetInterface()) {
    if (auto i = LookupInterface(argument.GetInterface().value()); i)
      return fmt::format("{0}::{1} *", root_namespace_,
                         FormatInterfaceType(i.value()));

    throw std::runtime_error(fmt::format("Unresolved interface '{0}'",
                                         argument.GetInterface().value()));
  }

  switch (argument.GetType()) {
    case Argument::Type::kArray:
      return "Array *";
    case Argument::Type::kFd:
      return "int ";
    case Argument::Type::kFixed:
      return "Fixed ";
    case Argument::Type::kInt:
      return "int32_t ";
    case Argument::Type::kNewId:
      return "Proxy *";
    case Argument::Type::kObject:
      return "void *";
    case Argument::Type::kString:
      return "const char *";
    case Argument::Type::kUint:
      return "uint32_t ";
    default:
      throw std::logic_error(
          fmt::format("Unable to determine C++ type for argument '{0}'",
                      argument.GetName()));
  }
}

std::string CppGenerator::FormatEnumEntry(const Interface &interface,
                                          const Enum &e,
                                          const Enum::Entry &entry) const {
  return fmt::format("k{0}", ToUpperCamelCase(entry.GetName()));
}

std::string CppGenerator::FormatEnumType(const Interface &interface,
                                         const Enum &e) const {
  return FormatInterfaceType(interface) + ToUpperCamelCase(e.GetName());
}

std::string CppGenerator::FormatEventName(const Interface &interface,
                                          const Event &event) const {
  return ToUpperCamelCase(event.GetName());
}

std::string CppGenerator::FormatInterfaceType(
    const Interface &interface) const {
  auto name = interface.Name;
  if (name.substr(0, 3) == "wl_") {
    name = name.substr(3);
  }

  return ToUpperCamelCase(name);
}

std::string CppGenerator::FormatInterfaceValue(
    const Interface &interface) const {
  return FormatInterfaceType(interface) + "Interface";
}

void CppGenerator::Generate(const Protocol &protocol) {
  std::ofstream header_output_stream{
      fmt::format("{0}.{1}", protocol.Name, header_extension_)};
  GenerateHeader(header_output_stream, protocol);

  std::ofstream source_output_stream{
      fmt::format("{0}.{1}", protocol.Name, source_extension_)};
  source_output_stream << "#include \"wayland.h\"\n";
}

std::string CppGenerator::FormatRequestName(const Interface &interface,
                                            const Request &request) const {
  return ToUpperCamelCase(request.GetName());
}

void CppGenerator::GenerateHeader(std::ostream &out, const Protocol &protocol) {
  std::stringstream class_declarations;
  std::stringstream interface_declarations;
  std::stringstream enum_definitions;
  std::stringstream class_definitions;

  std::list<Interface> interfaces = protocol.Interfaces;
  interfaces.sort([this](const auto &a, const auto &b) {
    return FormatInterfaceType(a) < FormatInterfaceType(b);
  });
  for (auto &interface : interfaces) {
    class_declarations << "class " << FormatInterfaceType(interface) << ";\n";
    MakeInterfaceClassDefinition(class_definitions, interface);
    interface_declarations << "extern const Interface "
                           << FormatInterfaceValue(interface) << ";\n";

    for (auto &e : interface.Enums) {
      MakeEnumDefinition(enum_definitions, interface, e);
    }
  }

  out << "#pragma once\n\n"
      << "\n"
      << "#include <cstdint>\n"
      << "\n"
      << "#include \"core.h\"\n"
      << "#include \"util.h\"\n"
      << "\n"
      << "namespace " << root_namespace_ << " {\n\n";

  out << class_declarations.str() << "\n"
      << interface_declarations.str() << "\n"
      << enum_definitions.str() << "\n"
      << class_definitions.str() << "\n";

  out << "\n}  // namespace " << root_namespace_ << "\n";
}

void CppGenerator::GenerateSource(std::ostream &out, const Protocol &protocol) {

}

std::optional<std::pair<Interface, Enum>> CppGenerator::LookupEnum(
    const std::string &name) const {
  if (auto result = enums_.find(name); result != enums_.end()) {
    return result->second;
  }

  return std::nullopt;
}

std::optional<Interface> CppGenerator::LookupInterface(
    const std::string &name) const {
  if (auto result = interfaces_.find(name); result != interfaces_.end()) {
    return result->second;
  }

  return std::nullopt;
}

void CppGenerator::MakeDocumentationBlock(std::ostream &out,
                                          const Description &desc,
                                          size_t indentation) const {
  std::string indent(indentation, ' ');
  if (desc.GetText()) {
    out << indent << "//! " << desc.GetSummary() << "\n";

    std::string text = desc.GetText().value();
    std::stringstream text_stream{'\t' + TrimString(text)};

    std::string line;
    while (std::getline(text_stream, line, '\n')) {
      if (!line.empty()) {
        line = line.substr(1);
      }
      out << indent << "/// " << line << "\n";
    }
  } else {
    out << indent << "/// " << desc.GetSummary() << "\n";
  }
}

void CppGenerator::MakeEnumDefinition(std::ostream &out,
                                      const Interface &interface,
                                      const Enum &e) const {
  if (e.GetDescription()) {
    MakeDocumentationBlock(out, e.GetDescription().value());
  }
  out << "enum class " << FormatEnumType(interface, e) << " : uint32_t {\n";
  for (auto &entry : e.GetEntries()) {
    if (entry.GetDescription()) {
      MakeDocumentationBlock(out, entry.GetDescription().value(), 2);
    }
    out << "  " << FormatEnumEntry(interface, e, entry) << " = "
        << entry.GetValue() << ",\n";
  }
  out << "};\n";
}

void CppGenerator::MakeInterfaceClassDefinition(
    std::ostream &out, const Interface &interface) const {
  auto base_class = "Proxy";
  // Special base class for display
  if (interface.Name == "wl_display") base_class = "DisplayBase";

  out << "class " << FormatInterfaceType(interface) << " final : public "
      << base_class << " {\n"
      << " public:\n";

  // Event declarations
  if (!interface.Events.empty()) {
    out << "  struct Listener {\n";
    for (const auto &ev : interface.Events) {
      std::stringstream docs_stream;
      if (ev.GetDescription())
        MakeDocumentationBlock(docs_stream, ev.GetDescription().value(), 4);

      std::stringstream params_stream;
      params_stream << "void *data, " << FormatInterfaceType(interface)
                    << " *this_ptr";
      for (const auto &arg : ev.GetArguments()) {
        if (arg.GetSummary()) {
          docs_stream << "    /// \\param " << arg.GetName() << " "
                      << arg.GetSummary().value() << "\n";
        }

        params_stream << ", " << FormatArgumentType(interface, arg)
                      << arg.GetName();
      }

      out << docs_stream.str();
      out << "    void (*" << ToUpperCamelCase(ev.GetName()) << ")("
          << params_stream.str() << ");\n";
    }
    out << "  };\n\n";
    out << "  int AddListener(const Listener &listener, void *data) {\n"
        << "    return static_cast<Proxy *>(this)->AddListener(\n"
        << "        const_cast<void (**)()>(reinterpret_cast<void (*const "
           "*)()>(&listener)),\n"
        << "        data);\n"
        << "  };\n";
  }

  uint32_t request_opcode = 0;
  for (auto &req : interface.Requests) {
    std::optional<std::string> ret_type;
    std::optional<Interface> ret_interface;
    std::stringstream param_stream;
    std::stringstream param_names;
    std::stringstream body_stream;
    for (auto &arg : req.GetArguments()) {
      std::string type = FormatArgumentType(interface, arg);

      if (arg.GetType() == Argument::Type::kNewId) {
        ret_type = FormatArgumentType(interface, arg);
        if (arg.GetInterface()) {
          ret_interface = LookupInterface(arg.GetInterface().value()).value();
        } else {
          if (!param_stream.str().empty()) param_stream << ", ";
          if (!param_names.str().empty()) param_names << ", ";

          param_stream << "const Interface *interface, uint32_t version";
          param_names << "interface->Name, version";
        }
      } else {
        if (!param_stream.str().empty()) param_stream << ", ";
        if (!param_names.str().empty()) param_names << ", ";

        param_stream << type << arg.GetName();
        if (arg.GetEnum()) {
          // If it's an enum cast it to a uint32_t
          param_names << "static_cast<uint32_t>(" << arg.GetName() << ")";
        } else if (arg.GetInterface()) {
          // If it's an interface cast it to an Object
          param_names << "reinterpret_cast<Object *>(" << arg.GetName() << ")";
        } else {
          param_names << arg.GetName();
        }
      }
    }

    if (req.GetType() == Request::Type::kDestructor) {
      body_stream << "    Marshal(" << request_opcode << ");\n"
                  << "    static_cast<Proxy *>(this)->Destroy();\n";
    } else if (ret_type && !ret_interface) {
      // A return type but no interface means that we are just returing a proxy
      // and the interface and version are specified as parameters
      body_stream << "  return MarshalConstructorVersioned(" << request_opcode
                  << ", interface, version, " << param_names.str()
                  << ", nullptr);\n";
    } else if (ret_interface) {
      // A return interface means that we have to return a new proxy object.
      body_stream << "    return reinterpret_cast<"
                  << FormatInterfaceType(ret_interface.value()) << " *>(\n"
                  << "        MarshalConstructor(" << request_opcode << ", &"
                  << FormatInterfaceValue(ret_interface.value())
                  << ", nullptr));\n";
    } else {
      // If theres's nothing else special just use Marshal to send the request
      // to the server
      body_stream << "    Marshal(" << request_opcode;

      if (!param_names.str().empty()) {
        body_stream << ", " << param_names.str();
      }
      body_stream << ");\n";
    }

    MakeDocumentationBlock(out, req.GetDescription(), 2);
    out << "  " << ret_type.value_or("void ")
        << FormatRequestName(interface, req) << "(" << param_stream.str()
        << ") {\n"
        << body_stream.str() << "  }\n";
    request_opcode++;
  }

  out << "};\n";
}

void CppGenerator::MakeInterfaceValueDefinition(
    std::ostream &out, const Interface &interface) const {}
