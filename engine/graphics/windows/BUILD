load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "windows",
    srcs = ["window.cc"],
    hdrs = ["window.h"],
    linkopts = ["User32.lib"],
    visibility = ["//visibility:public"],
    deps = [
        "//engine/graphics:window",
        "@com_github_google_glog//:glog",
    ],
)
