load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "fmt",
    srcs = [
        "src/format.cc",
        "src/posix.cc",
    ],
    hdrs = [
        "include/fmt/chrono.h",
        "include/fmt/color.h",
        "include/fmt/compile.h",
        "include/fmt/core.h",
        "include/fmt/format.h",
        "include/fmt/format-inl.h",
        "include/fmt/locale.h",
        "include/fmt/ostream.h",
        "include/fmt/posix.h",
        "include/fmt/printf.h",
        "include/fmt/ranges.h",
    ],
    strip_include_prefix = "include",
)
