load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//engine/graphics:__subpackages__"])

cc_library(
    name = "window",
    srcs = ["window.cc"],
    hdrs = ["window.h"],
)

cc_library(
    name = "graphics",
    visibility = ["//visibility:public"],
    deps = [
        ":window",
        "//engine/graphics/vulkan",
    ] + select({
        "@//platforms:is_linux": ["//engine/graphics/wayland"],
        "@//platforms:is_windows": ["//engine/graphics/windows"],
    }),
)
