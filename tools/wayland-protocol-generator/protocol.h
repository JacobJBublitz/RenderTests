#pragma once

#include <tinyxml2.h>

#include <list>
#include <optional>
#include <string>

class Argument final {
 public:
  enum class Type {
    kArray,
    kFd,
    kFixed,
    kInt,
    kNewId,
    kObject,
    kString,
    kUint
  };

  static constexpr const char *kElement = "arg";

  Argument(const std::string &name, Type type,
           const std::optional<std::string> &summary,
           const std::optional<std::string> &interface = std::nullopt,
           const std::optional<std::string> &enum_opt = std::nullopt,
           bool allowNull = false,
           std::optional<uint32_t> since = std::nullopt);

  static Argument Parse(const tinyxml2::XMLElement *element);

  inline const std::string &GetName() const { return name_; }

  inline Type GetType() const { return type_; }

  inline const std::optional<std::string> &GetSummary() const {
    return summary_;
  }

  inline const std::optional<std::string> &GetInterface() const {
    return interface_;
  }

  inline const std::optional<std::string> &GetEnum() const { return enum_; }

  inline bool GetAllowNull() const { return allowNull_; }

  inline std::optional<uint32_t> GetSince() const { return since_; }

 private:
  static constexpr const char *kNameAttribute = "name";
  static constexpr const char *kTypeAttribute = "type";
  static constexpr const char *kSummaryAttribute = "summary";
  static constexpr const char *kInterfaceAttribute = "interface";
  static constexpr const char *kEnumAttribute = "enum";
  static constexpr const char *kAllowNullAttribute = "allow-null";
  static constexpr const char *kSinceAttribute = "since";

  std::string name_;
  Type type_;
  std::optional<std::string> summary_;
  std::optional<std::string> interface_;
  std::optional<std::string> enum_;
  bool allowNull_;
  std::optional<uint32_t> since_;
};

class Description final {
 public:
  static constexpr const char *kElement = "description";

  Description(const std::string &summary,
              const std::optional<std::string> &text = std::nullopt);

  static Description Parse(const tinyxml2::XMLElement *element);

  inline std::string GetSummary() const { return summary_; }

  inline std::optional<std::string> GetText() const { return text_; }

 private:
  static constexpr const char *kSummaryAttribute = "summary";

  std::string summary_;
  std::optional<std::string> text_;
};

class Enum final {
 public:
  class Entry final {
   public:
    static constexpr const char *kElement = "entry";

    Entry(const std::string &name, uint32_t value,
          const std::optional<Description> &description_ = std::nullopt,
          std::optional<uint32_t> since = std::nullopt);

    static Entry Parse(const tinyxml2::XMLElement *element);

    inline const std::string &GetName() const { return name_; }

    inline uint32_t GetValue() const { return value_; }

    inline const std::optional<Description> &GetDescription() const {
      return description_;
    }

    inline std::optional<uint32_t> GetSince() const { return since_; }

   private:
    static constexpr const char *kNameAttribute = "name";
    static constexpr const char *kValueAttribute = "value";
    static constexpr const char *kSummaryAttribute = "summary";
    static constexpr const char *kSinceAttribute = "since";

    std::string name_;
    uint32_t value_;
    std::optional<Description> description_;
    std::optional<uint32_t> since_;
  };

  static constexpr const char *kElement = "enum";

  Enum(const std::string &name, const std::list<Entry> &entries,
       bool bitfield = false, std::optional<uint32_t> since = std::nullopt,
       const std::optional<Description> &description = std::nullopt);

  static Enum Parse(const tinyxml2::XMLElement *element);

  inline const std::string &GetName() const { return name_; }

  inline const std::list<Entry> &GetEntries() const { return entries_; }

  inline bool IsBitfield() const { return bitfield_; }

  inline std::optional<uint32_t> GetSince() const { return since_; }

  inline const std::optional<Description> &GetDescription() const {
    return description_;
  }

 private:
  static constexpr const char *kNameAttribute = "name";
  static constexpr const char *kBitfieldAttribute = "bitfield";
  static constexpr const char *kSinceAttribute = "since";

  std::string name_;
  std::list<Entry> entries_;

  bool bitfield_;
  std::optional<uint32_t> since_;
  std::optional<Description> description_;
};

class Event final {
 public:
  static constexpr const char *kElement = "event";

  Event(const std::string &name, const std::list<Argument> &arguments,
        const std::optional<Description> &description,
        std::optional<uint32_t> since = std::nullopt)
      : name_(name),
        arguments_(arguments),
        description_(description),
        since_(since) {}

  static Event Parse(const tinyxml2::XMLElement *element);

  const std::string &GetName() const { return name_; }

  const std::list<Argument> &GetArguments() const { return arguments_; }

  const std::optional<Description> &GetDescription() const {
    return description_;
  }

  std::optional<uint32_t> GetSince() const { return since_; }

 private:
  static constexpr const char *kNameAttribute = "name";
  static constexpr const char *kSinceAttribute = "since";

  std::string name_;
  std::list<Argument> arguments_;

  std::optional<Description> description_;
  std::optional<uint32_t> since_;
};

class Request final {
 public:
  static constexpr const char *kElement = "request";

  enum class Type { kDefault, kDestructor };

  Request(const std::string &name, Type type, ::Description description,
          const std::list<Argument> &arguments,
          std::optional<uint32_t> since = std::nullopt);

  static Request Parse(tinyxml2::XMLElement *element);

  inline std::string GetName() const { return name_; }

  inline Type GetType() const { return type_; }

  inline const Description &GetDescription() const { return description_; }

  inline const std::list<Argument> &GetArguments() const { return arguments_; }

  inline std::optional<uint32_t> GetSince() const { return since_; }

 private:
  static constexpr const char *kNameAttribute = "name";
  static constexpr const char *kTypeAttribute = "type";
  static constexpr const char *kSinceAttribute = "since";

  std::string name_;
  Type type_;
  ::Description description_;
  std::list<Argument> arguments_;
  std::optional<uint32_t> since_;
};

struct Interface final {
  static constexpr const char *kElement = "interface";

  std::string Name;
  uint32_t Version = 0;
  std::optional<::Description> Description;

  std::list<Enum> Enums;
  std::list<Event> Events;
  std::list<Request> Requests;

  static Interface Parse(tinyxml2::XMLElement *element);

 private:
  static constexpr const char *kNameAttribute = "name";
  static constexpr const char *kVersionAttribute = "version";
};

struct Protocol final {
  static constexpr const char *kElement = "protocol";

  const char *Name = nullptr;
  const char *Copyright = nullptr;

  std::list<Interface> Interfaces;

  static Protocol Parse(tinyxml2::XMLElement *element);

 private:
  static constexpr const char *kNameAttribute = "name";

  static constexpr const char *kCopyrightElement = "copyright";
};
