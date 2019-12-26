#include "protocol.h"

#include <fmt/format.h>

#include <cstring>
#include <initializer_list>
#include <iostream>

using namespace tinyxml2;

namespace {

const std::list<std::pair<Argument::Type, const char *>> ArgumentTypeStringMap =
    {std::pair{Argument::Type::kArray, "array"},
     std::pair{Argument::Type::kFd, "fd"},
     std::pair{Argument::Type::kFixed, "fixed"},
     std::pair{Argument::Type::kInt, "int"},
     std::pair{Argument::Type::kNewId, "new_id"},
     std::pair{Argument::Type::kObject, "object"},
     std::pair{Argument::Type::kString, "string"},
     std::pair{Argument::Type::kUint, "uint"}};

const std::list<std::pair<Request::Type, const char *>> RequestTypeStringMap = {
    std::pair{Request::Type::kDestructor, "destructor"},
};

void HandleAttributeMissing(XMLError error, const XMLElement *element,
                            const char *attribute_name, bool required) {
  switch (error) {
    case XML_NO_ATTRIBUTE:
      if (required)
        throw std::runtime_error(fmt::format(
            "'{0}' on line {1} is missing required attribute '{2}'",
            element->Name(), element->GetLineNum(), attribute_name));
      break;
    default:
      throw std::runtime_error("Unknown attribute error");
  }
}

void ThrowBadChildElement(const XMLElement *parent, const XMLElement *child) {
  throw std::runtime_error(
      fmt::format("Element '{0}' had an unexpected child '{1}' (line: {2})",
                  parent->Name(), child->Name(), child->GetLineNum()));
}

void ThrowDuplicatedChildElement(const XMLElement *parent,
                                 const XMLElement *child) {
  throw std::runtime_error(fmt::format(
      "Element '{0}' has too many children of type '{1}' (line: {2})",
      parent->Name(), child->Name(), child->GetLineNum()));
}

template <typename T>
std::optional<T> TryMatch(const char *str,
                          const std::list<std::pair<T, const char *>> &map) {
  for (auto &pair : map) {
    if (strcmp(str, pair.second) == 0) return pair.first;
  }

  return std::optional<T>{};
}

template <typename T>
std::optional<const char *> TryToString(
    T value, const std::list<std::pair<T, const char *>> &map) {
  for (auto &pair : map) {
    if (value == pair.first) return pair.second;
  }

  return std::optional<const char *>{};
}

void VerifyNoExtraAttributes(
    const XMLElement *element,
    std::initializer_list<const char *> attribute_names) {
  for (auto attrib = element->FirstAttribute(); attrib;
       attrib = attrib->Next()) {
    bool found = false;
    for (auto it = attribute_names.begin(); it != attribute_names.end(); ++it) {
      if (strcmp(*it, attrib->Name()) == 0) {
        found = true;
        break;
      }
    }

    if (!found)
      throw std::runtime_error(
          fmt::format("Unknown attribute '{0}' for element '{1}' on line {2}.",
                      attrib->Name(), element->Name(), attrib->GetLineNum()));
  }
}

}  // namespace

Argument::Argument(const std::string &name, Type type,
                   const std::optional<std::string> &summary,
                   const std::optional<std::string> &interface,
                   const std::optional<std::string> &enum_opt, bool allowNull,
                   std::optional<uint32_t> since)
    : name_(name),
      type_(type),
      summary_(summary),
      interface_(interface),
      enum_(enum_opt),
      allowNull_(allowNull),
      since_(since) {}

Argument Argument::Parse(const XMLElement *element) {
  const char *name;
  if (auto error = element->QueryStringAttribute(kNameAttribute, &name);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kNameAttribute, true);

  const char *type_str;
  if (auto error = element->QueryStringAttribute(kTypeAttribute, &type_str);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kTypeAttribute, true);
  std::optional<Type> type = TryMatch(type_str, ArgumentTypeStringMap);
  if (!type)
    throw std::runtime_error(
        fmt::format("Unknown type '{0}' for argument '{1}' on line {2}",
                    type_str, name, element->GetLineNum()));

  const char *summary;
  std::optional<std::string> summary_opt;
  if (auto error = element->QueryStringAttribute(kSummaryAttribute, &summary);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kSummaryAttribute, false);
  else
    summary_opt = summary;

  const char *interface;
  std::optional<std::string> interface_opt;
  if (auto error =
          element->QueryStringAttribute(kInterfaceAttribute, &interface);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kInterfaceAttribute, false);
  else if (type != Type::kNewId && type != Type::kObject)
    throw std::runtime_error(fmt::format(
        "Cannot use '{0}' with type '{1}' in argument '{2}' on line {3}",
        kInterfaceAttribute, type_str, name, element->GetLineNum()));
  else
    interface_opt = interface;

  const char *enum_str;
  std::optional<std::string> enum_opt;
  if (auto error = element->QueryStringAttribute(kEnumAttribute, &enum_str);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kEnumAttribute, false);
  else if (type != Type::kInt && type != Type::kUint)
    throw std::runtime_error(
        fmt::format("The '{0}' attribute can only be used with arguments of "
                    "types 'int' or 'uint'. (Line: {1})",
                    kEnumAttribute, element->GetLineNum()));
  else
    enum_opt = enum_str;

  bool allow_null = false;
  if (auto error =
          element->QueryBoolAttribute(kAllowNullAttribute, &allow_null);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kAllowNullAttribute, false);

  uint32_t since;
  std::optional<uint32_t> since_opt = std::nullopt;
  if (auto error = element->QueryUnsignedAttribute(kSinceAttribute, &since);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kSinceAttribute, false);
  else
    since_opt = since;

  VerifyNoExtraAttributes(
      element,
      {kNameAttribute, kTypeAttribute, kSummaryAttribute, kInterfaceAttribute,
       kEnumAttribute, kAllowNullAttribute, kSinceAttribute});

  if (!element->NoChildren())
    throw std::runtime_error(fmt::format(
        "Arguments are not allowed to have child elements. (Line: {0})",
        element->GetLineNum()));

  return Argument{name,     type.value(), summary_opt, interface_opt,
                  enum_opt, allow_null,   since_opt};
}

Description::Description(const std::string &summary,
                         const std::optional<std::string> &text)
    : summary_(summary), text_(text) {}

Description Description::Parse(const XMLElement *element) {
  const char *summary;
  auto result = element->QueryStringAttribute(kSummaryAttribute, &summary);
  if (result != XML_SUCCESS)
    HandleAttributeMissing(result, element, kSummaryAttribute, true);

  std::optional<std::string> text;
  if (auto str = element->GetText(); str) text = str;

  return Description{summary, text};
}

Enum::Enum(const std::string &name, const std::list<Entry> &entries,
           bool bitfield, std::optional<uint32_t> since,
           const std::optional<Description> &description)
    : name_(name),
      entries_(entries),
      bitfield_(bitfield),
      since_(since),
      description_(description) {}

Enum Enum::Parse(const XMLElement *element) {
  const char *name;
  if (auto error = element->QueryStringAttribute(kNameAttribute, &name);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kNameAttribute, true);

  bool bitfield = false;
  if (auto error = element->QueryBoolAttribute(kBitfieldAttribute, &bitfield);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kBitfieldAttribute, false);

  uint32_t since;
  std::optional<uint32_t> since_opt;
  if (auto error = element->QueryUnsignedAttribute(kSinceAttribute, &since);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kSinceAttribute, false);
  else
    since_opt = since;

  VerifyNoExtraAttributes(
      element, {kNameAttribute, kBitfieldAttribute, kSinceAttribute});

  std::list<Entry> entries;
  std::optional<Description> description;

  for (auto child = element->FirstChildElement(); child;
       child = child->NextSiblingElement()) {
    if (strcmp(child->Name(), Description::kElement) == 0) {
      if (description)
        throw std::runtime_error(
            "Multiple descriptions specified for enum");  // TODO: Better error
                                                          // message
      else
        description = Description::Parse(child);
    } else if (strcmp(child->Name(), Entry::kElement) == 0)
      entries.push_back(Entry::Parse(child));
    else
      ThrowBadChildElement(element, child);
  }

  return Enum{name, entries, bitfield, since_opt, description};
}

Enum::Entry::Entry(const std::string &name, uint32_t value,
                   const std::optional<Description> &description,
                   std::optional<uint32_t> since)
    : name_(name), value_(value), description_(description), since_(since) {}

Enum::Entry Enum::Entry::Parse(const XMLElement *element) {
  const char *name;
  if (auto error = element->QueryStringAttribute(kNameAttribute, &name);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kNameAttribute, true);

  uint32_t value;
  if (auto error = element->QueryUnsignedAttribute(kValueAttribute, &value);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kValueAttribute, true);

  const char *summary;
  std::optional<std::string> summary_opt;
  if (auto error = element->QueryStringAttribute(kSummaryAttribute, &summary);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kSummaryAttribute, false);
  else
    summary_opt = summary;

  uint32_t since;
  std::optional<uint32_t> since_opt;
  if (auto error = element->QueryUnsignedAttribute(kSinceAttribute, &since);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kSinceAttribute, false);
  else
    since_opt = since;

  VerifyNoExtraAttributes(element, {kNameAttribute, kValueAttribute,
                                    kSummaryAttribute, kSinceAttribute});

  std::optional<Description> description_opt;

  for (auto child = element->FirstChildElement(); child;
       child = child->NextSiblingElement()) {
    if (strcmp(child->Name(), Description::kElement) == 0)
      description_opt = Description::Parse(child);
    else
      ThrowBadChildElement(element, child);
  }

  if (summary_opt && !description_opt)
    description_opt = Description{summary_opt.value()};

  return Entry{name, value, description_opt, since_opt};
}

Event Event::Parse(const XMLElement *element) {
  // Required name attribute
  const char *name;
  if (auto error = element->QueryStringAttribute(kNameAttribute, &name);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kNameAttribute, true);

  // Optional since attribute
  uint32_t since;
  std::optional<uint32_t> since_opt;
  if (auto error = element->QueryUnsignedAttribute(kSinceAttribute, &since);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kSinceAttribute, false);
  else
    since_opt = since;

  VerifyNoExtraAttributes(element, {kNameAttribute, kSinceAttribute});

  std::list<Argument> arguments;
  std::optional<Description> description;

  for (auto child = element->FirstChildElement(); child;
       child = child->NextSiblingElement()) {
    if (strcmp(child->Name(), Argument::kElement) == 0)
      arguments.push_back(Argument::Parse(child));
    else if (strcmp(child->Name(), Description::kElement) == 0) {
      if (description)
        ThrowDuplicatedChildElement(element, child);
      else
        description = Description::Parse(child);

    } else
      ThrowBadChildElement(element, child);
  }

  return Event{name, arguments, description, since_opt};
}

Interface Interface::Parse(XMLElement *element) {
  ::Interface interface;
  const char *name;
  auto result = element->QueryStringAttribute(kNameAttribute, &name);
  if (result != XML_SUCCESS)
    HandleAttributeMissing(result, element, kNameAttribute, true);
  interface.Name = name;

  result =
      element->QueryUnsignedAttribute(kVersionAttribute, &interface.Version);
  if (result != XML_SUCCESS)
    HandleAttributeMissing(result, element, kVersionAttribute, true);

  VerifyNoExtraAttributes(element, {kNameAttribute, kVersionAttribute});

  for (auto child = element->FirstChildElement(); child;
       child = child->NextSiblingElement()) {
    if (strcmp(child->Name(), Description::kElement) == 0)
      interface.Description = Description::Parse(child);
    else if (strcmp(child->Name(), Enum::kElement) == 0)
      interface.Enums.push_back(Enum::Parse(child));
    else if (strcmp(child->Name(), Event::kElement) == 0)
      interface.Events.push_back(Event::Parse(child));
    else if (strcmp(child->Name(), Request::kElement) == 0)
      interface.Requests.push_back(Request::Parse(child));
    else
      ThrowBadChildElement(element, child);
  }

  return interface;
}

Protocol Protocol::Parse(XMLElement *element) {
  Protocol proto;
  auto result = element->QueryStringAttribute(kNameAttribute, &proto.Name);
  if (result != XML_SUCCESS)
    HandleAttributeMissing(result, element, kNameAttribute, true);

  VerifyNoExtraAttributes(element, {kNameAttribute});

  for (auto child = element->FirstChildElement(); child;
       child = child->NextSiblingElement()) {
    if (strcmp(child->Name(), kCopyrightElement) == 0)
      proto.Copyright = child->GetText();
    else if (strcmp(child->Name(), Interface::kElement) == 0)
      proto.Interfaces.push_back(Interface::Parse(child));
    else
      ThrowBadChildElement(element, child);
  }

  return proto;
}

Request::Request(const std::string &name, Type type, ::Description description,
                 const std::list<Argument> &arguments,
                 std::optional<uint32_t> since)
    : name_(name),
      type_(type),
      description_(description),
      arguments_(arguments),
      since_(since) {}

Request Request::Parse(XMLElement *element) {
  const char *name;
  if (auto error = element->QueryStringAttribute(kNameAttribute, &name);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kNameAttribute, true);

  const char *type_str;
  Type type = Type::kDefault;
  if (auto error = element->QueryStringAttribute(kTypeAttribute, &type_str);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kTypeAttribute, false);
  else
    type = TryMatch(type_str, RequestTypeStringMap).value();

  uint32_t since;
  std::optional<uint32_t> since_opt;
  if (auto error = element->QueryUnsignedAttribute(kSinceAttribute, &since);
      error != XML_SUCCESS)
    HandleAttributeMissing(error, element, kSinceAttribute, false);
  else
    since_opt = since;

  VerifyNoExtraAttributes(element,
                          {kNameAttribute, kTypeAttribute, kSinceAttribute});

  std::list<Argument> arguments;
  std::optional<::Description> description;

  for (auto child = element->FirstChildElement(); child;
       child = child->NextSiblingElement()) {
    if (strcmp(child->Name(), Argument::kElement) == 0)
      arguments.push_back(Argument::Parse(child));
    else if (strcmp(child->Name(), Description::kElement) == 0)
      description = Description::Parse(child);
    else
      ThrowBadChildElement(element, child);
  }

  if (!description)
    throw std::runtime_error(fmt::format(
        "Request does not have required child element '{0}'. (Line {1})",
        Description::kElement, element->GetLineNum()));

  return Request{name, type, description.value(), arguments, since_opt};
}
