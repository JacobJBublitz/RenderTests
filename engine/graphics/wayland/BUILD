load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "wayland",
    srcs = [
        "display.cc",
        "monitor.cc",
        "window.cc",
    ],
    hdrs = [
        "display.h",
        "monitor.h",
        "window.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//engine/graphics:window",
        "//engine/graphics/egl",
        "//engine/graphics/vulkan",
        "//engine/graphics/wayland/internal",
        "@com_github_google_glog//:glog",
        "@fmt",
    ],
)
