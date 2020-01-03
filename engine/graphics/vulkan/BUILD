load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "vulkan",
    srcs = ["renderer.cc"],
    hdrs = ["renderer.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_google_glog//:glog",
        "@vulkan_headers",
        "@vulkan_loader",
    ],
)
