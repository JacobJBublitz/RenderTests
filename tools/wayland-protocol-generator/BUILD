load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "wayland-protocol-generator",
    srcs = [
        "cpp_generator.cc",
        "cpp_generator.h",
        "generator.cc",
        "generator.h",
        "main.cc",
        "protocol.cc",
        "protocol.h",
    ],
    deps = [
        "@com_github_gflags_gflags//:gflags",
        "@fmt",
        "@tinyxml2",
    ],
)
