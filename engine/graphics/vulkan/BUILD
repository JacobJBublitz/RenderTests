load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "vulkan",
    srcs = [
        "device.cc",
        "renderer.cc",
    ],
    hdrs = [
        "device.h",
        "renderer.h",
        "surface.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_google_glog//:glog",
        "@fmt",
        "@vulkan_headers",
        "@vulkan_loader",
    ],
)
